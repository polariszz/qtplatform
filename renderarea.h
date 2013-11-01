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

public:
    void showText();
    void showPath();
    void showComputing();

private:
    enum {SHOW_TEXT, SHOW_COMPUTING, SHOW_PATH };
    int state;
    QPainterPath path;
    int rotationAngle;
    void drawText();
    void drawPath();
    void drawComputing();
};



#endif // RENDERAREA_H
