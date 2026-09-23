// ADR009: C++23. As 15 cópias octoniônicas dentro dos sedênios estão na mesma
// órbita? Calcula um invariante por cópia: como os 84 divisores de zero
// primitivos (e_i ± e_j) se distribuem em relação a cada cópia W.
#include <cstdio>
#include <vector>
using V=std::vector<long>;
V cj(const V&x){V r=x;for(size_t i=1;i<r.size();++i)r[i]=-r[i];return r;}
V vadd(const V&x,const V&y){V r(x.size());for(size_t i=0;i<r.size();++i)r[i]=x[i]+y[i];return r;}
V vsub(const V&x,const V&y){V r(x.size());for(size_t i=0;i<r.size();++i)r[i]=x[i]-y[i];return r;}
V cdmul(const V&x,const V&y){size_t n=x.size();if(n==1)return V{x[0]*y[0]};size_t h=n/2;
 V a(x.begin(),x.begin()+h),b(x.begin()+h,x.end()),c(y.begin(),y.begin()+h),d(y.begin()+h,y.end());
 V f=vsub(cdmul(a,c),cdmul(cj(d),b)),s=vadd(cdmul(d,a),cdmul(b,cj(c)));f.insert(f.end(),s.begin(),s.end());return f;}
V e(int k){V v(16,0);v[k]=1;return v;}
int idx[16][16];
bool isZero(const V&v){for(long x:v) if(x) return false; return true;}
int main(){
  for(int i=0;i<16;i++)for(int j=0;j<16;j++){ V p=cdmul(e(i),e(j)); int k=-1;
    for(int t=0;t<16;t++){ if(p[t]) k=t; } idx[i][j]=k; }
  // cópias: subconjuntos fechados de 7 unidades imaginárias
  std::vector<std::vector<int>> W;
  for(int mask=0;mask<(1<<15);++mask){ if(__builtin_popcount(mask)!=7) continue;
    std::vector<int> S; for(int b=0;b<15;b++) if(mask>>b&1) S.push_back(b+1);
    bool ok=true;
    for(size_t a=0;ok&&a<S.size();++a) for(size_t b=0;ok&&b<S.size();++b){ if(a==b) continue;
      int k=idx[S[a]][S[b]]; if(k==0){ok=false;break;} bool in=false; for(int x:S) if(x==k) in=true; if(!in) ok=false; }
    if(ok) W.push_back(S); }
  // divisores de zero primitivos: e_i + s e_j
  struct ZD{int i,j,s;};
  std::vector<ZD> zd;
  for(int i=1;i<=15;i++)for(int j=i+1;j<=15;j++)for(int s:{1,-1}){
    V x(16,0); x[i]=1; x[j]=s; bool has=false;
    for(int p=1;p<=15&&!has;p++)for(int q=p+1;q<=15&&!has;q++)for(int t:{1,-1}){
      V y(16,0); y[p]=1; y[q]=t; if(isZero(cdmul(x,y))){ has=true; break; } }
    if(has) zd.push_back({i,j,s}); }
  printf("copias octonionicas: %zu | divisores de zero primitivos: %zu\n", W.size(), zd.size());
  printf("copia (indices)                        | ZD com 0,1,2 indices na copia\n");
  for(auto&S:W){ int cnt[3]={0,0,0};
    for(auto&z:zd){ int c=0; for(int x:S){ if(x==z.i) c++; if(x==z.j) c++; } cnt[c]++; }
    bool low=true; for(int x:S) if(x>7) low=false;
    printf("%s", low?"* ":"  ");
    for(int x:S) printf("%2d ", x);
    printf("| %3d %3d %3d\n", cnt[0], cnt[1], cnt[2]); }
}
