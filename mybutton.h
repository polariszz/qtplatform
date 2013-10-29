#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QPushButton>
class QStringLiteral;

class myButton : public QPushButton
{
public:
    myButton(QString string);
    static const int MINI_WIDTH;
private:
    void setWidth();

};

#endif // MYBUTTON_H
