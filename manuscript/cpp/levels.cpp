// ADR009: C++23, inteiros exatos. Dois testes sobre a divisão em "dois níveis"
// dos sedênios: (1) quantas subálgebras de dimensão 8 geradas por elementos da
// base são isomorfas aos octônios; (2) automorfismos monomiais (permutação da
// base com sinais) e se algum mistura as metades 1..7 e 8..15.
// Convenção idêntica a cd_bloch.cpp.
#include <array>
#include <cstdio>
#include <set>
#include <vector>
using V=std::vector<long>;
V cj(const V&x){V r=x;for(size_t i=1;i<r.size();++i)r[i]=-r[i];return r;}
V vadd(const V&x,const V&y){V r(x.size());for(size_t i=0;i<r.size();++i)r[i]=x[i]+y[i];return r;}
V vsub(const V&x,const V&y){V r(x.size());for(size_t i=0;i<r.size();++i)r[i]=x[i]-y[i];return r;}
V cdmul(const V&x,const V&y){size_t n=x.size();if(n==1)return V{x[0]*y[0]};size_t h=n/2;
 V a(x.begin(),x.begin()+h),b(x.begin()+h,x.end()),c(y.begin(),y.begin()+h),d(y.begin()+h,y.end());
 V f=vsub(cdmul(a,c),cdmul(cj(d),b)),s=vadd(cdmul(d,a),cdmul(b,cj(c)));f.insert(f.end(),s.begin(),s.end());return f;}
V e(int k){V v(16,0);v[k]=1;return v;}

int sgn[16][16]; int idx[16][16];

int main(){
  for(int i=0;i<16;i++)for(int j=0;j<16;j++){ V p=cdmul(e(i),e(j)); int k=-1,s=0;
    for(int t=0;t<16;t++){ if(p[t]){k=t;s=(int)p[t];} }
    idx[i][j]=k;
    sgn[i][j]=s; }

  // (1) subconjuntos {0} ∪ 7 unidades imaginárias fechados sob multiplicação (a menos de sinal)
  std::vector<std::vector<int>> closed;
  // busca: percorre subconjuntos de tamanho 7 de {1..15} fechados
  for(int mask=0; mask<(1<<15); ++mask){
    int pc=__builtin_popcount(mask); if(pc!=7) continue;
    std::vector<int> S; for(int b=0;b<15;b++) if(mask>>b&1) S.push_back(b+1);
    bool ok=true;
    for(size_t a=0; ok&&a<S.size(); ++a) for(size_t b=0; ok&&b<S.size(); ++b){
      if(a==b) continue;
      int k=idx[S[a]][S[b]];
      if(k==0) { ok=false; break; }            // produto real fora do conjunto imaginário
      bool in=false; for(int x:S) if(x==k) in=true; if(!in) ok=false; }
    if(ok) closed.push_back(S);
  }
  int lowHalf=0, mixed=0, upperOnly=0;
  for(auto&S:closed){ bool low=true, up=true; for(int x:S){ if(x>7) low=false; if(x<8) up=false; }
    if(low) lowHalf++; else if(up) upperOnly++; else mixed++; }
  printf("(1) subconjuntos fechados de 7 unidades imaginarias: %zu\n", closed.size());
  printf("    so indices 1..7: %d | so 8..15: %d | mistos: %d\n", lowHalf, upperOnly, mixed);

  // (2) automorfismos monomiais: imagens dos geradores e1,e2,e4,e8
  int gens[4]={1,2,4,8};
  long total=0, mixing=0;
  for(int a=1;a<16;a++)for(int sa:{1,-1})
  for(int b=1;b<16;b++)for(int sb:{1,-1})
  for(int c=1;c<16;c++)for(int sc:{1,-1})
  for(int d=1;d<16;d++)for(int sd:{1,-1}){
    int im[16]; int sg[16]; for(int i=0;i<16;i++){im[i]=-1;sg[i]=0;}
    im[0]=0; sg[0]=1;
    int gi[4]={a,b,c,d}, gs[4]={sa,sb,sc,sd};
    bool ok=true;
    for(int t=0;t<4;t++){ int g=gens[t]; if(im[g]!=-1){ if(im[g]!=gi[t]||sg[g]!=gs[t]) ok=false; } im[g]=gi[t]; sg[g]=gs[t]; }
    if(!ok) continue;
    // fecho: propaga phi(e_i e_j) = phi(e_i) phi(e_j)
    for(int iter=0; iter<6 && ok; ++iter)
      for(int i=0;i<16&&ok;i++)for(int j=0;j<16&&ok;j++){
        if(im[i]<0||im[j]<0) continue;
        int k=idx[i][j];
        int s=sgn[i][j];
        int ri=im[i], rj=im[j]; int rk=idx[ri][rj]; int rs=sgn[ri][rj]*sg[i]*sg[j];
        // phi(e_i e_j) = s * phi(e_k)  =>  phi(e_k) = s * (phi(e_i)phi(e_j))
        int want_k=rk, want_s=rs*s;
        if(im[k]<0){ im[k]=want_k; sg[k]=want_s; }
        else if(im[k]!=want_k || sg[k]!=want_s) ok=false;
      }
    if(!ok) continue;
    // bijetividade e verificação completa
    std::set<int> used; for(int i=0;i<16;i++){ if(im[i]<0){ok=false;break;} used.insert(im[i]); }
    if(!ok||used.size()!=16) continue;
    for(int i=0;i<16&&ok;i++)for(int j=0;j<16&&ok;j++){
      int k=idx[i][j], s=sgn[i][j];
      int rk=idx[im[i]][im[j]], rs=sgn[im[i]][im[j]]*sg[i]*sg[j];
      if(rk!=im[k] || rs!=s*sg[k]) ok=false; }
    if(!ok) continue;
    ++total;
    bool mix=false; for(int i=1;i<=7;i++) if(im[i]>7) mix=true;
    if(mix) ++mixing;
  }
  printf("(2) automorfismos monomiais: %ld | dos quais levam algum e_i (1..7) para a metade 8..15: %ld\n", total, mixing);
}
