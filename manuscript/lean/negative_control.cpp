// negative_control.cpp — ADR009: C++23, sem dependências externas.
//
// Testa adversarialmente o mesmo portão de admissão usado por verify.cpp
// (source_gate.hpp). Não recompila Lean: as quatro checagens abaixo
// acontecem em texto, antes de qualquer invocação de `lake`, exatamente
// onde o portão real intercepta uma tentativa de admissão. Por isso este
// binário roda sem toolchain Lean instalada.
//
// Cada mutação é aplicada em memória sobre o conteúdo real de
// RCDA/RelationalComposition.lean — nenhum arquivo em disco é escrito ou
// alterado. Um controle positivo (a fonte real, sem mutação) acompanha os
// negativos: um portão que rejeitasse tudo, inclusive o que é legítimo,
// passaria os negativos e ainda assim estaria quebrado.
//
// Saída: verification/negative-control.log. Código de saída 0 somente se
// todos os casos tiverem o resultado esperado (aceitar o limpo, rejeitar
// cada admissão). verify.cpp recusa-se a emitir PASS sem esse log.
#include "source_gate.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
namespace fs = std::filesystem;

struct Case {
  std::string name;
  std::string mutatedContent;
  bool expectRejection;
};

struct Outcome {
  Case const* c;
  bool rejected;
  std::string message;
};

std::string replaceOnce(const std::string& content, const std::string& anchor,
                         const std::string& replacement, const std::string& caseName) {
  auto pos = content.find(anchor);
  if (pos == std::string::npos)
    throw std::runtime_error("fixture anchor not found for '" + caseName +
                              "' — RelationalComposition.lean may have changed; update the fixture.");
  std::string mutated = content;
  mutated.replace(pos, anchor.size(), replacement);
  return mutated;
}

int main(int argc, char** argv) {
  try {
    fs::path root = (argc == 2) ? fs::path(argv[1]) : fs::path("..");
    fs::create_directories(root / "verification");
    fs::path relComp = root / "RCDA" / "RelationalComposition.lean";
    std::string clean = SourceGate::readFile(relComp);

    std::vector<Case> cases;

    // Controle positivo: a fonte real, sem mutação, precisa ser aceita.
    cases.push_back({"positive-control: unmodified RelationalComposition.lean", clean, false});

    // Negativo 1: token de admissão ('sorry') no lugar de uma prova real.
    cases.push_back({"negative-control-1: sorry spliced into frozen_protocol_reduces",
                      replaceOnce(clean,
                        "simp only [operationalDefect, leftValue, rightValue, frozenDefect, hl, hr]",
                        "sorry", "negative-control-1"),
                      true});

    // Negativo 2: 'native_decide' no lugar de 'decide' num teorema real
    // (associative_contexts_nonzero_operational). Único 'decide' nessa
    // âncora exata nesse teorema, dada a indentação de duas colunas.
    cases.push_back({"negative-control-2: native_decide substituted for decide",
                      replaceOnce(clean,
                        "operationalDefect shifted () (afterComposite shifted ()) 0 1 2 3 = 1 := by\n  decide",
                        "operationalDefect shifted () (afterComposite shifted ()) 0 1 2 3 = 1 := by\n  native_decide",
                        "negative-control-2"),
                      true});

    // Negativo 3: declaração 'axiom' anexada — burla por decreto lógico.
    cases.push_back({"negative-control-3: axiom declaration appended",
                      clean + "\naxiom admission_backdoor : False\n", true});

    // Negativo 4: import não aprovado.
    cases.push_back({"negative-control-4: unapproved import (Mathlib.Tactic)",
                      "import Mathlib.Tactic\n" + clean, true});

    std::vector<Outcome> outcomes;
    for (auto const& c : cases) {
      try {
        SourceGate::checkFile(relComp, c.mutatedContent);
        outcomes.push_back({&c, false, "(none — content accepted by the gate)"});
      } catch (std::exception const& e) {
        outcomes.push_back({&c, true, e.what()});
      }
    }

    std::ofstream log((root / "verification" / "negative-control.log").string());
    bool allOk = true;
    for (auto const& o : outcomes) {
      bool ok = (o.rejected == o.c->expectRejection);
      allOk = allOk && ok;
      std::string line = std::string(ok ? "OK   " : "FAIL ") + o.c->name +
                          " | rejected=" + (o.rejected ? "yes" : "no") +
                          " | expected_rejection=" + (o.c->expectRejection ? "yes" : "no") +
                          " | message=" + o.message;
      log << line << "\n";
      std::cout << line << "\n";
    }
    if (allOk) {
      log << "NEGATIVE CONTROL SUITE PASS: gate accepted the unmodified source and rejected all "
          << (outcomes.size() - 1) << " admission attempts tested.\n";
      std::cout << "NEGATIVE CONTROL SUITE PASS: gate accepted the unmodified source and rejected all "
                << (outcomes.size() - 1) << " admission attempts tested.\n";
      return 0;
    }
    log << "NEGATIVE CONTROL SUITE FAIL: see above.\n";
    std::cerr << "NEGATIVE CONTROL SUITE FAIL: see above.\n";
    return 1;
  } catch (std::exception const& e) {
    std::cerr << "FAIL: " << e.what() << "\n";
    return 1;
  }
}
