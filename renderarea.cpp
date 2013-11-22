#include "renderarea.h"
#include "local.h"


renderArea::renderArea(QWidget *parent) :
    QWidget(parent),
    state(SHOW_TEXT)
{
    this->setMinimumWidth(350);
    rotationAngle = 0;
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
    QRect top(r.left(), r.top(), r.width(), r.height()/4*3);
    QRect bottom(r.left(), r.top() + r.height()/6, r.width(), r.height()/4*3);
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
    QPainter painter(this);
    painter.setPen(LOCAL()->welcomePen);
    painter.setFont(LOCAL()->computingFont);

    QRect rect = this->rect();
    vector<vector<QPoint>> vvcp;

    QPainterPath path;
    double minx, miny, maxx, maxy;
    if (!vvp) return;

    forit(vp,*vvp)forit(it,*vp){
        if(it == vp->begin() && vp ==vvp->begin()){
            maxx = minx = (*it).x;
            maxy = miny = (*it).y;
        }
        else{
            minx = LESS(minx,(*it).x);
            miny = LESS(miny,(*it).y);
            maxx = MORE(maxx,(*it).x);
            maxy = MORE(maxy,(*it).y);
        }
    }
    double kx = (rect.width()-50.0)/(maxx-minx);
    double ky = (rect.height()-50.0)/(maxy-miny);
    double k = LESS(kx,ky);

    forit(vp,*vvp)
    {
        vector<QPoint> vcp;
        forit(it,*vp){
            int x =rect.width()/2 - (int)(((minx + maxx)/2 - (*it).x)*k );
            int y =rect.height()/2 - (int)(((miny + maxy)/2 - (*it).y)*k );
            vcp.push_back(QPoint(x,y));
        }
        vvcp.push_back(vcp);
    }
    forit(vcp, vvcp) {
        vector<QPoint>::iterator vit = vcp->begin();
        path.moveTo(*vit++);
        while(vit!=vcp->end()) path.lineTo(*vit++);
        path.lineTo(*(vcp->begin()));
    }
    painter.drawPath(path);
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

void renderArea::setPaintPath(VVP *rvvp) {
    vvp = rvvp;

    update();
}
