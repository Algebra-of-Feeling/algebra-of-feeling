// source_gate.hpp — ADR009: C++23, sem dependências externas.
//
// Lógica do portão de admissão de fontes, extraída de verify.cpp para ser
// compartilhada por verify.cpp (pipeline positivo) e negative_control.cpp
// (pipeline adversarial). A extração existe para uma única razão: um
// controle negativo só demonstra algo se testar exatamente o mesmo código
// que protege o pipeline real. Duas cópias do mesmo regex, mantidas por
// mãos e datas diferentes, divergem cedo ou tarde — e a primeira vez que
// divergirem é exatamente quando o controle negativo passa a mentir.
//
// Este header preserva, termo a termo, o comportamento que verify.cpp já
// tinha antes desta extração: mesmas expressões regulares, mesmas
// mensagens de erro, mesma ordem de checagem.
#pragma once
#include <filesystem>
#include <fstream>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>

namespace SourceGate {
namespace fs = std::filesystem;

inline std::string readFile(const fs::path& p) {
  std::ifstream f(p);
  if (!f) throw std::runtime_error("Cannot read: " + p.string());
  return {std::istreambuf_iterator<char>(f), {}};
}

inline const std::regex& forbiddenTokenRegex() {
  static const std::regex r(
      R"(\b(sorry|admit|native_decide|sorryAx|unsafe|implemented_by)\b)");
  return r;
}

inline const std::regex& forbiddenDeclRegex() {
  static const std::regex r(R"(^\s*(private\s+)?(axiom|opaque)\s)");
  return r;
}

// Verifica o conteúdo já lido de um arquivo-fonte. Lança std::runtime_error
// na primeira violação encontrada; retorna normalmente se o conteúdo for
// limpo. `path` entra apenas na mensagem de erro — permite testar conteúdo
// sintético sob um nome de arquivo real ou fictício, sem tocar o disco.
inline void checkFile(const fs::path& path, const std::string& content) {
  if (std::regex_search(content, forbiddenTokenRegex()))
    throw std::runtime_error("Forbidden source token: " + path.string());
  std::istringstream in(content);
  std::string line;
  while (std::getline(in, line)) {
    if (std::regex_search(line, forbiddenDeclRegex()))
      throw std::runtime_error("Forbidden declaration: " + path.string());
    std::istringstream words(line);
    std::string word;
    words >> word;
    if (word == "import") {
      while (words >> word) {
        if (word != "Init" && word != "Lean" && word != "RCDA" &&
            !word.starts_with("RCDA."))
          throw std::runtime_error("Unapproved import: " + word);
      }
    }
  }
}

// Sobrecarga de conveniência: lê o arquivo do disco e checa.
inline void checkFile(const fs::path& path) { checkFile(path, readFile(path)); }

inline void checkManifest(const std::string& manifestContent) {
  if (!std::regex_search(manifestContent,
                          std::regex(R"("packages"\s*:\s*\[\s*\])")))
    throw std::runtime_error("External packages are not permitted");
}

}  // namespace SourceGate
