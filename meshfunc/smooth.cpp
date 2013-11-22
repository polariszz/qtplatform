#pragma warning(disable : 4018)
#pragma warning(disable : 4996)
#include "smooth.h"
#include<algorithm>
#include<queue>
#include<list>
#include<set>
#define PI 3.1415926

point *points;
Vec4i *PointVertexofEle;
int numvert;
int numele;
Cell *Elements;
double *qualityofele;
string filename;
string datapath="../DATA/";
vector<BdEdge> BoundaryEdges;
vector<TriFace> faces;
map<int,vector<int>> ptfacemap;
map<int,vector<int>> ptedgemap;
int BdPtNum;
int BdEdgeNum=0;
int BdFaceNum=0;
int SplitPointNum=0;
//pt *SplitPoints=NULL;
vector<vector<pt>> SplitPoints;
vector<vector<pt>> Split3dPoints;

/*
 *	int mesh
 */

void init(char* meshfile,char* ptfile)
{
    ifstream ptin(ptfile);
    if(!ptin)
        cout<<"open ptfile error!";
    char c;
    ptin>>numvert>>c;
    points=new point[numvert];

    for(int i=0;i<numvert;i++)
    {
        ptin>>points[i].v;
    }
    ptin.close();

    ifstream meshin(meshfile);
    if(!meshin) cout<<"open meshfile error"<<endl;
    meshin>>numele>>c;

    PointVertexofEle=new Vec4i[numele];
    Elements=new Cell[numele];
    for(int i=0;i<numele;i++)
    {
        char c;
        meshin>>PointVertexofEle[i][0]>>c;
        meshin>>PointVertexofEle[i][1]>>c;
        meshin>>PointVertexofEle[i][2]>>c;
        meshin>>PointVertexofEle[i][3]>>c;

        PointVertexofEle[i]-=Vec4i(1,1,1,1);
    }
    meshin.close();
}

//----------------------------------------------------------
//point operator + - * /
//distance & angle calculate
//Arrary operation


ostream &operator<<(ostream& out,pt &a)
{
    out<<a.v;
    return out;
}
istream &operator>>(istream& in,pt &a)
{
    in>>a.v;
    return in;
}

point& point::operator=(const point& other)
{
    v=other.v;
    return *this;
}

inline double distancebyInt(int i,int j)
{
    return dist(points[i].v,points[j].v);
}

inline double distanceofPoints(point a,point b)
{
    return dist(a.v,b.v);
}

inline int GetIndexInArray(int v,int t[],int n)
{
    for(int i=0;i<n;i++)
        if(t[i]==v)
            return i;
    return -1;
}

inline int contains(int a[],int num,int b)
{
    for(int i=0;i<num;i++)
    {
        if(a[i]==b)
            return 1;
    }
    return 0;
}

//-------------------------------------------------------------
//build boundary
//compute Quality of Ele

void buildBoundary()
{
    for(int i=0;i<numele;i++)
    {
        int a=PointVertexofEle[i][0],b=PointVertexofEle[i][1],c=PointVertexofEle[i][2];
        int d=PointVertexofEle[i][3];
        points[a].eleofnode[points[a].numeleofnode++]=i;
        points[b].eleofnode[points[b].numeleofnode++]=i;
        points[c].eleofnode[points[c].numeleofnode++]=i;
        points[d].eleofnode[points[d].numeleofnode++]=i;
    }

    for(int i=0;i<numvert;i++)
    {
        for(int j=0;j<points[i].numeleofnode;j++)
        {
            int ele=points[i].eleofnode[j];
            int a=GetIndexInArray(i,PointVertexofEle[ele].v,4);
            int b=(a+1)%4;
            int c=(a+2)%4;
            int d=(a+3)%4;
            int vb=PointVertexofEle[ele][b];
            int vc=PointVertexofEle[ele][c];
            int vd=PointVertexofEle[ele][d];
            if(!contains(points[i].neighbor,points[i].numofneighbor,vb))
                points[i].neighbor[points[i].numofneighbor++]=vb;
            if(!contains(points[i].neighbor,points[i].numofneighbor,vc))
                points[i].neighbor[points[i].numofneighbor++]=vc;
            if(!contains(points[i].neighbor,points[i].numofneighbor,vd))
                points[i].neighbor[points[i].numofneighbor++]=vd;
        }
    }

    class tri
    {
    public:
        tri(){n=0;}
        void set(int a,int b)
        {
            if(a<b)
            {
                p[0]=a;p[1]=b;
            }
            else
            {
                p[0]=b;p[1]=a;
            }
        }
        bool isSame(tri other)
        {
            if(other.p==p)
            return true;
            else return false;
        }
        tri operator=(const tri other)
        {
            p=other.p;
            return *this;
        }
        Vec2i p;
        int n;
    };

    for(int i=0;i<numvert;i++)
    {
        tri *l;
        if(points[i].isside==1)
            continue;
        l=new tri[points[i].numeleofnode*3];
        int trinum=0;
        for(int j=0;j<points[i].numeleofnode;j++)
        {
            int ele=points[i].eleofnode[j];
            int a=GetIndexInArray(i,PointVertexofEle[ele].v,4);
            int b=(a+1)%4;
            int c=(a+2)%4;
            int d=(a+3)%4;
            int vb=PointVertexofEle[ele][b];
            int vc=PointVertexofEle[ele][c];
            int vd=PointVertexofEle[ele][d];

            for(int m=0;m<3;m++)
            {
                tri temp;
                if(m==0)
                    temp.set(vb,vc);
                else if(m==1)
                    temp.set(vb,vd);
                else
                    temp.set(vc,vd);
                int t=1;
                for(int k=0;k<trinum;k++)
                {
                    if(temp.isSame(l[k]))
                    {
                        l[k].n++;
                        t=0;
                        break;
                    }
                }
                if(t){
                    l[trinum].n++;
                    l[trinum++]=temp;
                }
            }
        }
        for(int j=0;j<trinum;j++)
        {
            if(l[j].n==1)
            {
                if(points[i].isside==0)
                    points[i].isside=1;
                if(points[l[j].p[0]].isside==0)
                    points[l[j].p[0]].isside=1;
                if(points[l[j].p[1]].isside==0)
                    points[l[j].p[1]].isside=1;
            }
        }
        delete[] l;
    }
    BdPtNum=0;
    for(int i=0;i<numvert;i++)
        if(points[i].isside)
            BdPtNum++;
}


