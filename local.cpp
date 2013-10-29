#include "local.h"

local* local::initance = NULL;

local::local():welcomeInfo1(QStringLiteral("压气机零件冷热态")),
    welcomeInfo2(QStringLiteral("转换计算平台")),
    welcomePen(QPen(Qt::blue, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin)),
    textFont(QFont(QStringLiteral("华文彩云"), 25, -1, false))
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
