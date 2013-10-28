#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QtWidgets>

class renderArea : public QWidget{
    Q_OBJECT

public:
    explicit renderArea(QWidget *parent = 0);
public slots:
    void setPaintPath(QPainterPath &path);
    void setRotationAngle(int angle);
protected:
    void paintEvent(QPaintEvent *event);

private:
    QPainterPath path;
    int rotationAngle;
};



#endif // RENDERAREA_H
