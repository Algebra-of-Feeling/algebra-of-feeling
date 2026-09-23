// ADR009: C++23. Verificação da proposição: nos sedênios (A_3 ⊕ A_3), nenhum
// elemento com uma das metades nula é divisor de zero.
// Teste 1: exaustivo sobre pares de elementos da base com coeficientes ±1 em
// duas posições (os 210 elementos usados no censo), restritos a uma só metade.
// Teste 2: amostragem aleatória de elementos com metade nula, contra parceiros
// aleatórios, verificando que |xy| nunca se anula (e nem se aproxima de zero).
#include <cmath>
#include <cstdio>
#include <random>
#include <vector>
using Vd=std::vector<double>;
Vd cj(const Vd&x){Vd r=x;for(size_t i=1;i<r.size();++i)r[i]=-r[i];return r;}
Vd vadd(const Vd&x,const Vd&y){Vd r(x.size());for(size_t i=0;i<r.size();++i)r[i]=x[i]+y[i];return r;}
Vd vsub(const Vd&x,const Vd&y){Vd r(x.size());for(size_t i=0;i<r.size();++i)r[i]=x[i]-y[i];return r;}
Vd cdmul(const Vd&x,const Vd&y){size_t n=x.size();if(n==1)return Vd{x[0]*y[0]};size_t h=n/2;
 Vd a(x.begin(),x.begin()+h),b(x.begin()+h,x.end()),c(y.begin(),y.begin()+h),d(y.begin()+h,y.end());
 Vd f=vsub(cdmul(a,c),cdmul(cj(d),b)),s=vadd(cdmul(d,a),cdmul(b,cj(c)));f.insert(f.end(),s.begin(),s.end());return f;}
double nrm(const Vd&v){double s=0;for(double t:v)s+=t*t;return std::sqrt(s);}
int main(){
  // Teste 1: exaustivo com coeficientes inteiros pequenos em uma metade
  long pairs=0; double worst=1e9;
  std::vector<Vd> halfVecs;
  for(int i=0;i<8;i++)for(int j=i+1;j<8;j++)for(int s:{1,-1}){ Vd v(16,0.0); v[i]=1; v[j]=s; halfVecs.push_back(v); }   // metade de baixo
  for(int i=8;i<16;i++)for(int j=i+1;j<16;j++)for(int s:{1,-1}){ Vd v(16,0.0); v[i]=1; v[j]=s; halfVecs.push_back(v); } // metade de cima
  std::vector<Vd> all;
  for(int i=1;i<16;i++)for(int j=i+1;j<16;j++)for(int s:{1,-1}){ Vd v(16,0.0); v[i]=1; v[j]=s; all.push_back(v); }
  for(const auto&x:halfVecs) for(const auto&y:all){ ++pairs; double n=nrm(cdmul(x,y))/(nrm(x)*nrm(y)); if(n<worst) worst=n; }
  printf("Teste 1 (exaustivo): %ld pares com um fator numa so metade; menor |xy|/(|x||y|) = %.6f\n", pairs, worst);
  // Teste 2: aleatorio
  std::mt19937_64 g(20260922ULL); std::normal_distribution<double> N(0.0,1.0);
  double worst2=1e9; long n2=0;
  for(int k=0;k<200000;k++){
    Vd x(16,0.0), y(16,0.0);
    bool lower = (k%2==0);
    for(int i=0;i<8;i++) x[lower? i : i+8]=N(g);
    for(int i=0;i<16;i++) y[i]=N(g);
    double d=nrm(x), e2=nrm(y); if(d<1e-12||e2<1e-12) continue;
    double n=nrm(cdmul(x,y))/(d*e2); if(n<worst2) worst2=n; ++n2;
  }
  printf("Teste 2 (aleatorio): %ld pares; menor |xy|/(|x||y|) = %.6f\n", n2, worst2);
  printf("(um divisor de zero exigiria 0.000000)\n");
}
