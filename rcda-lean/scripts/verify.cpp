// ADR009: C++23 orchestrates; Lean 4 checks proofs and computes audit counts.
#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
namespace fs = std::filesystem;
std::string read(const fs::path& p) {
  std::ifstream f(p); if (!f) throw std::runtime_error("Cannot read: " + p.string());
  return {std::istreambuf_iterator<char>(f), {}};
}
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
    std::vector<fs::path> sources={"RCDA.lean","Audit.lean"};
    for(auto const& entry:fs::directory_iterator("RCDA"))
      if(entry.path().extension()==".lean") sources.push_back(entry.path());
    std::sort(sources.begin(),sources.end());
    const std::regex forbidden(R"(\b(sorry|admit|native_decide|sorryAx|unsafe|implemented_by)\b)");
    const std::regex axiom(R"(^\s*(private\s+)?(axiom|opaque)\s)");
    for(auto const& path:sources) {
      auto content=read(path);
      if(std::regex_search(content,forbidden)) throw std::runtime_error("Forbidden source token: "+path.string());
      std::istringstream in(content); std::string line;
      while(std::getline(in,line)) {
        if(std::regex_search(line,axiom)) throw std::runtime_error("Forbidden declaration: "+path.string());
        std::istringstream words(line); std::string word; words>>word;
        if(word=="import") while(words>>word) {
          if(word!="Init" && word!="Lean" && word!="RCDA" && !word.starts_with("RCDA."))
            throw std::runtime_error("Unapproved import: "+word);
        }
      }
    }
    if(!std::regex_search(read("lake-manifest.json"),std::regex(R"("packages"\s*:\s*\[\s*\])")))
      throw std::runtime_error("External packages are not permitted");
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
    for(auto file:{"lean-toolchain","lakefile.toml","lake-manifest.json","AUDIT.md","README.md","PROOF_LEDGER.md","scripts/verify.cpp","scripts/verify.sh"}) tracked.emplace_back(file);
    std::sort(tracked.begin(),tracked.end());
    std::string command="shasum -a 256";
    for(auto const& path:tracked) command+=" "+shell(path.generic_string());
    run(command,"sha256.txt");
    write("verification/receipt.json","{\n  \"status\": \"PASS\",\n  \"verifier\": \"C++23 scripts/verify.cpp\",\n  \"proof_authority\": \"Lean 4 kernel\",\n  \"lean\": "+json(version)+",\n  \"source_gate\": \"PASS\",\n  \"clean_build_exit_code\": 0,\n  \"axiom_audit_exit_code\": 0,\n  \"summary\": "+json(summary)+",\n  \"foundational_axioms\": "+json(foundations)+",\n  \"source_hashes\": \"sha256.txt\",\n  \"external_packages\": [],\n  \"runtime_evidence_revalidated\": false\n}\n");
    std::cout<<version<<summary<<'\n'<<foundations<<"\nPASS: fresh clean build and axiom audit; verification/receipt.json\n";
    return 0;
  } catch(std::exception const& e) { std::cerr<<"FAIL: "<<e.what()<<'\n'; return 1; }
}
