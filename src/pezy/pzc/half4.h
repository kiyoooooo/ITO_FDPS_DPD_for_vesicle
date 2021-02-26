#ifndef H_HALF4
#define H_HALF4
#define f_ld(V,P,OFFSET) asm volatile ("f.ld %0 %1 %2":"=w"(V):"r"(P),"i"(OFFSET))
#define f_eld(V,P,OFFSET) asm volatile ("f.eld %0 %1 %2":"=w"(V):"r"(P),"i"(OFFSET))
#define f_add(D,A,B) asm volatile ("f.add %0 %1 %2":"=w"(D):"w"(A),"w"(B))
#define f_sub(D,A,B) asm volatile ("f.sub %0 %1 %2":"=w"(D):"w"(A),"w"(B))
#define f_mul(D,A,B) asm volatile ("f.mul %0 %1 %2":"=w"(D):"w"(A),"w"(B))
#define f_mad(D,A,B,C) asm volatile ("f.mad %0 %1 %2 %3":"=w"(D):"w"(A),"w"(B),"w"(C))
#define f_msub(D,A,B,C) asm volatile ("f.msub %0 %1 %2 %3":"=w"(D):"w"(A),"w"(B),"w"(C))
#define f_nmad(D,A,B,C) asm volatile ("f.nmad%0 %1 %2 %3":"=w"(D):"w"(A),"w"(B),"w"(C))
#define f_rsqrt(D,A) asm volatile ("f.rsqrt %0 %1":"=w"(D):"w"(A))

#define d_add(D,A,B) asm volatile ("d.add %0 %1 %2":"=w"(D):"w"(A),"w"(B))
#define d_sub(D,A,B) asm volatile ("d.sub %0 %1 %2":"=w"(D):"w"(A),"w"(B))
#define d_mul(D,A,B) asm volatile ("d.mul %0 %1 %2":"=w"(D):"w"(A),"w"(B))
#define d_ldd(V,P,OFFSET) asm volatile ("d.ldd %0 %1 %2":"=w"(V):"r"(P),"i"(OFFSET))
#define d_eldd(V,P,OFFSET) asm volatile ("d.eldd %0 %1 %2":"=w"(V):"r"(P),"i"(OFFSET))
#define d_eldd_ct(V,P,OFFSET) asm volatile ("d.eldd.ct %0 %1 %2":"=w"(V):"r"(P),"i"(OFFSET))
#define d_sd(A,I,B) asm volatile ("d.sd %0 %1 %2"::"r"(A),"i"(I),"w"(B))
#define d_ftod(D,A) asm volatile ("d.ftod %0 %1":"=w"(D):"w"(A))
#define nop() asm volatile ("c.nop")

#define v2f_add(D,A,B) asm volatile ("v2f.add %0 %1 %2":"=w"(D):"w"(A),"w"(B))
#define v2f_sub(D,A,B) asm volatile ("v2f.sub %0 %1 %2":"=w"(D):"w"(A),"w"(B))
#define v2f_mul(D,A,B) asm volatile ("v2f.mul %0 %1 %2":"=w"(D):"w"(A),"w"(B))
#define v2f_mad(D,A,B,C) asm volatile ("v2f.mad %0 %1 %2 %3":"=w"(D):"w"(A),"w"(B),"w"(C))
#define v2f_rsqrt(D,A) asm volatile ("v2f.rsqrt %0 %1":"=w"(D):"w"(A))
#define v2f_swap(D,A) asm volatile ("v4h.rot %0 %1 2":"=w"(D):"w"(A))
#define v2f_packlo(D,A,B) asm volatile("v2f.pack.lo %0 %1 %2":"=w"(D):"w"(A),"w"(B))
#define v2f_packhi(D,A,B) asm volatile("v2f.pack.hi %0 %1 %2":"=w"(D):"w"(A),"w"(B))
#define v2f_mov(D,A,B) asm volatile ("v2f.mov %0 %1 %2":"=w"(D):"w"(A),"w"(B))

#define v4h_add(D,A,B) asm volatile("v4h.add %0 %1 %2":"=w"(D):"w"(A),"w"(B))
#define v4h_sub(D,A,B) asm volatile("v4h.sub %0 %1 %2":"=w"(D):"w"(A),"w"(B))
#define v4h_mul(D,A,B) asm volatile("v4h.mul %0 %1 %2":"=w"(D):"w"(A),"w"(B))
#define v4h_mad(D,A,B,C) asm volatile("v4h.mad %0 %1 %2":"=w"(D):"w"(A),"w"(B),"w"(C))
#define v4h_ftoh(D,A) asm volatile("v4h.ftoh %0 %1":"=w"(D):"w"(A))
#define v4h_rot(D,A,I) asm volatile("v4h.rot %0 %1 %2":"=w"(D):"w"(A),"i"(I))
#define v4h_packlo(D,A,B) asm volatile("v4h.pack.lo %0 %1 %2":"=w"(D):"w"(A),"w"(B))
#define v4h_packhi(D,A,B) asm volatile("v4h.pack.hi %0 %1 %2":"=w"(D):"w"(A),"w"(B))
#define v4h_extractlo(D,A)  asm volatile("v4h.extract.lo %0 %1":"=w"(D):"w"(A))
#define v4h_extracthi(D,A)  asm volatile("v4h.extract.hi %0 %1":"=w"(D):"w"(A))

