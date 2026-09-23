// zd_sparse.cpp — ADR009: C++23, aritmética inteira exata.
// Conta, entre elementos da forma e_i + s e_j (1 <= i < j <= 15, s = ±1),
// quantos pares ordenados (x, y) têm produto sedeniônico exatamente nulo.
// Mesma convenção de Cayley–Dickson de cd_bloch.cpp.
#include <cstdio>
#include <vector>

using V = std::vector<long>;
V cj(const V& x){ V r=x; for(size_t i=1;i<r.size();++i) r[i]=-r[i]; return r; }
V vadd(const V&x,const V&y){V r(x.size());for(size_t i=0;i<r.size();++i)r[i]=x[i]+y[i];return r;}
V vsub(const V&x,const V&y){V r(x.size());for(size_t i=0;i<r.size();++i)r[i]=x[i]-y[i];return r;}
V cdmul(const V& x,const V& y){
  size_t n=x.size(); if(n==1) return V{x[0]*y[0]}; size_t h=n/2;
  V a(x.begin(),x.begin()+h), b(x.begin()+h,x.end()), c(y.begin(),y.begin()+h), d(y.begin()+h,y.end());
  V f=vsub(cdmul(a,c),cdmul(cj(d),b)), s=vadd(cdmul(d,a),cdmul(b,cj(c)));
  f.insert(f.end(),s.begin(),s.end()); return f; }
bool isZero(const V& v){ for(long x:v) if(x) return false; return true; }

int main(){
  std::vector<V> elems;
  for(int i=1;i<=15;++i) for(int j=i+1;j<=15;++j) for(int s:{1,-1}){
    V v(16,0); v[i]=1; v[j]=s; elems.push_back(v);
  }
  long pairs=0, zeros=0, elemsWithPartner=0;
  for(const auto& x:elems){
    bool has=false;
    for(const auto& y:elems){ ++pairs; if(isZero(cdmul(x,y))){ ++zeros; has=true; } }
    if(has) ++elemsWithPartner;
  }
  // Checagem do par do manuscrito.
  V a(16,0); a[1]=1; a[10]=1; V b(16,0); b[4]=1; b[15]=-1;
  printf("check (e1+e10)(e4-e15) == 0: %s\n", isZero(cdmul(a,b)) ? "yes" : "NO");
  printf("elements: %zu\n", elems.size());
  printf("ordered pairs: %ld\n", pairs);
  printf("pairs with product exactly 0: %ld\n", zeros);
  printf("fraction: %.6f\n", static_cast<double>(zeros)/pairs);
  printf("elements with at least one zero-divisor partner: %ld of %zu\n", elemsWithPartner, elems.size());
}
