#include "mybutton.h"

const int myButton::MINI_WIDTH = 50;

myButton::myButton(QString string):QPushButton(string)
{
    setWidth();
}

void myButton::setWidth(){
    this->setMinimumWidth(myButton::MINI_WIDTH);
    this->setFixedWidth(2*myButton::MINI_WIDTH + 10);
}