#define h_ftoh(D,A) asm volatile("h.ftoh.h %0 %1":"=w"(D):"w"(A))
#define h_unpackloh(D,A) asm volatile("h.unpackloh.s %0 %1":"=w"(D):"w"(A))
//#define h_unpackloh(D,A) asm volatile("v4h.unpack.lo %0 %1":"=w"(D):"w"(A))
#define h_unpackhih(D,A) asm volatile("h.unpackhih.s %0 %1":"=w"(D):"w"(A))


__attribute__ ((always_inline))
inline float sum(const float2& v){
#if 1
  return v.x + v.y;
#else
  asm volatile ("// sum");
  union{
    float2 f2;
    float  f;
  }x,y;
  x.f2 = v;
  v2f_swap(y.f2,x.f2);
  return x.f + y.f;
#endif
}

__attribute__ ((always_inline))
inline float2 swap(const float2& v){
  float2 ret;
  v4h_rot(ret,v,2);
  return ret;
}

class half4{
 public:
  union{
    double d;
    float2 f2;
    float  f;
    __fp16 h;
  };

  half4(){}
  half4(float v){
    *this = v;
  }
  half4(float2 v){
    *this = v;
  }
  half4(float2 v0,float2 v1){
    half4 tmp0,tmp1;
    v4h_ftoh(tmp0.d,v0);
    v4h_ftoh(tmp1.d,v1);
    v4h_packlo(d,tmp0.d,tmp1.d);
  }

  half4 operator+(const half4& v) const {
    half4 ret;
    v4h_add(ret.d,d,v.d);
    return ret;
  }
  half4 operator-(const half4& v) const {
    half4 ret;
    v4h_sub(ret.d,d,v.d);
    return ret;
  }
  half4 operator*(const half4& v) const {
    half4 ret;
    v4h_mul(ret.d,d,v.d);
    return ret;
  }
  half4 operator=(const half4& v){
    d = v.d;
    return *this;
  }

  half4 operator=(const float& v){
    float2 tmp;
    v2f_mov(tmp,v,v);
    v4h_ftoh(d,tmp);
    v4h_packlo(d,d,d);
    return *this;
  }
  half4 operator=(const float2& v){
    v4h_ftoh(d,v);
    v4h_packlo(d,d,d);
    return *this;
  }

  half4 xyzw(){
    return *this;
  }
  half4 yzwx(){
    half4 ret;
    v4h_rot(ret.d,d,1);
    return ret;
  }
  half4 zwxy(){
    half4 ret;
    v4h_rot(ret.d,d,2);
    return ret;
  }
  half4 wxyz(){
    half4 ret;
    v4h_rot(ret.d,d,3);
    return ret;
  }

  float x(){
    float ret;
    h_unpackloh(ret,f);
    return ret;
  }
  float y(){
    float ret;
    h_unpackhih(ret,f);
    return ret;
  }
  float z(){
    float ret;
    h_unpackloh(ret,swap(f2).x);
    return ret;
  }
  float w(){
    float ret;
    h_unpackhih(ret,swap(f2).x);
    return ret;
  }

  float2 unpacklo(){
    float2 ret;
    union{
      float2 f2;
      float  f;
    } tmp;
    v2f_swap(tmp.f2,d);
    h_unpackloh(ret.x,tmp.f);
    v2f_swap(ret,ret);
    h_unpackloh(ret.x,f);
    return ret;
  }
  float2 unpackhi(){
    float2 ret;
    union{
      float2 f2;
      float  f;
    } tmp;
    v2f_swap(tmp.f2,d);
    h_unpackhih(ret.x,tmp.f);
    v2f_swap(ret,ret);
    h_unpackhih(ret.x,f);
    return ret;
  }
};

inline half4 make_half4(const float2& a,const float2& b){
  half4 ret;
  half4 hi,lo;
  v4h_ftoh(hi.d,a);
  v4h_ftoh(lo.d,b);
  v4h_packlo(ret.d,hi.d,lo.d);
  return ret;
}
inline half4 make_half4(const float2& v){
  return make_half4(v,v);
}
inline half4 make_half4(const float& a,const float& b,const float& c,const float& d){
  half4 ret;
  return ret;
}
inline half4 make_half4(const float& v){
  return make_half4(v,v,v,v);
}

inline float sum(const half4& v){
#if 1
  half4 tmp;
  float v0,v1,v2,v3;
  h_unpackhih(v0,v.f);
  h_unpackloh(v1,v.f);
  v2f_swap(tmp.d,v.d);
  h_unpackhih(v2,tmp.f);
  h_unpackloh(v3,tmp.f);
  return v0+v1+v2+v3;
#else
  float sum = (float)v.h;
  half4 tmp;
  v4h_rot(tmp.d,v.d,1);
  sum += (float)tmp.h;
  v4h_rot(tmp.d,v.d,2);
  sum += (float)tmp.h;
  v4h_rot(tmp.d,v.d,3);
  sum += (float)tmp.h;
  return sum;
#endif
}

inline float2 sum2(const half4& v){
  half4 tmp;
  v2f_swap(tmp.d,v.d);
  v4h_add(tmp.d,tmp.d,v.d);
  return tmp.unpacklo();
}
inline half4 swap2(const half4& v){
  half4 ret;
  v2f_swap(ret.d,v.d);
  return ret;
}

typedef half4 half;

#endif
