#ifndef _SMOOTH_
#define _SMOOTH_
#include"vec.h"
#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<fstream>
#include<math.h>
#include<assert.h>
#include"Edge.h"
#include"face.h"
using namespace std;
#define TOLERANCE 1e-9
#define FOREACH(i,n) for(int (i)=0;(i)<(int)(n);(i)++)
#define forit(it,con) for(auto it=(con).begin();it!=(con).end();it++)


class pt
{
public:
    pt(double xx=0,double yy=0,double zz=0):v(xx,yy,zz){};
    pt(Vec3d vv):v(vv){};
    Vec3d v;
    double operator[](int index)
    {
        return this->v[index];
    }
    const double operator[](int index) const
    {
        return this->v[index];
    }
    bool is(const pt& other){
        inline bool iszero(double a);
        return iszero(this->v[0]-other[0])&&iszero(this->v[1]-other[1])&&iszero(this->v[2]-other[2]);
    }
    bool operator==(const pt& other){
        return this->is(other);
    }
};
//bool operator<(const pt& lhs,const pt& rhs);

class point: public pt
{
public:
    point(double xx=0,double yy=0,double zz=0):pt(xx,yy,zz)
    {
        disp=Vec3d(0,0,0);
        isside=0;
        numeleofnode=0;
        numofneighbor=0;
    }
    point& operator=(const point& other);
    Vec3d disp;
    int numeleofnode;
    int numofneighbor;
    int eleofnode[100];
    int neighbor[100];
    point *AdjentPoints;
    int isside;

};


ostream &operator<<(ostream& out,pt &a);

istream &operator>>(istream& in,pt &a);


inline double distancebyInt(int i,int j);
inline double distanceofPoints(point a,point b);
inline bool iszero(double a){return fabs(a)<TOLERANCE;}
double multiply(const pt& a,const pt& b,const pt& c);

inline int GetIndexInArray(int v,int tn[],int n);
inline int contains(int a[],int num,int b);

void init(char* meshfile,char* ptfile);//d
void init2();
void buildBoundary();
void buildBoundaryEdge();
bool EdgePlaneCross(int bdEdgeIndex,Vec4d plane);
pt EdgeSplit(int bdEdgeIndex,Vec4d v);
double computeQuality(int i);
void planeCross(Vec3d n,Vec3d v);

void LaplacianSmoothOnePoint(int va);
void PointBasedSmooth(void fun(int),int K);

void EleSmoothbySMEC(int va); //Smoothing Method by Expand and Contract
void EleBasedSmooth(void fun(int),int K);

void outputVTK();
void WriteDXF(string filename);
void LineDXF(string filename);
void DrawLine(ofstream &outfile,point &start,point &end);
void outputQualityDistribution(int i);
void ClearMemory();

class Cell
{
public:
    Vec3d p[4];
};

extern point *points;
extern Vec4i *PointVertexofEle;
extern int numvert;
extern int numele;
extern Cell *Elements;
extern string filename;
extern string datapath;
extern int BdPtNum;
extern int SplitPointNum;
extern vector<vector<pt>> SplitPoints;
extern vector<vector<pt>> Split3dPoints;


#endif //_SMOOTH_
