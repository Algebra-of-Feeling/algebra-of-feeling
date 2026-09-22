// ADR009: C++23 orchestrates; Lean 4 checks proofs and computes audit counts.
//
// Alteração desta correção: o portão de admissão de fontes foi extraído
// para source_gate.hpp (ver esse arquivo para a razão). O comportamento é
// preservado termo a termo — mesmas regras, mesmas mensagens de erro,
// mesma ordem. O pipeline agora também exige, como pré-requisito, que
// verification/negative-control.log já exista e registre PASS: um
// verificador cujo portão nunca foi visto rejeitando nada não corrobora
// nada sobre esse portão.
#include "source_gate.hpp"
#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
namespace fs = std::filesystem;
std::string read(const fs::path& p) { return SourceGate::readFile(p); }
void write(const fs::path& p, const std::string& s) {
  std::ofstream f(p); if (!(f << s)) throw std::runtime_error("Cannot write: " + p.string());
}
std::string shell(const std::string& s) {
  std::string r="'"; for(char c:s) r += c=='\'' ? "'\\''" : std::string(1,c); return r+"'";
}
std::string json(const std::string& s) {
  std::string r="\""; for(char c:s) {
    if(c=='\"'||c=='\\') { r+='\\'; r+=c; }
    else if(c=='\n') r+="\\n";
    else if(c=='\r') r+="\\r";
    else if(c=='\t') r+="\\t";
    else if(static_cast<unsigned char>(c)<32) throw std::runtime_error("Invalid control character");
    else r+=c;
  } return r+'\"';
}
std::string run(const std::string& command, const std::string& log) {
  int result=std::system((command+" > "+shell("verification/"+log)+" 2>&1").c_str());
  auto output=read("verification/"+log);
  if(result!=0) throw std::runtime_error("Command failed: "+command+"\n"+output);
  return output;
}
std::string lineStarting(const std::string& text,const std::string& prefix) {
  std::istringstream in(text); std::string line;
  while(std::getline(in,line)) if(line.starts_with(prefix)) return line;
  throw std::runtime_error("Missing receipt line: "+prefix);
}
int main(int argc,char** argv) {
  try {
    if(argc>2) throw std::runtime_error("Usage: verify [project-root]");
    if(argc==2) fs::current_path(argv[1]);
    fs::create_directories("verification");
    // Invalidate a prior PASS before any check can fail.
    fs::remove("verification/receipt.json");

    // Pré-requisito: o controle negativo precisa ter rodado nesta mesma
    // árvore de trabalho e ter passado. verify.sh garante a ordem; esta
    // checagem existe para que rodar verify sozinho, fora de ordem, falhe
    // alto e cedo em vez de emitir um PASS sem essa evidência.
    if(!fs::exists("verification/negative-control.log"))
      throw std::runtime_error("Missing verification/negative-control.log — run scripts/negative_control before verify (see verify.sh).");
    auto negLog=read("verification/negative-control.log");
    if(negLog.find("NEGATIVE CONTROL SUITE PASS")==std::string::npos)
      throw std::runtime_error("verification/negative-control.log does not record a PASS — the source gate has not been shown to reject admission attempts.");

    std::vector<fs::path> sources={"RCDA.lean","Audit.lean"};
    for(auto const& entry:fs::directory_iterator("RCDA"))
      if(entry.path().extension()==".lean") sources.push_back(entry.path());
    std::sort(sources.begin(),sources.end());
    for(auto const& path:sources) SourceGate::checkFile(path);
    SourceGate::checkManifest(read("lake-manifest.json"));

    auto version=run("lake env lean --version","toolchain.log");
    run("lake clean","clean.log");
    run("lake build","build.log");
    auto audit=run("lake env lean -DwarningAsError=true Audit.lean","axioms.log");
    auto summary=lineStarting(audit,"AUDIT PASS:");
    auto foundations=lineStarting(audit,"FOUNDATIONAL AXIOMS USED:");
    for(auto name:{"shifted_frozen_associative","associative_contexts_nonzero_operational","faithful_updates_force_associativity"}) {
      auto claim=std::string("CLAIM RCDAAudit.Tag.proved RCDA.RelationalComposition.")+name;
      if(audit.find(claim)==std::string::npos) throw std::runtime_error("Missing claim: "+claim);
    }
    std::vector<fs::path> tracked=sources;
    for(auto file:{"lean-toolchain","lakefile.toml","lake-manifest.json","AUDIT.md","README.md","PROOF_LEDGER.md",
                    "scripts/source_gate.hpp","scripts/verify.cpp","scripts/verify.sh",
                    "scripts/negative_control.cpp"}) tracked.emplace_back(file);
    std::sort(tracked.begin(),tracked.end());
    std::string command="shasum -a 256";
    for(auto const& path:tracked) command+=" "+shell(path.generic_string());
    run(command,"sha256.txt");
    write("verification/receipt.json","{\n  \"status\": \"PASS\",\n  \"verifier\": \"C++23 scripts/verify.cpp\",\n  \"proof_authority\": \"Lean 4 kernel\",\n  \"lean\": "+json(version)+",\n  \"source_gate\": \"PASS\",\n  \"negative_control\": \"PASS (see verification/negative-control.log)\",\n  \"clean_build_exit_code\": 0,\n  \"axiom_audit_exit_code\": 0,\n  \"summary\": "+json(summary)+",\n  \"foundational_axioms\": "+json(foundations)+",\n  \"source_hashes\": \"sha256.txt\",\n  \"external_packages\": [],\n  \"runtime_evidence_revalidated\": false\n}\n");
    std::cout<<version<<summary<<'\n'<<foundations<<"\nPASS: negative control, fresh clean build and axiom audit; verification/receipt.json\n";
    return 0;
  } catch(std::exception const& e) { std::cerr<<"FAIL: "<<e.what()<<'\n'; return 1; }
}
