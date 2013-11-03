#ifndef _EDGE_
#define _EDGE_
#include"vec.h"
using namespace std;
class Edge
{
public:
    Edge(){}
    Vec2i pindex;
    bool HavePoint(int i){return pindex[0]==i||pindex[1]==i;}
    bool operator==(Edge& other)
    {
        return this->pindex == other.pindex;
    }
};

class BdEdge:public Edge
{
public:

    BdEdge(){isCrossed=0;}
    BdEdge(int a,int b){
        if(a>b){int temp=a;a=b;b=temp;};
        pindex[0]=a;pindex[1]=b;isCrossed=0;}
    bool isCrossed;
    void pushface(int i){
        adj_face.push_back(i);
    }
    int adjFace(int i){
        return adj_face[i];
    }
    int sizeofadj(){return adj_face.size();}
    bool operator==(const BdEdge& other)
    {
        return this->pindex[0] == other.pindex[0]&&this->pindex[1]==other.pindex[1];
    }
    friend bool operator<(const BdEdge& lhs,const BdEdge& rhs){
        return lhs.pindex[0]<rhs.pindex[0] ||(lhs.pindex[0]==rhs.pindex[0]&&lhs.pindex[1]<rhs.pindex[1]);
    };
private:
    vector<int> adj_face;
};


#endif //_EDGE_
