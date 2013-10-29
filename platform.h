#ifndef PLATFORM_H
#define PLATFORM_H

#include <QMainWindow>

class renderArea;
class myButton;

namespace Ui {
class platform;
}

class platform : public QMainWindow
{
    Q_OBJECT

public:
    explicit platform(QWidget *parent = 0);
    ~platform();

//methods

public slots:
    void set_path();

private:
    void ui_init();
    void ui_connect_function();
    void ui_set_contraints();

//members
private:
    Ui::platform *ui;
    myButton* setPath;
    myButton* computeDataGenerate;
    myButton* geoDataGenerate;
    myButton* getLoadBoundary;
    myButton* loadInterpCompute;
    myButton* coldHotTranfer;
    myButton* showResult;
    myButton* editCutPlane;
    myButton* cutPlane;
    myButton* showCuttingPath;
    myButton* prePath;
    myButton* nextPath;
    QString icemPath;
    QString ansysPath;
    QString proName;
    renderArea* canvas;
};

#endif // PLATFORM_H
