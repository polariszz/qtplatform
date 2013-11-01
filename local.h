#ifndef LOCAL_H
#define LOCAL_H

#include <QtWidgets>
#define R QStringLiteral

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
}



#endif // LOCAL_H
