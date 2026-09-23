// ADR009: C++23, racionais exatos (bloch_core.hpp).
// Decoerência de intensidade lambda sobre rho_L e rho_R do manuscrito:
// coerências multiplicadas por lambda, populações intactas.
// Mede Delta_Q = p(Q+) - p(Q+ após P), Delta_P = p(P+) - p(P+ após Q), e q_QQ.
#include "bloch_core.hpp"
#include <cstdio>
using namespace BlochCore;
void p(const char* s, Q q){ printf("%s%ld/%ld", s, q.n, q.d); }
int main(){
  const M rhos[2] = { U(scale(Q(1,2),I), 1, Q(1,2)), U(scale(Q(1,2),I), -1, Q(1,2)) };
  const char* names[2] = {"rho_L","rho_R"};
  for(int k=0;k<2;++k){
    printf("=== %s ===\n", names[k]);
    for(Q lam : {Q(1),Q(3,4),Q(1,2),Q(1,4),Q(0)}){
      M r = rhos[k]; r[0][1] = r[0][1]*lam; r[1][0] = r[1][0]*lam;
      Q pP = tr(mul(P,r)), pQ = tr(mul(Qp,r));
      Q pQafterP = joint(r,P,Qp) + joint(r,Pm,Qp);
      Q pPafterQ = joint(r,Qp,P) + joint(r,Qm,P);
      Q qq = joint(r,P,Qm)+joint(r,Pm,Qp)-joint(r,Qp,Pm)-joint(r,Qm,P);
      Q x2 = r[0][1], z2 = (r[0][0]-r[1][1])*Q(1,2);
      p("lambda=",lam); p("  coer=",r[0][1]);
      p("  Delta_Q=",pQ-pQafterP); p(" (x/2=",x2); printf(")");
      p("  Delta_P=",pP-pPafterQ); p(" (z/2=",z2); printf(")");
      p("  qQQ=",qq); printf("  %s\n", ((pQ-pQafterP)==x2 && (pP-pPafterQ)==z2) ? "OK" : "MISMATCH");
    }
  }
}
