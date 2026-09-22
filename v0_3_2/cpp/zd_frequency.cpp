// zd_frequency.cpp — ADR009: C++23, sem dependências externas.
// Pergunta: para pares (a,b) de sedênios unitários tomados uniformemente na
// esfera S^15 x S^15, qual é a distribuição de |ab|? Com que frequência o
// produto fica "perto de zero"?
// Convenção de Cayley–Dickson idêntica a cd_bloch.cpp:
//   (a,b)(c,d) = (ac - conj(d) b, d a + b conj(c)),  conj(a,b) = (conj a, -b).
// Controle: octônios (dimensão 8), onde |ab| = |a||b| = 1 exatamente.
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <random>
#include <vector>

using V = std::vector<double>;
V cj(const V& x){ V r=x; for(size_t i=1;i<r.size();++i) r[i]=-r[i]; return r; }
V vadd(const V&x,const V&y){V r(x.size());for(size_t i=0;i<r.size();++i)r[i]=x[i]+y[i];return r;}
V vsub(const V&x,const V&y){V r(x.size());for(size_t i=0;i<r.size();++i)r[i]=x[i]-y[i];return r;}
V cdmul(const V& x,const V& y){
  size_t n=x.size(); if(n==1) return V{x[0]*y[0]}; size_t h=n/2;
  V a(x.begin(),x.begin()+h), b(x.begin()+h,x.end()), c(y.begin(),y.begin()+h), d(y.begin()+h,y.end());
  V f=vsub(cdmul(a,c),cdmul(cj(d),b)), s=vadd(cdmul(d,a),cdmul(b,cj(c)));
  f.insert(f.end(),s.begin(),s.end()); return f; }
double norm(const V& v){ double s=0; for(double x:v) s+=x*x; return std::sqrt(s); }

V randUnit(size_t n, std::mt19937_64& g){
  std::normal_distribution<double> N(0.0,1.0);
  V v(n); for(auto& x:v) x=N(g); double r=norm(v); for(auto& x:v) x/=r; return v;
}

void run(size_t dim, size_t samples, std::mt19937_64& g){
  std::vector<double> ps; ps.reserve(samples);
  for(size_t k=0;k<samples;++k){ V a=randUnit(dim,g), b=randUnit(dim,g); ps.push_back(norm(cdmul(a,b))); }
  std::sort(ps.begin(),ps.end());
  auto q=[&](double p){ return ps[static_cast<size_t>(p*(ps.size()-1))]; };
  auto frac=[&](double t){ return static_cast<double>(std::lower_bound(ps.begin(),ps.end(),t)-ps.begin())/ps.size(); };
  printf("dim=%zu samples=%zu\n",dim,samples);
  printf("  min=%.6f  q01=%.6f  q05=%.6f  q25=%.6f  median=%.6f  q75=%.6f  max=%.6f\n",
         ps.front(),q(0.01),q(0.05),q(0.25),q(0.5),q(0.75),ps.back());
  for(double t:{0.9,0.8,0.7,0.5,0.3,0.2,0.1})
    printf("  P(|ab| < %.1f) = %.6f\n",t,frac(t));
}

int main(){
  std::mt19937_64 g(20260921ULL); // semente fixa: resultado reprodutível
  run(8, 200000, g);    // controle: octônios, deve dar |ab| = 1
  run(16, 1000000, g);  // sedênios
}
