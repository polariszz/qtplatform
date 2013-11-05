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

void init2()
{
    cout<<"Please input the name of meshfile:";
    cin>>filename;
    int pos=filename.find_last_of('.');
    filename.assign(filename,0,pos);

    string meshfile;
    meshfile=datapath+filename+".txt";
    ifstream in(meshfile);
    if(!in)
        cout<<"open file error!";
    string str;
    in>>str;
    in>>numvert;
    points=new point[numvert];

    for(int i=0;i<numvert;i++)
    {
        int t;
        in>>t>>points[i].v;
    }
    in>>str>>numele;

    PointVertexofEle=new Vec4i[numele];
    Elements=new Cell[numele];
    for(int i=0;i<numele;i++)
    {
        int t;
        in>>t;
        in>>PointVertexofEle[i];

        PointVertexofEle[i]-=Vec4i(1,1,1,1);
/*		for(int j=0;j<4;j++)
            if(PointVertexofEle[i][j]>20000)
                PointVertexofEle[i][j]-=19044;*/
    }

    int num_pt_with_disp;
    in>>str>>num_pt_with_disp;
    for(int i=0;i<num_pt_with_disp;i++)
    {
        int t;
        in>>t;
        in>>points[t-1].disp;
    }

    in.close();
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

double computeQuality(int i)
{
    Vec4i p=PointVertexofEle[i];
    double a12=distancebyInt(p[0],p[1]);
    double a13=distancebyInt(p[0],p[2]);
    double a14=distancebyInt(p[0],p[3]);
    double a23=distancebyInt(p[1],p[2]);
    double a24=distancebyInt(p[1],p[3]);
    double a34=distancebyInt(p[2],p[3]);

    double a = a12*a34;
    double b = a13*a24;
    double c = a14*a23;
    //各面的面积
    double A1= points[p[1]].v[0]*(points[p[2]].v[1]*points[p[3]].v[2]-points[p[3]].v[1]*points[p[2]].v[2])-points[p[2]].v[0]*(points[p[1]].v[1]*points[p[3]].v[2]-points[p[3]].v[1]*points[p[1]].v[2])+points[p[3]].v[0]*(points[p[1]].v[1]*points[p[2]].v[2]-points[p[2]].v[1]*points[p[1]].v[2]);
    double A2= points[p[0]].v[0]*(points[p[2]].v[1]*points[p[3]].v[2]-points[p[3]].v[1]*points[p[2]].v[2])-points[p[2]].v[0]*(points[p[0]].v[1]*points[p[3]].v[2]-points[p[3]].v[1]*points[p[0]].v[2])+points[p[3]].v[0]*(points[p[0]].v[1]*points[p[2]].v[2]-points[p[2]].v[1]*points[p[0]].v[2]);
    double A3= points[p[0]].v[0]*(points[p[1]].v[1]*points[p[3]].v[2]-points[p[3]].v[1]*points[p[1]].v[2])-points[p[1]].v[0]*(points[p[0]].v[1]*points[p[3]].v[2]-points[p[3]].v[1]*points[p[0]].v[2])+points[p[3]].v[0]*(points[p[0]].v[1]*points[p[1]].v[2]-points[p[1]].v[1]*points[p[0]].v[2]);
    double A4= points[p[0]].v[0]*(points[p[1]].v[1]*points[p[2]].v[2]-points[p[2]].v[1]*points[p[1]].v[2])-points[p[1]].v[0]*(points[p[0]].v[1]*points[p[2]].v[2]-points[p[2]].v[1]*points[p[0]].v[2])+points[p[2]].v[0]*(points[p[0]].v[1]*points[p[1]].v[2]-points[p[1]].v[1]*points[p[0]].v[2]);
    //体积
    double v = -(A1-A2+A3-A4)/6;

    double p123 = (a12+a13+a23)/2;
    double s123 = sqrt(p123*(p123-a12)*(p123-a13)*(p123-a23));
    double p124 = (a12+a14+a24)/2;
    double s124 = sqrt(p124*(p124-a12)*(p124-a14)*(p124-a24));
    double p134 = (a14+a13+a34)/2;
    double s134 = sqrt(p134*(p134-a14)*(p134-a13)*(p134-a34));
    double p234 = (a24+a34+a23)/2;
    double s234 = sqrt(p234*(p234-a24)*(p234-a34)*(p234-a23));

    double R = sqrt((a+b+c)*(a+b-c)*(a+c-b)*(b+c-a))/(24*v);  //外接圆半径
    double r = 3*v/(s123+s124+s134+s234);            //内接圆半径

    double k = 3*r/R;
    Vec3d na,nb,nc;
    na=points[p[1]].v - points[p[0]].v;
    nb=points[p[2]].v - points[p[0]].v;
    nc=points[p[3]].v - points[p[0]].v;
    double sign=dot(cross(na,nb),nc);
    if(sign>0) return k;
    else return k*-1;
}


//-----------------------------------------------------------
//outputVTK
//WriteDXF
//LineDXF
//DrawLine

void outputVTK()
{
    ofstream outvtk;
    char c[50];
    cout<<"input the outputvtkfilename: ";
    cin>>c;
    char filename[50]="../OUTPUT/";
    strcat(filename,c);
    strcat(filename,".vtk");
    outvtk.open(filename);
    outvtk<<"# vtk DataFile Version 2.0\n";
    outvtk<<"tet Mesh Smoothed_Mesh .\n";

    outvtk<<"ASCII\n";

    outvtk<<"DATASET UNSTRUCTURED_GRID\n";
    outvtk<<"POINTS "<<numvert<<" float\n";

    for(int i=0;i<numvert;i++)
        outvtk<<points[i].v[0]<<" "<<points[i].v[1]<<" "<<points[i].v[2]<<endl;
    outvtk<<"CELLS "<<numele<<" "<<numele*5<<endl;
    for(int i=0;i<numele;i++)
    {
        outvtk<<"4"<<" "<<PointVertexofEle[i][0]<<" "<<PointVertexofEle[i][1]<<" "<<PointVertexofEle[i][2]<<" "<<PointVertexofEle[i][3]<<endl;
    }
    outvtk<<"CELL_TYPES "<<numele<<"\n";
    for(int i=0;i<numele;i++)
        outvtk<<10<<endl;
    outvtk<<"CELL_DATA "<<numele<<"\n";
    outvtk<<"SCALARS Quality float\n";
    outvtk<<"LOOKUP_TABLE default\n";
    for(int i=0;i<numele;i++)
        outvtk<<computeQuality(i);
    outvtk.close();
}




void WriteDXF(string filename)
{
    ofstream outfile(filename);
    outfile<<"0\nSECTION\n";
    outfile<<"2\nENTITIES\n";

    for(int i=0;i<numele;i++)
    {
        for(int k=0;k<6;k++)
        {
            Vec2i a;
            if(k==0)a=Vec2i(0,1);
            else if(k==1)a=Vec2i(1,2);
            else if(k==2)a=Vec2i(2,0);
            else if(k==3)a=Vec2i(0,3);
            else if(k==4)a=Vec2i(1,3);
            else if(k==5)a=Vec2i(2,3);

            point start,end;
            start=points[PointVertexofEle[i][a[0]]];
            end  =points[PointVertexofEle[i][a[1]]];
            DrawLine(outfile,start,end);
        }
    }
    outfile<<"0\nENDSEC\n";
    outfile<<"0\nEOF";
    outfile.close();
}

void DrawLine(ofstream &outfile,point &start,point &end)
{
    outfile<<"0\nLINE\n";
    outfile<<"8\n0\n";
    outfile<<"10\n"<<start.v[0]<<"\n20 \n"<<start.v[1]<<"\n30 \n"<<start.v[2]<<"\n11\n"<<end.v[0]<<"\n21\n"<<end.v[1]<<"\n31 \n"<<end.v[2]<<"\n";
}


void outputQualityDistribution(int t)
{
    double q_sum=0;
    double q_min=1;
    double quality[11]={0};
    for(int i=0;i<numele;i++)
    {
        double q=computeQuality(i);
        q_sum+=q;
        q_min=q_min<q?q_min:q;
        for(int j=0;j<11;j++)
            if(q<j*0.1+1e-5)
            {
                quality[j]++;
                break;
            }
    }
    if(t != 0)
        cout<<"before smoothing, the distribution of quality:"<<endl;
    else cout<<"after smoothing, the distribution of quality:"<<endl;
    cout<<"       (-inf, 0]:  "<<quality[0]*100/numele<<"%"<<endl;
    cout<<"       (0  ,0.1]:  "<<quality[1]*100/numele<<"%"<<"\n";
    cout<<"       (0.1,0.2]:  "<<quality[2]*100/numele<<"%"<<"\n";
    cout<<"       (0.2,0.3]:  "<<quality[3]*100/numele<<"%"<<"\n";
    cout<<"       (0.3,0.4]:  "<<quality[4]*100/numele<<"%"<<"\n";
    cout<<"       (0.4,0.5]:  "<<quality[5]*100/numele<<"%"<<"\n";
    cout<<"       (0.5,0.6]:  "<<quality[6]*100/numele<<"%"<<"\n";
    cout<<"       (0.6,0.7]:  "<<quality[7]*100/numele<<"%"<<"\n";
    cout<<"       (0.7,0.8]:  "<<quality[8]*100/numele<<"%"<<"\n";
    cout<<"       (0.8,0.9]:  "<<quality[9]*100/numele<<"%"<<"\n";
    cout<<"       (0.9,  1]:  "<<quality[10]*100/numele<<"%"<<"\n";

    cout<<"average quality: "<<q_sum/numele<<"\n"<<"minimum quality: "<<q_min<<"\n";
}


//---------------------------------------------------------------
//EleSmoothbySMEC
//EleBasedSmooth

Vec3d vec(int &a,int &b)
{
    return points[b].v-points[a].v;
}

double det(Vec3d &v1,Vec3d &v2,Vec3d &v3,Vec3d &v4)
{
    Vec3d a[3];
    a[0]=v2-v1;
    a[1]=v3-v1;
    a[2]=v4-v1;
    double res=0;
    res+=a[0][0]*(a[1][1]*a[2][2]-a[1][2]*a[2][1]);
    res-=a[0][1]*(a[1][0]*a[2][2]-a[1][2]*a[2][0]);
    res+=a[0][2]*(a[1][0]*a[2][1]-a[1][1]*a[2][0]);
    return res;
}

void EleSmoothbySMEC(int va)
{
    Vec4i p=PointVertexofEle[va];
    Vec3d n[4];
    Vec3d n1,n2;
    double SIGMA=0;
    n1=vec(p[3],p[1]);
    n2=vec(p[2],p[1]);
    n[0]=cross(n1,n2);
    SIGMA+=mod(n1)+mod(n2);

    n1=vec(p[3],p[2]);
    n2=vec(p[0],p[2]);
    n[1]=cross(n1,n2);

    SIGMA+=mod(n1)+mod(n2);

    n1=vec(p[1],p[3]);
    n2=vec(p[0],p[3]);
    n[2]=cross(n1,n2);
    SIGMA+=mod(n2);

    n1=vec(p[1],p[0]);
    n2=vec(p[2],p[0]);
    n[3]=cross(n1,n2);
    SIGMA+=mod(n1);
    SIGMA/=6;

    Vec3d v[4];
    for(int i=0;i<4;i++)
        v[i]=points[p[i]].v+n[i]/mod(n[i])*SIGMA;


//	double para=dist(v[0],v[1])+dist(v[0],v[2])+dist(v[0],v[3])+dist(v[1],v[2])+dist(v[1],v[3])+dist(v[2],v[3]);
//	double k=6*SIGMA/para;

    double k=det(points[p[0]].v,points[p[1]].v,points[p[2]].v,points[p[3]].v)/det(v[0],v[1],v[2],v[3]);
    if(k<0)
        k=-(pow(-k,1.0/3));
    else k=pow(k,1.0/3);


        Vec3d c=(points[p[0]].v+points[p[1]].v+points[p[2]].v+points[p[3]].v)/4;
        Vec3d vc=(v[0]+v[1]+v[2]+v[3])/4;
        for(int i=0;i<4;i++)
            Elements[va].p[i]=c+(v[i]-vc)*k;


}

double qofEle(int)
{
    return 0;
}

void EleBasedSmooth(void fun(int),int K)
{
    qualityofele=new double[numele];
    for(int k=0;k<K;k++)
    {
        for(int i=0;i<numele;i++)
        {
            qualityofele[i]=computeQuality(i);
            fun(i);
        }
        for(int i=0;i<numvert;i++)
        {
            double x=0,y=0,z=0;
            if(points[i].isside)
                continue;

            for(int j=0;j<points[i].numeleofnode;j++)
            {
                int t=points[i].eleofnode[j];
                int v=GetIndexInArray(i,PointVertexofEle[t].v,4);
                x+=Elements[t].p[v].v[0];
                y+=Elements[t].p[v].v[1];
                z+=Elements[t].p[v].v[2];
            }
            x/=points[i].numeleofnode;
            y/=points[i].numeleofnode;
            z/=points[i].numeleofnode;

            Vec3d temp = points[i].v;
            points[i].v[0]=x;
            points[i].v[1]=y;
            points[i].v[2]=z;

            bool isOK = true;
            for(int j=0;j<points[i].numeleofnode;j++)
            {
                int t = points[i].eleofnode[j];
                if(computeQuality(t)<0.05)
                {
                    isOK = false;
                    break;
                }
            }

            if(!isOK)
                points[i].v = temp;
        }
    }
    delete[] qualityofele;
}

//--------------------------------------------------------
//PointBasedSmooth
//LaplacianSmoothMethod

void LaplacianSmoothOnePoint(int va)
{
    double x=0,y=0,z=0;
    for(int i=0;i<points[va].numofneighbor;i++)
    {
        int p=points[va].neighbor[i];
        x+=points[p].v[0];
        y+=points[p].v[1];
        z+=points[p].v[2];
    }
    points[va].v[0]=x/points[va].numofneighbor;
    points[va].v[1]=y/points[va].numofneighbor;
    points[va].v[2]=z/points[va].numofneighbor;

}
void PointBasedSmooth(void fun(int ),int K)
{
    for(int k=0;k<K;k++)
    {
        for(int i=0;i<numvert;i++)
        {
            if(points[i].isside)
                continue;

            fun(i);

        }
    }
}

void ClearMemory()
{
    delete[] points;
    delete[] PointVertexofEle;
    delete[] Elements;
//	delete[] qualityofele;
//	delete[] BoundaryEdges;
}
