#include "platform.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForUtfText("UTF-8"));
   // QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    platform w;
    w.show();

    return a.exec();
}
