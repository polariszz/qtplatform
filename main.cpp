#include "platform.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    platform w;
    w.show();

    return a.exec();
}
