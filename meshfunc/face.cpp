#include"face.h"

bool operator<(const TriFace&lhs, const TriFace&rhs){
    return lhs.pindex[0]<rhs.pindex[0] || (lhs.pindex[0]==rhs.pindex[0]&&lhs.pindex[1]<rhs.pindex[1]) \
        ||(lhs.pindex[0]==rhs.pindex[0]&&lhs.pindex[1]==rhs.pindex[1]&&lhs.pindex[2]<rhs.pindex[2]);
}
