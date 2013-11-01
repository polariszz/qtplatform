#include "local.h"



local* local::initance = NULL;

local::local():welcomeInfo1(QStringLiteral("压气机零件冷热态")),
    welcomeInfo2(QStringLiteral("转换计算平台")),
    computingText1(QStringLiteral("计算中...")),
    computingText2(QStringLiteral("请耐心等待...")),
    welcomePen(QPen(Qt::blue, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin)),
    textFont(QFont(QStringLiteral("华文彩云"), 25, -1, false)),
    computingFont(QFont(QStringLiteral("宋体"), 30, -1, false)),
    fileFilter(QString("pro Files (*.pro)"))
{
}

local* local::get_initance(){
    if(!initance){
        initance = new local();
    }
    return initance;
}

local* LOCAL(){
    return local::get_initance();
}

namespace ZZ {

}
