#-------------------------------------------------
#
# Project created by QtCreator 2013-10-28T22:03:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtplatform
TEMPLATE = app


SOURCES += main.cpp\
        platform.cpp \
    renderarea.cpp \
    mybutton.cpp \
    local.cpp \
    setpathdialog.cpp \
    addition/fileoperate.cpp \
    addition/foo.cpp \
    meshfunc/face.cpp \
    meshfunc/smooth.cpp

HEADERS  += platform.h \
    renderarea.h \
    mybutton.h \
    local.h \
    setpathdialog.h \
    addition/fileoperate.h \
    addition/foo.h \
    addition/addi.h \
    meshfunc/face.h \
    meshfunc/vec.h \
    meshfunc/smooth.h \
    meshfunc/Edge.h

FORMS    += platform.ui

RC_FILE = platform.rc
