#include "renderarea.h"
#include "local.h"


renderArea::renderArea(QWidget *parent) :
    QWidget(parent)
{
    this->setMinimumWidth(350);
    path = QPainterPath();
    rotationAngle = 0;
}

void renderArea::setPaintPath(QPainterPath &qpath){
    this->path = qpath;
}

void renderArea::setRotationAngle(int angle){
    this->rotationAngle = angle;
}

void renderArea::paintEvent(QPaintEvent*){
    /*
    QPainterPath path;
    path.moveTo(20, 80);
    path.lineTo(20, 30);
    path.cubicTo(80, 0, 50, 50, 80, 80);
    QPainter painter(this);
    painter.drawPath(path);
    painter.drawText(this->rect(), LOCAL()->welcomeInfo1);
    */
    showText();
}


void renderArea::showText(){
    QPainter painter(this);
    painter.setPen(LOCAL()->welcomePen);
    painter.setFont(LOCAL()->textFont);
    QRect r = this->rect();
    QRect top(r.left(), r.top(), r.width(), r.height()/3*2);
    QRect bottom(r.left(), r.top()+r.height()/3, r.width(), r.height()/3*2);
    painter.drawText(top, Qt::AlignHCenter|Qt::AlignVCenter ,LOCAL()->welcomeInfo1);
    painter.drawText(bottom, Qt::AlignHCenter|Qt::AlignVCenter, LOCAL()->welcomeInfo2);

}

void renderArea::showPath(){

}
