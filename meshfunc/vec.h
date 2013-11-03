#ifndef _VEC_H
#define _VEC_H
#include<cassert>
#include<cmath>
#include<iostream>


template<unsigned int N,class T>
struct Vec
{
    T v[N];
    Vec<N,T>()
    {}

    explicit Vec<N,T>(T value)
    {
        for(int i=0;i<N;i++)
            v[i]=value;
    }

    template<class S>
    explicit Vec<N,T>(const S *source)
    {
        for(int i=0;i<N;i++)
            v[i]=(T)cource[i];
    }

    template<class S>
    explicit Vec<N,T>(const Vec<N,S>& source)
    {
        for(int i=0;i<N;i++)
            v[i]=(T)source[i];
    }


    Vec<N,T>(T v0, T v1)
    {
        assert(N==2);
        v[0]=v0;v[1]=v1;
    }
    Vec<N,T>(T v0, T v1, T v2)
    {
        assert(N==3);
        v[0]=v0;v[1]=v1;v[2]=v2;
    }

    Vec<N,T>(T v0, T v1, T v2, T v3)
    {
        assert(N==4);
        v[0]=v0;v[1]=v1;v[2]=v2;v[3]=v3;
    }

    Vec<N,T>(T v0, T v1, T v2, T v3, T v4, T v5)
    {
        assert(N==6);
        v[0]=v0;v[1]=v1;v[2]=v2;v[3]=v3;v[4]=v4;v[5]=v5;
    }

    T &operator[](int index)
    {
        assert(0<=index && (unsigned int)index<N);
        return v[index];
    }

    const T &operator[](int index) const
    {
        assert(0<=index && (unsigned int)index<N);
        return v[index];
    }

    bool nonzero(void) const
    {
        for(int i=0;i<N;i++)if(v[i])return true;
        return false;
    }

    Vec<N,T> operator+=(Vec<N,T> &w)
    {
        for(int i=0;i<N;i++) v[i]+=w[i];
        return *this;
    }

    Vec<N,T> operator+(Vec<N,T> &w) const
    {
        Vec<N,T> sum;
        for(int i=0;i<N;i++)
            sum[i]=v[i]+w[i];
        return sum;
    }

    Vec<N,T> operator-=(Vec<N,T> &w)
    {
        for(int i=0;i<N;i++) v[i]-=w[i];
        return *this;
    }

    Vec<N,T> operator-(void) const
    {
        Vec<N,T> negtive;
        for(int i=0;i<N;i++)
            negtive[i]=-v[i];
        return negtive;
    }

    Vec<N,T> operator-(Vec<N,T> &w) const
    {
        Vec<N,T> sum;
        for(int i=0;i<N;i++)
            sum[i]=v[i]-w[i];
        return sum;
    }

    Vec<N,T> operator*=(T a)
    {
        for(int i=0;i<N;i++)
            v[i]*=a;
        return *this;
    }

    Vec<N,T> operator*(const T a)const
    {
        Vec<N,T> w(*this);
        w*=a;
        return w;
    }

    Vec<N,T> operator*=(const Vec<N,T> &w)
    {
        for(unsigned int i=0; i<N; ++i) v[i]*=w.v[i];
        return *this;
    }

    Vec<N,T> operator*(const Vec<N,T> &w) const
    {
        Vec<N,T> componentwise_product;
        for(unsigned int i=0; i<N; ++i) componentwise_product[i]=v[i]*w.v[i];
        return componentwise_product;
    }

    Vec<N,T> operator/=(T a)
    {
        for(unsigned int i=0; i<N; ++i) v[i]/=a;
        return *this;
    }

    Vec<N,T> operator/(T a) const
    {
        Vec<N,T> w(*this);
        w/=a;
        return w;
    }
};


typedef Vec<2,double>			Vec2d;
typedef Vec<2,float>			Vec2f;
typedef Vec<2,int>				Vec2i;
typedef Vec<2,unsigned int>		Vec2ui;

typedef Vec<3,double>			Vec3d;
typedef Vec<3,float>			Vec3f;
typedef Vec<3,int>				Vec3i;
typedef Vec<3,unsigned int>		Vec3ui;

typedef Vec<4,double>			Vec4d;
typedef Vec<4,float>			Vec4f;
typedef Vec<4,int>				Vec4i;
typedef Vec<4,unsigned int>		Vec4ui;

typedef Vec<6,double>           Vec6d;

template<class T>
inline T x2(const T &a)
{
    return a*a;
}

template<unsigned int N, class T>
inline T mod(const Vec<N,T> &a)
{
    T l=0;
    for(int i=0;i<N;i++)
        l+=a.v[i]*a.v[i];
    return sqrt(l);
}

template<unsigned int N, class T>
inline T dist(const Vec<N,T>&a,const Vec<N,T> &b)
{
    T l=0;
    for(int i=0;i<N;i++)
        l+=(a.v[i]-b.v[i])*(a.v[i]-b.v[i]);
    return sqrt(l);
}

template<unsigned int N,class T>
inline void normalize(Vec<N,T> &a)
{
    T m=mod(a);
    for(int i=0;i<N;i++)
        a[i]/=m;
}

template<unsigned int N,class T>
inline Vec<N,T> normalized(Vec<N,T> &a)
{
    return a/mod(a);
}

template<unsigned int N,class T>
inline void zero(Vec<N,T> &a)
{
    for(int i=0;i<N;i++)
        a[i]=0;
}

