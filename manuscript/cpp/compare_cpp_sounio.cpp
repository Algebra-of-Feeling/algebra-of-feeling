// compare_cpp_sounio.cpp — ADR009: C++23, sem dependências externas.
//
// Compara, campo a campo, o despejo canônico C++ (dump_bloch.cpp, frações
// exatas convertidas a double) contra um despejo Sounio no mesmo formato de
// chave (f64 nativo). Implementa o item 5 do ADR009 de forma automatizada:
// até aqui a concordância C++/Sounio era avaliada por leitura humana dos
// dois logs; este comparador decide por tolerância declarada, célula a
// célula, e falha alto se qualquer célula ultrapassar a tolerância ou se
// o conjunto de chaves divergir entre os dois lados.
//
// Uso: compare_cpp_sounio <dump_cpp.tsv> <dump_sounio.tsv> [tolerancia_abs]
// Formato de cada linha de entrada: chave\tvalor[\t...restante ignorado]
// Tolerância absoluta default: 1e-9 (ver justificativa no README anexo —
// as frações desta realização têm denominador potência de 2, logo double
// as representa exatamente; a tolerância existe para f64 arbitrário, não
// para acomodar imprecisão esperada deste caso específico).
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

std::map<std::string, double> readDump(const std::string& path) {
  std::ifstream f(path);
  if (!f) throw std::runtime_error("Cannot read: " + path);
  std::map<std::string, double> m;
  std::string line;
  while (std::getline(f, line)) {
    if (line.empty()) continue;
    std::istringstream in(line);
    std::string key, value;
    if (!std::getline(in, key, '\t')) continue;
    if (!std::getline(in, value, '\t')) continue;
    try {
      m[key] = std::stod(value);
    } catch (...) {
      throw std::runtime_error("Non-numeric value for key '" + key + "' in " + path + ": '" + value + "'");
    }
  }
  return m;
}

int main(int argc, char** argv) {
  if (argc < 3 || argc > 4) {
    std::cerr << "Usage: compare_cpp_sounio <dump_cpp.tsv> <dump_sounio.tsv> [abs_tolerance]\n";
    return 2;
  }
  double tol = (argc == 4) ? std::stod(argv[3]) : 1e-9;
  try {
    auto cpp = readDump(argv[1]);
    auto sounio = readDump(argv[2]);

    std::size_t missingInSounio = 0, missingInCpp = 0, mismatched = 0, compared = 0;
    double worstAbsDiff = 0.0;
    std::string worstKey;

    for (auto const& [key, cppValue] : cpp) {
      auto it = sounio.find(key);
      if (it == sounio.end()) { std::cout << "MISSING-IN-SOUNIO\t" << key << "\n"; ++missingInSounio; continue; }
      double diff = std::fabs(cppValue - it->second);
      ++compared;
      bool ok = diff <= tol;
      if (!ok) ++mismatched;
      if (diff > worstAbsDiff) { worstAbsDiff = diff; worstKey = key; }
      std::cout << (ok ? "OK   " : "FAIL ") << key
                << "\tcpp=" << cppValue << "\tsounio=" << it->second
                << "\tabs_diff=" << diff << "\n";
    }
    for (auto const& [key, _] : sounio) {
      if (!cpp.contains(key)) { std::cout << "MISSING-IN-CPP\t" << key << "\n"; ++missingInCpp; }
    }

    std::cout << "\n--- resumo ---\n";
    std::cout << "campos comparados: " << compared << "\n";
    std::cout << "divergencias acima da tolerancia (" << tol << "): " << mismatched << "\n";
    std::cout << "ausentes no lado Sounio: " << missingInSounio << "\n";
    std::cout << "ausentes no lado C++: " << missingInCpp << "\n";
    if (compared > 0) std::cout << "maior desvio absoluto: " << worstAbsDiff << " em " << worstKey << "\n";

    bool pass = (mismatched == 0) && (missingInSounio == 0) && (missingInCpp == 0) && (compared > 0);
    std::cout << (pass ? "COMPARATOR PASS\n" : "COMPARATOR FAIL\n");
    return pass ? 0 : 1;
  } catch (std::exception const& e) {
    std::cerr << "FAIL: " << e.what() << "\n";
    return 1;
  }
}
