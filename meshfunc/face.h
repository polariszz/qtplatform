#ifndef _FACE_
#define _FACE_
#include"vec.h"
#include<vector>
#include<algorithm>
using namespace std;
#define  PB(a) pindex.push_back((a))

class TriFace
{
public:
    TriFace(){}
    TriFace(int a,int b,int c){
        if(a>b)swap(a,b);
        if(a>c)swap(a,c);
        if(b>c)swap(b,c);
        PB(a);PB(b);PB(c);}
    TriFace(Vec3i v){TriFace(v[0],v[1],v[2]);}
    int operator[](int i){return pindex[i];}
    std::vector<int> edges;
    std::vector<int> pindex;
    friend bool operator<(const TriFace&,const TriFace&);
};

#endif
