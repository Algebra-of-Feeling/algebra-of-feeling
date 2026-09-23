// bloch_core.hpp — ADR009: C++23, sem dependências externas, sem E/S.
//
// Núcleo computacional de cd_bloch.cpp, extraído para ser compartilhado
// por cd_bloch.cpp (log legível, o artefato já verificado byte a byte
// duas vezes contra o original) e dump_bloch.cpp (despejo estruturado
// para o comparador C++/Sounio). Nenhuma fórmula foi alterada em relação
// ao cd_bloch.cpp recebido do autor: mesma convenção de multiplicação de
// Cayley–Dickson, mesma regra de ligação de Bloch, mesma ordem de
// operações. A extração separa cálculo de impressão; não muda nenhum dos
// dois.
#pragma once
#include <array>
#include <cstdlib>
#include <numeric>
#include <optional>
#include <vector>

namespace BlochCore {

// --- Cayley–Dickson, inteiros exatos --------------------------------------
using V = std::vector<long>;
inline V cj(const V& x) { V r = x; for (size_t i = 1; i < r.size(); ++i) r[i] = -r[i]; return r; }
inline V vadd(const V& x, const V& y) { V r(x.size()); for (size_t i = 0; i < r.size(); ++i) r[i] = x[i] + y[i]; return r; }
inline V vsub(const V& x, const V& y) { V r(x.size()); for (size_t i = 0; i < r.size(); ++i) r[i] = x[i] - y[i]; return r; }
inline V cdmul(const V& x, const V& y) {
  size_t n = x.size(); if (n == 1) return V{x[0] * y[0]}; size_t h = n / 2;
  V a(x.begin(), x.begin() + h), b(x.begin() + h, x.end()), c(y.begin(), y.begin() + h), d(y.begin() + h, y.end());
  V f = vsub(cdmul(a, c), cdmul(cj(d), b)), s = vadd(cdmul(d, a), cdmul(b, cj(c)));
  f.insert(f.end(), s.begin(), s.end()); return f;
}
inline V e(size_t n, size_t k) { V r(n, 0); r[k] = 1; return r; }

// --- Racionais exatos (long, sem precisão arbitrária) ---------------------
struct Q {
  long n, d;
  Q(long a = 0, long b = 1) {
    long g = std::gcd(std::labs(a), std::labs(b)); if (!g) g = 1;
    if (b < 0) { a = -a; b = -b; } n = a / g; d = b / g;
  }
  Q operator+(Q o) const { return Q(n * o.d + o.n * d, d * o.d); }
  Q operator-(Q o) const { return Q(n * o.d - o.n * d, d * o.d); }
  Q operator*(Q o) const { return Q(n * o.n, d * o.d); }
  bool operator<=(Q o) const { return n * o.d <= o.n * d; }
  bool operator==(Q o) const { return n == o.n && d == o.d; } // já normalizado no ctor
  double toDouble() const { return static_cast<double>(n) / static_cast<double>(d); }
};

using M = std::array<std::array<Q, 2>, 2>;
inline M mul(const M& a, const M& b) { M r{}; for (int i = 0; i < 2; i++) for (int j = 0; j < 2; j++) { Q s; for (int k = 0; k < 2; k++) s = s + a[i][k] * b[k][j]; r[i][j] = s; } return r; }
inline M add(const M& a, const M& b) { M r{}; for (int i = 0; i < 2; i++) for (int j = 0; j < 2; j++) r[i][j] = a[i][j] + b[i][j]; return r; }
inline M scale(Q c, const M& a) { M r{}; for (int i = 0; i < 2; i++) for (int j = 0; j < 2; j++) r[i][j] = c * a[i][j]; return r; }
inline Q tr(const M& a) { return a[0][0] + a[1][1]; }
inline Q det(const M& m) { return m[0][0] * m[1][1] - m[0][1] * m[1][0]; }

inline const M I{{{Q(1), Q(0)}, {Q(0), Q(1)}}};
inline const M P{{{Q(1), Q(0)}, {Q(0), Q(0)}}};
inline const M Pm{{{Q(0), Q(0)}, {Q(0), Q(1)}}};
inline const M Qp{{{Q(1, 2), Q(1, 2)}, {Q(1, 2), Q(1, 2)}}};
inline const M Qm{{{Q(1, 2), Q(-1, 2)}, {Q(-1, 2), Q(1, 2)}}};

inline Q sval(long r7) { return Q(r7, 1 + std::labs(r7)); }
inline bool in_domain(long r7, Q kx) { Q s = sval(r7); return (Q(1) + kx * kx) * s * s <= Q(1); }
inline M sigma(long r7, Q kx) { Q s = sval(r7); M m{}; m[0][0] = (Q(1) + s) * Q(1, 2); m[1][1] = (Q(1) - s) * Q(1, 2); m[0][1] = m[1][0] = kx * s * Q(1, 2); return m; }
inline M U(const M& rho, long r7, Q kx) { return add(scale(Q(1, 2), rho), scale(Q(1, 2), sigma(r7, kx))); }
inline Q joint(const M& rho, const M& A, const M& B) { return tr(mul(mul(B, mul(mul(A, rho), A)), B)); }

// Uma linha de tabela admissível (o que table() no cd_bloch.cpp original
// calculava antes de imprimir). pq[i][j] = joint(rho, P_i, Q_j); indexação
// de qp preservada exatamente como no original: qp[j][i] = joint(rho, Q_j, P_i).
struct TableRow {
  M rho;
  Q detRho, pPplus, pQplus;
  Q pq[2][2];
  Q qp[2][2];
  Q marginalPafterQ, qQQ;
};

struct TableResult {
  bool admissible;
  Q detSigmaIfRejected; // significativo apenas quando !admissible
  std::optional<TableRow> row;
};

// Mesma sequência de cálculo de table() no cd_bloch.cpp original, sem a
// parte de impressão.
inline TableResult computeTable(long r7, Q kx) {
  if (!in_domain(r7, kx)) return {false, det(sigma(r7, kx)), std::nullopt};
  M rho0 = scale(Q(1, 2), I);
  M rho = U(rho0, r7, kx);
  TableRow row{};
  row.rho = rho;
  row.detRho = det(rho);
  row.pPplus = tr(mul(P, rho));
  row.pQplus = tr(mul(Qp, rho));
  const M* Ps[2] = {&P, &Pm};
  const M* Qs[2] = {&Qp, &Qm};
  for (int i = 0; i < 2; i++) for (int j = 0; j < 2; j++) {
    row.pq[i][j] = joint(rho, *Ps[i], *Qs[j]);
    row.qp[j][i] = joint(rho, *Qs[j], *Ps[i]);
  }
  row.marginalPafterQ = row.qp[0][0] + row.qp[1][0];
  row.qQQ = row.pq[0][1] + row.pq[1][0] - row.qp[0][1] - row.qp[1][0];
  return {true, Q(0), row};
}

} // namespace BlochCore
