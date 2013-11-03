#ifndef LOCAL_H
#define LOCAL_H

#include <QtWidgets>

#define R QStringLiteral

#define  LESS(x,y) (x)<(y)?(x):(y)
#define  MORE(x,y) (x)>(y)?(x):(y)
#define forit(it,con) for(auto it=(con).begin();it!=(con).end();it++)

class local{
private:
    local();
    static local *initance;
public:
    static local* get_initance();
    const QString welcomeInfo1;
    const QString welcomeInfo2;
    const QPen welcomePen;
    const QFont textFont;
    const QFont computingFont;
    const QString fileFilter;
    const QString computingText1;
    const QString computingText2;
};

local* LOCAL();

namespace ZZ {
    QStringList generate_APDL(QString modelName);

    struct pt2d{
        double x,y;
        pt2d(double xx,double yy):x(xx),y(yy){};
    };

    typedef std::vector<pt2d> VP;
    typedef std::vector<VP> VVP;
    typedef std::vector<VVP> VVVP;

}


#endif // LOCAL_H