template<unsigned int N,class T>
std::ostream &operator<<(std::ostream &out,Vec<N,T> &a)
{
    out<<a[0];
    for(int i=1;i<N;i++)
        out<<" "<<a[i];
    cout<<endl;
    return out;
}

template<unsigned int N,class T>
std::istream &operator>>(std::istream &in,Vec<N,T> &a)
{
    for(int i=0;i<N;i++)
        in>>a[i];
    return in;
}

template<unsigned int N,class T>
inline bool operator==(Vec<N,T> &a, Vec<N,T> &b)
{
    for(int i=0;i<N;i++)
    {
        if(a[i]!=b[i])return false;
    }
    return true;
}

template<unsigned int N,class T>
inline bool operator<(Vec<N,T> &a,Vec<N,T> &b)
{
    for(int i=0;i<N;i++)
        if(a[i]>=b[i])return false;
    return true;
}

template<unsigned int N,class T>
inline bool operator>(Vec<N,T> &a,Vec<N,T> &b)
{
    for(int i=0;i<N;i++)
        if(a[i]<=b[i])return false;
    return true;
}

template<unsigned int N,class T>
inline bool operator<=(Vec<N,T> &a,Vec<N,T> &b)
{
    for(int i=0;i<N;i++)
        if(a[i]>b[i])return false;
    return true;
}

template<unsigned int N,class T>
inline bool operator>=(Vec<N,T> &a,Vec<N,T> &b)
{
    for(int i=0;i<N;i++)
        if(a[i]<b[i])return false;
    return true;
}

template<unsigned int N,class T>
inline bool operator!=(const Vec<N,T> &a,const Vec<N,T> &b)
{
    for(int i=0;i<N;i++)
    {
        if(a[i]!=b[i])return true;
    }
    return false;
}

template<unsigned int N, class T>
inline Vec<N,T> operator*(T a, const Vec<N,T> &v)
{
    Vec<N,T> w(v);
    w*=a;
    return w;
}


template<unsigned int N,class T>
inline T dot(Vec<N,T> &a,Vec<N,T> &b)
{
    T d=0;
    for(int i=0;i<N;i++)
        d+=a[i]*b[i];
    return d;
}

template<class T>
inline Vec<2,T> vec(Vec<2,T>&a, Vec<2,T>& b)
{
    return b-a;
}

template<class T>
inline Vec<3,T> vec(Vec<3,T>&a, Vec<3,T>& b)
{
    return b-a;
}

template<class T>
inline Vec<2,T> rotate( Vec<2,T>& a, T& angle)
{
    T c=cos(angle);
    T s=sin(angle);
    return Vec<2,T>(c*a[0]-s*a[1],s*a[0]+c*a[1]);
}

// Rotate the point (x,y,z) around the vector (u,v,w)
template<class T>
inline Vec<3,T> rotate( Vec<3,T>& x,  T& angle,  Vec<3,T>& u )
{
    T ux = u[0]*x[0];
    T uy = u[0]*x[1];
    T uz = u[0]*x[2];
    T vx = u[1]*x[0];
    T vy = u[1]*x[1];
    T vz = u[1]*x[2];
    T wx = u[2]*x[0];
    T wy = u[2]*x[1];
    T wz = u[2]*x[2];

    T sa = (T) sin(angle);
    T ca = (T) cos(angle);

    return Vec<3,T> ( u[0] * (ux+vy+wz) + (x[0]*(u[1]*u[1]+u[2]*u[2])-u[0]*(vy+wz))*ca+(-wy+vz)*sa,
        u[1] * (ux+vy+wz) + (x[1]*(u[0]*u[0]+u[2]*u[2])-u[1]*(ux+wz))*ca+(wx-uz)*sa,
        u[2] * (ux+vy+wz) + (x[2]*(u[0]*u[0]+u[1]*u[1])-u[2]*(ux+vy))*ca+(-vx+uy)*sa );

}

template<class T>
inline T cross(Vec<2,T> &a, Vec<2,T> &b)
{
    return a[0]*b[1]-a[1]*b[0];
}

template<class T>
inline Vec<3,T> cross(Vec<3,T> &a, Vec<3,T> &b)
{
    return Vec<3,T>(a[1]*b[2]-a[2]*b[1],a[2]*b[0]-a[0]*b[2],a[0]*b[1]-a[1]*b[0]);
}

template<unsigned int N, class T>
inline void assign(const Vec<N,T> &a, T &a0, T &a1)
{
    assert(N==2);
    a0=a.v[0]; a1=a.v[1];
}

template<unsigned int N, class T>
inline void assign(const Vec<N,T> &a, T &a0, T &a1, T &a2)
{
    assert(N==3);
    a0=a.v[0]; a1=a.v[1]; a2=a.v[2];
}

template<unsigned int N,class T>
inline Vec<N,T> fabs(const Vec<N,T> &a)
{
    Vec<N,T> b;
    for(int i=0;i<N;i++)
        b[i]=(T)fabs(a[i]);
    return b;
}

template<unsigned int N,class T>
void translate(Vec<N,Vec<N,T>> &m)
{
    Vec<N,Vec<N,T>> v;
    for(size_t i=0;i<N;i++)
        for(size_t j=0;j<N;j++)
            v[i][j]=m[j][i];
    for(size_t i=0;i<N;i++)
        for(size_t j=0;j<N;j++)
            m[i][j]=v[i][j];
}
#endif
