// ADR009: C++23, inteiros exatos. Censo dos divisores de zero primitivos
// e_i + s e_j (1<=i<j<=15, s=±1, definidos a menos de sinal global).
#include <cstdio>
#include <map>
#include <set>
#include <vector>
using V=std::vector<long>;
V cj(const V&x){V r=x;for(size_t i=1;i<r.size();++i)r[i]=-r[i];return r;}
V vadd(const V&x,const V&y){V r(x.size());for(size_t i=0;i<r.size();++i)r[i]=x[i]+y[i];return r;}
V vsub(const V&x,const V&y){V r(x.size());for(size_t i=0;i<r.size();++i)r[i]=x[i]-y[i];return r;}
V cdmul(const V&x,const V&y){size_t n=x.size();if(n==1)return V{x[0]*y[0]};size_t h=n/2;
 V a(x.begin(),x.begin()+h),b(x.begin()+h,x.end()),c(y.begin(),y.begin()+h),d(y.begin()+h,y.end());
 V f=vsub(cdmul(a,c),cdmul(cj(d),b)),s=vadd(cdmul(d,a),cdmul(b,cj(c)));f.insert(f.end(),s.begin(),s.end());return f;}
bool z(const V&v){for(long x:v)if(x)return false;return true;}
int main(){
  std::vector<V> E; std::vector<int> I,J,S;
  for(int i=1;i<=15;++i)for(int j=i+1;j<=15;++j)for(int s:{1,-1}){V v(16,0);v[i]=1;v[j]=s;E.push_back(v);I.push_back(i);J.push_back(j);S.push_back(s);}
  size_t n=E.size(); long ordered=0, both=0; std::set<std::pair<size_t,size_t>> unord;
  std::map<size_t,int> deg; std::set<std::pair<int,int>> assessors; std::set<int> idxUsed;
  for(size_t a=0;a<n;++a)for(size_t b=0;b<n;++b){
    if(!z(cdmul(E[a],E[b])))continue;
    ++ordered; if(z(cdmul(E[b],E[a])))++both;
    unord.insert({std::min(a,b),std::max(a,b)}); deg[a]++;
    assessors.insert({I[a],J[a]});
  }
  std::map<int,int> degHist; for(auto&[k,d]:deg)degHist[d]++;
  printf("elements (mod global sign): %zu\n",n);
  printf("zero-divisor elements: %zu\n",deg.size());
  printf("ordered pairs xy=0: %ld\n",ordered);
  printf("  of which also yx=0: %ld\n",both);
  printf("unordered pairs: %zu\n",unord.size());
  printf("assessors (index pairs i<j) involved: %zu\n",assessors.size());
  for(auto&[d,c]:degHist)printf("  elements with %d annihilating partners: %d\n",d,c);
  // estrutura das assessoras: i<8<=j ?
  int lowHigh=0; for(auto&[i,j]:assessors) if(i<8 && j>=8) ++lowHigh;
  printf("assessors with i in 1..7 and j in 8..15: %d of %zu\n",lowHigh,assessors.size());
}
