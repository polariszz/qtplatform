#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QtWidgets>
#include "local.h"
using namespace ZZ;
using std::vector;


class renderArea : public QWidget{
    Q_OBJECT

public:
    explicit renderArea(QWidget *parent = 0);
public slots:
    void setPaintPath(VVP*);
    void setRotationAngle(int angle);

protected:
    void paintEvent(QPaintEvent *event);
public:
    enum {SHOW_TEXT, SHOW_COMPUTING, SHOW_PATH };
    void showText();
    void showPath();
    void showComputing();
    int getState() const {return state;}

private:

    int state;
    VVP *vvp;
    int rotationAngle;
    void drawText();
    void drawPath();
    void drawComputing();
};



#endif // RENDERAREA_H
