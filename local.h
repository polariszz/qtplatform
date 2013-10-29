#ifndef LOCAL_H
#define LOCAL_H

#include <QtWidgets>

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
};

local* LOCAL();


#endif // LOCAL_H