int getEdgeIndex(const BdEdge& e)
{
    for(int i=0;i < ptedgemap[e.pindex[0]].size();i++)
    {
        vector<int> &qv=ptedgemap[e.pindex[1]];
        if(find(qv.begin(),qv.end(),ptedgemap[e.pindex[0]][i])!=qv.end())
            return ptedgemap[e.pindex[0]][i];
    }
    return -1;
}


/////////////////////////////////////////////////////////////
//buildBoundaryEdge
////////////////////////////////////////////////////////////

void buildBoundaryEdge()
{

    //todo: get Boundary Faces
    set<TriFace> faceset;
    auto func = [](set<TriFace> &faceset,TriFace& tri){
        if(!faceset.count(tri)) faceset.insert(tri);
        else faceset.erase(tri);
    };
    FOREACH(i,numele){
        int sidept=0;
        int tri[4];
        FOREACH(j,4){
            int v=PointVertexofEle[i][j];
            if(points[v].isside)tri[sidept++]=v;
        }
        if(sidept==3)
            func(faceset,TriFace(tri[0],tri[1],tri[2]));
        else if(sidept==4){
            //todo
            func(faceset,TriFace(tri[0],tri[1],tri[2]));
            func(faceset,TriFace(tri[0],tri[1],tri[3]));
            func(faceset,TriFace(tri[0],tri[2],tri[3]));
            func(faceset,TriFace(tri[1],tri[2],tri[3]));
        }
    }

    faces=vector<TriFace>(faceset.begin(),faceset.end());
    faceset.clear();
    //todo: get Boundary Edges

    set<BdEdge> edgeset;
    forit(it,faces)FOREACH(i,3){
        edgeset.insert(BdEdge((*it)[i],(*it)[i==2?0:(i+1)]));
    }
    BoundaryEdges=vector<BdEdge>(edgeset.begin(),edgeset.end());
    edgeset.clear();

    BdEdgeNum=BoundaryEdges.size();
    BdFaceNum=faces.size();

    for(int i=0;i!=BdEdgeNum;i++)
    {
        ptedgemap[BoundaryEdges[i].pindex[0]].push_back(i);
        ptedgemap[BoundaryEdges[i].pindex[1]].push_back(i);
    }

    for(int i=0;i!=BdFaceNum;i++)
    {
        for(int j=0;j<3;j++){
            int a,b;
            if(j==0){a=faces[i][0];b=faces[i][1];}
            else if(j==1){a=faces[i][0];b=faces[i][2];}
            else {a=faces[i][1];b=faces[i][2];}
            int index=getEdgeIndex(BdEdge(a,b));

            faces[i].edges.push_back(index);
            BoundaryEdges[index].pushface(i);
            ptfacemap[faces[i][j]].push_back(i);
        }
    }



}

inline double dotv34(Vec3d p,Vec4d q)
{
    return p[0]*q[0]+p[1]*q[1]+p[2]*q[2]+q[3];
}

