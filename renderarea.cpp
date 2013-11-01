#include "renderarea.h"
#include "local.h"


renderArea::renderArea(QWidget *parent) :
    QWidget(parent),
    state(SHOW_TEXT)
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

    switch(state) {
    case SHOW_TEXT : drawText();break;
    case SHOW_PATH : drawPath();break;
    case SHOW_COMPUTING : drawComputing();break;
    default : drawText();
    }
}


void renderArea::drawText(){
    QPainter painter(this);
    painter.setPen(LOCAL()->welcomePen);
    painter.setFont(LOCAL()->textFont);
    QRect r = this->rect();
    QRect top(r.left(), r.top(), r.width(), r.height()/3*2);
    QRect bottom(r.left(), r.top()+r.height()/3, r.width(), r.height()/3*2);
    painter.drawText(top, Qt::AlignHCenter|Qt::AlignVCenter ,LOCAL()->welcomeInfo1);
    painter.drawText(bottom, Qt::AlignHCenter|Qt::AlignVCenter, LOCAL()->welcomeInfo2);

}

void renderArea::drawComputing(){
    QPainter painter(this);
    painter.setPen(LOCAL()->welcomePen);
    painter.setFont(LOCAL()->computingFont);
    QRect r = this->rect();
    QRect top(r.left(), r.top(), r.width(), r.height()/3*2);
    QRect bottom(r.left(), r.top()+r.height()/3, r.width(), r.height()/3*2);
    painter.drawText(top, Qt::AlignHCenter|Qt::AlignVCenter ,LOCAL()->computingText1);
    painter.drawText(bottom, Qt::AlignHCenter|Qt::AlignVCenter, LOCAL()->computingText2);
}

void renderArea::drawPath(){

}

void renderArea::showText(){
    state = SHOW_TEXT;
    repaint();
}

void renderArea::showPath() {
    state = SHOW_PATH;
    repaint();
}

void renderArea::showComputing() {
    state = SHOW_COMPUTING;
    repaint();
}
