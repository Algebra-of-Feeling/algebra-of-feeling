// cd_bloch.cpp — Álgebra do Sentir v0.3.1, suplemento C++23 único: produto de Cayley–Dickson -> r7 -> Bloch -> instrumentos.
// Convenção: (a,b)(c,d) = (ac - conj(d) b, d a + b conj(c)), conj(a,b) = (conj a, -b); base recursiva e0=1.
// Regra de ligação: s = r7/(1+|r7|); sigma = 1/2 (I + kx s X + s Z), kx in {1/2, 1 (controle)}; dominio (1+kx^2) s^2 <= 1.
//
// Nesta correção: o cálculo foi extraído para bloch_core.hpp, compartilhado
// com dump_bloch.cpp. A saída deste arquivo foi conferida, byte a byte,
// contra o cd-cpp-run.log original antes e depois da extração — ver
// verification/cd_bloch_refactor.log.
#include "bloch_core.hpp"
#include <cstdio>
using namespace BlochCore;

void pv(const char*s,const V&v){printf("%s =",s);bool z=true;for(size_t i=0;i<v.size();++i)if(v[i]){printf(" %+ld e%zu",v[i],i);z=false;}if(z)printf(" 0");printf("\n");}
void pq_(const Q& q,const char*s=""){printf("%s%ld/%ld",s,q.n,q.d);}
void pm(const char*s,const M&a){printf("%s = [[",s);pq_(a[0][0]);pq_(a[0][1],", ");printf("],[");pq_(a[1][0]);pq_(a[1][1],", ");printf("]]  det=");pq_(det(a));printf("\n");}

void table(const char*name,long r7,Q kx){
  TableResult t=computeTable(r7,kx);
  if(!t.admissible){ printf("%s (r7=%ld): REJEITADO, fora do dominio; det sigma=",name,r7); pq_(t.detSigmaIfRejected); printf("\n"); return; }
  const TableRow& row=*t.row;
  char nm[96]; snprintf(nm,96,"%s (r7=%ld)",name,r7); pm(nm,row.rho);
  printf("  p(P+)="); pq_(row.pPplus); printf("  p(Q+)="); pq_(row.pQplus); printf("\n");
  const char* sg[2]={"+","-"};
  printf("  P->Q:"); for(int i=0;i<2;i++)for(int j=0;j<2;j++){printf(" (P%s,Q%s)=",sg[i],sg[j]);pq_(row.pq[i][j]);} printf("\n");
  printf("  Q->P:"); for(int j=0;j<2;j++)for(int i=0;i<2;i++){printf(" (Q%s,P%s)=",sg[j],sg[i]);pq_(row.qp[j][i]);} printf("\n");
  printf("  marginal P+ apos Q="); pq_(row.marginalPafterQ); printf("  q_QQ="); pq_(row.qQQ); printf("\n");
}
int main(){
  // 1. composição: octônios A3
  V x=e(8,1),y=e(8,2),z=e(8,4);
  V rL=cdmul(cdmul(x,y),z), rR=cdmul(x,cdmul(y,z));
  pv("r_L = (e1 e2) e4",rL); pv("r_R = e1 (e2 e4)",rR); pv("associador",vsub(rL,rR));
  // 2. composição: sedênios A4, divisor de zero
  V a=vadd(e(16,1),e(16,10)), zz=vsub(e(16,4),e(16,15)); V r0=cdmul(a,zz);
  pv("r_0 = (e1+e10)(e4-e15)",r0);
  long L7=rL[7], R7=rR[7], Z7=r0[7];
  printf("coeficientes e7: r_L=%ld r_R=%ld r_0=%ld\n",L7,R7,Z7);
  // 3. ligação e instrumentos, nos dois regimes
  for(Q kx : {Q(1,2),Q(1)}){
    printf("=== kx="); pq_(kx); printf(" %s ===\n", kx.n==1&&kx.d==2 ? "(quebra de simetria)" : "(controle simetrico)");
    table("rho_L",L7,kx); table("rho_R",R7,kx); table("rho_0",Z7,kx); table("codigo invalido",9,kx);
  }
}