bool EdgePlaneCross(int bdEdgeIndex,Vec4d plane)
{
    Vec2i pindex=BoundaryEdges[bdEdgeIndex].pindex;
    Vec3d v1=points[pindex[0]].v,v2=points[pindex[1]].v;
    BoundaryEdges[bdEdgeIndex].isCrossed=false;
    if(dotv34(v1,plane)*dotv34(v2,plane)>0||(iszero(dotv34(v1,plane))&&iszero(dotv34(v1,plane))))
        return false;
    else {
        BoundaryEdges[bdEdgeIndex].isCrossed=true;
        SplitPointNum++;
        return true;}
}
pt EdgeSplit(int bdEdgeIndex,Vec4d v)
{
    Vec2i pindex=BoundaryEdges[bdEdgeIndex].pindex;
    Vec3d v1=points[pindex[0]].v,v2=points[pindex[1]].v;
    Vec3d n(v[0],v[1],v[2]);
    double d=dotv34(v1,v)/mod(n);
    Vec3d vec=v2-v1;
    double a=fabs(d/dot(vec,n)*mod(n));

    return pt(v1+vec*a);
}

double multiply(const pt& a,const pt& b,const pt& c)
{
    Vec2d a2(a[0],a[1]);
    Vec2d b2(b[0],b[1]);
    Vec2d c2(c[0],c[1]);
    return cross(b2-a2,c2-b2);
}


