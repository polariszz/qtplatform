#ifndef PLATFORM_H
#define PLATFORM_H

#include <QMainWindow>

class renderArea;
class QPushButton;

namespace Ui {
class platform;
}

class platform : public QMainWindow
{
    Q_OBJECT

public:
    explicit platform(QWidget *parent = 0);
    ~platform();

private:
    Ui::platform *ui;
    QPushButton* setPath;
    QPushButton* computeDataGenerate;
    QPushButton* geoDataGenerate;
    QPushButton* getLoadBoundary;
    QPushButton* loadInterpCompute;
    QPushButton* coldHotTranfer;
    QPushButton* showResult;
    QPushButton* editCutPlane;
    QPushButton* cutPlane;
    QPushButton* showCuttingPath;
    QPushButton* prePath;
    QPushButton* nextPath;
    QString icemPath;
    QString ansysPath;
    QString proName;
    renderArea* canvas;
};

#endif // PLATFORM_H
