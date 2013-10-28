#include "renderarea.h"

renderArea::renderArea(QWidget *parent) :
    QWidget(parent)
{
    path = QPainterPath();
    rotationAngle = 0;
}

renderArea::setPaintPath(QPainterPath &qpath){
    this->path = qpath;
}

renderArea::setRotationAngle(int angle){
    this->rotationAngle = angle;
}