void planeCross(Vec3d n,Vec3d v)
{
    Vec4d plane(n[0],n[1],n[2],-dot(n,v));
    SplitPointNum=0;
    SplitPoints.clear();
    Split3dPoints.clear();

    for(int i=0;i<BdEdgeNum;i++)
        EdgePlaneCross(i,plane);
    if(SplitPointNum<=1)
        return ;
    /*
    TODO:
    split the mesh face by face
    */

    /*
    pt cur = EdgeSplit(edge,plane);
    SplitPoints.push_back(cur);
    while(true){
        bool get=false;
        foreach(p,2)
        {
            foreach(i,ptedgemap[BoundaryEdges[edge].pindex[p]].size())
            {
                int splitedge = ptedgemap[BoundaryEdges[edge].pindex[p]][i];
                if(splitedge==edge)continue;
                if(BoundaryEdges[splitedge].isCrossed)
                {
                    pt split = EdgeSplit(splitedge,plane);
                    if(find(SplitPoints.begin(),SplitPoints.end(),split)!=SplitPoints.end())continue;
                    SplitPoints.push_back(split);
                    edge=splitedge;
                    get=true;
                    break;
                }
            }
        }
        if(!get)break;
    }
    */
    //todo inside boundary and outside boundary

    int iter=0;
    vector<bool> used(BdEdgeNum);
    while(iter<BdEdgeNum)
    {
        for(;iter<BdEdgeNum;iter++){if(used[iter])continue; if(BoundaryEdges[iter].isCrossed) break;}
        if(iter==BdEdgeNum)break;
        int edge=iter;
        pt cur = EdgeSplit(edge,plane);
        used[edge]=true;
        vector<pt> ptvector;
        ptvector.push_back(cur);
        pt pre(1e6,1e6);
        while(true)
        {
            bool get=false;
            bool splitbypt=cur.is(points[BoundaryEdges[edge].pindex[0]])||cur.is(points[BoundaryEdges[edge].pindex[1]]);

            if(splitbypt)
            {
                int pv = cur.is(points[BoundaryEdges[edge].pindex[0]])? \
                    BoundaryEdges[edge].pindex[0]:BoundaryEdges[edge].pindex[1];
                FOREACH(i,ptfacemap[pv].size())
                {
                    int face=ptfacemap[pv][i];
                    int nextEdge=0;
                    int j=0;
                    for(;j<faces[face].edges.size();j++)if(!BoundaryEdges[faces[face].edges[j]].HavePoint(pv))break;
                    nextEdge=faces[face].edges[j];
                    if(!BoundaryEdges[nextEdge].isCrossed)continue;
                    pt split = EdgeSplit(nextEdge,plane);
                    if(split.is(pre)||split.is(cur))continue;
                    ptvector.push_back(split);
                    pre = cur;
                    cur = split;
                    edge = nextEdge;
                    used[edge]=true;
                    get = true;
                    break;
                }
            }
            else
            {
                FOREACH(k,BoundaryEdges[edge].sizeofadj())
                {
                    int s=faces[BoundaryEdges[edge].adjFace(k)].edges.size();
                    FOREACH(i,s)
                    {
                        int nextEdge = faces[BoundaryEdges[edge].adjFace(k)].edges[i];
                        if(nextEdge==edge || !BoundaryEdges[nextEdge].isCrossed)continue;
                        pt split = EdgeSplit(nextEdge,plane);
                        if(split.is(pre)||split.is(cur))continue;
                        ptvector.push_back(split);
                        pre = cur;
                        cur = split;
                        edge = nextEdge;
                        used[edge]=true;
                        get = true;
                        break;
                    }
                    if(get)break;
                }
            }
            if(!get || cur.is(ptvector[0])) break;
        }
        iter++;
        SplitPoints.push_back(ptvector);
    }


    Split3dPoints = SplitPoints;

    //this block of code transform points to 2D

    Vec<3,Vec3d> m;
    m[2]=normalized(n);
    auto p0=SplitPoints[0][0].v;
    auto p1=SplitPoints[0].size()>1?SplitPoints[0][1].v:SplitPoints[1][0].v;
    m[0]=normalized(p1-p0);
    m[1]=normalized(cross(m[2],m[0]));

    //translate 3d coordinate to 2d
    vector<vector<pt>> Sp2d;
    pt beginpoint=SplitPoints[0][0];
    forit(pit,SplitPoints){
        vector<pt> sp2dv;
        forit(it,*pit){
            Vec3d v3d=it->v-beginpoint.v;
            Vec3d v2d(dot(m[0],v3d),dot(m[1],v3d),0);
            sp2dv.push_back(v2d);
    }
        Sp2d.push_back(sp2dv);
    }

    swap(Sp2d,SplitPoints);



    /*
    SplitPoints.clear();

    pt left_bottom=Sp2d[0];
    auto lb_it=Sp2d.begin();
    //get the left and bottom point
    for(auto it=Sp2d.begin();it!=Sp2d.end();it++)
    {
        if((*it)[1]<left_bottom[1] || (*it)[1]==left_bottom[1] && (*it)[0]<left_bottom[0])
        {
            lb_it=it;
            left_bottom=*it;
        }
    }
    swap(Sp2d[0],*lb_it);

    //sort the vector by angle and distance
    for(int i=1;i<Sp2d.size()-1;i++)
    {
        int k=i;
        for(int j=i+1;j<Sp2d.size();j++)
        {
            if(multiply(Sp2d[0],Sp2d[k],Sp2d[j])<0
                || iszero(multiply(Sp2d[0],Sp2d[k],Sp2d[j])) && dist(Sp2d[i].v,Sp2d[k].v)>dist(Sp2d[i].v,Sp2d[j].v))
                k=j;
        }
        swap(Sp2d[i],Sp2d[k]);
    }

    //erase the same pt from the sp2d

    for(auto it=Sp2d.begin();it+1!=Sp2d.end();it++)
    {
        auto it2=it+1;
        while(iszero((*it)[0]-(*it2)[0]) && iszero((*it)[1]-(*it2)[1]))
        {
            Sp2d.erase(it2);
            it2=it+1;
        }
    }

    //get the tubao
    vector<pt> res;
    queue<pt> erased;
    if(Sp2d.size()<3)
        res=Sp2d;
    else
    {
        res.push_back(Sp2d[0]);
        res.push_back(Sp2d[1]);
        res.push_back(Sp2d[2]);
        int top=2;
        for(int i=3;i<Sp2d.size();i++)
        {
            while(multiply(Sp2d[i],res[top],res[top-1])>0)
            {
                erased.push(res[top]);
                top--;
                res.pop_back();
            }
            res.push_back(Sp2d[i]);
            top++;
        }
    }
    Sp2d.clear();
    */

    /*
    //insert the erased point to the vector
    while(!erased.empty())
    {
        double mind=1e10;
        vector<pt>::iterator itx;
        bool found=false;
        for(auto it=res.begin();it!=res.end();it++)
        {
            vector<pt>::iterator it2;
            if(it+1==res.end())
            {
                it2=res.begin();
            }
            else it2=it+1;
            Vec3d o=erased.front().v;
            Vec3d oa=o-it->v;
            Vec3d n=normalized(Vec3d(it->v[1]-it2->v[1],it2->v[0]-it->v[0],0));
            double d=dot(oa,n);
            Vec3d h=o-d*n;
            if((it->v[0]-h[0])*(h[0]-it2->v[0])>=0 && (it->v[1]-h[1])*(h[1]-it2->v[1])>=0)
            {
                found=true;
                if(mind>d)
                {
                    if(it2==res.begin())
                        itx=res.end();
                    else
                        itx=it2;
                    mind=d;
                }
            }
        }
        if(found)
        {
            res.insert(itx,erased.front());
        }
        erased.pop();
    }
    */
    //vec2d ->vec3d
//	translate(m);
/*
    for(auto it=res.begin();it!=res.end();it++)
    {
        Vec3d v2d=it->v;
        Vec3d v3d=beginpoint.v+Vec3d(dot(m[0],v2d),dot(m[1],v2d),dot(m[2],v2d));
        SplitPoints.push_back(v3d);
    }
    */
//	SplitPoints = res;
}

void ClearMemory()
{
    delete[] points;
    delete[] PointVertexofEle;
    delete[] Elements;
//	delete[] qualityofele;
//	delete[] BoundaryEdges;
}
