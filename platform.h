#ifndef PLATFORM_H
#define PLATFORM_H

#include <QMainWindow>

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
};

#endif // PLATFORM_H
