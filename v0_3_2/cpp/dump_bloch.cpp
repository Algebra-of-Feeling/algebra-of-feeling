// dump_bloch.cpp — ADR009: C++23, sem dependências externas.
//
// Mesmo cálculo de cd_bloch.cpp (via bloch_core.hpp), despejado em formato
// chave-valor de uma linha por campo (TSV: chave\tvalor_double\tfracao_exata),
// para consumo por compare_cpp_sounio.cpp. Não substitui cd_bloch.cpp — é
// um segundo modo de saída do mesmo núcleo, para comparação automatizada.
//
// O valor double de cada campo é a referência canônica C++ (ADR009, item 5).
// A fração exata acompanha para auditoria humana; o comparador usa o double.
#include "bloch_core.hpp"
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>
using namespace BlochCore;

static std::ofstream* out;

void emit(const std::string& key, Q value) {
  char buf[64];
  snprintf(buf, sizeof buf, "%.17g", value.toDouble());
  (*out) << key << '\t' << buf << '\t' << value.n << '/' << value.d << '\n';
}
void emitFlag(const std::string& key, bool value) {
  (*out) << key << '\t' << (value ? "1" : "0") << '\t' << (value ? "1/1" : "0/1") << '\n';
}

void dumpRegime(const std::string& regime, long L7, long R7, long Z7, Q kx) {
  struct Code { const char* name; long r7; };
  Code codes[4] = {{"rho_L", L7}, {"rho_R", R7}, {"rho_0", Z7}, {"invalid", 9}};
  for (auto const& code : codes) {
    std::string prefix = regime + "." + code.name + ".";
    TableResult t = computeTable(code.r7, kx);
    emitFlag(prefix + "admissible", t.admissible);
    if (!t.admissible) {
      emit(prefix + "det_sigma", t.detSigmaIfRejected);
      continue;
    }
    const TableRow& row = *t.row;
    emit(prefix + "rho_a", row.rho[0][0]);
    emit(prefix + "rho_b", row.rho[0][1]);
    emit(prefix + "rho_c", row.rho[1][0]);
    emit(prefix + "rho_d", row.rho[1][1]);
    emit(prefix + "det", row.detRho);
    emit(prefix + "p_Pplus", row.pPplus);
    emit(prefix + "p_Qplus", row.pQplus);
    emit(prefix + "pq_Pplus_Qplus", row.pq[0][0]);
    emit(prefix + "pq_Pplus_Qminus", row.pq[0][1]);
    emit(prefix + "pq_Pminus_Qplus", row.pq[1][0]);
    emit(prefix + "pq_Pminus_Qminus", row.pq[1][1]);
    emit(prefix + "qp_Qplus_Pplus", row.qp[0][0]);
    emit(prefix + "qp_Qplus_Pminus", row.qp[0][1]);
    emit(prefix + "qp_Qminus_Pplus", row.qp[1][0]);
    emit(prefix + "qp_Qminus_Pminus", row.qp[1][1]);
    emit(prefix + "marginal_P_after_Q", row.marginalPafterQ);
    emit(prefix + "qQQ", row.qQQ);
  }
}

int main(int argc, char** argv) {
  std::string outPath = (argc == 2) ? argv[1] : "verification/cpp_dump.tsv";
  std::ofstream f(outPath);
  if (!f) { fprintf(stderr, "Cannot write: %s\n", outPath.c_str()); return 1; }
  out = &f;

  V x = e(8, 1), y = e(8, 2), z = e(8, 4);
  V rL = cdmul(cdmul(x, y), z), rR = cdmul(x, cdmul(y, z));
  V a = vadd(e(16, 1), e(16, 10)), zz = vsub(e(16, 4), e(16, 15));
  V r0 = cdmul(a, zz);
  long L7 = rL[7], R7 = rR[7], Z7 = r0[7];

  f << "r_L7\t" << L7 << "\t" << L7 << "/1\n";
  f << "r_R7\t" << R7 << "\t" << R7 << "/1\n";
  f << "r_07\t" << Z7 << "\t" << Z7 << "/1\n";

  dumpRegime("asym", L7, R7, Z7, Q(1, 2));
  dumpRegime("sym", L7, R7, Z7, Q(1, 1));

  fprintf(stderr, "dump_bloch: wrote %s\n", outPath.c_str());
  return 0;
}
