#-------------------------------------------------
#
# Project created by QtCreator 2010-08-17T09:04:07
#
#-------------------------------------------------

QT  += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET  = demo_qwt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
        scrollbar.cpp\
        scrollzoomer.cpp

HEADERS += mainwindow.h\
           scrollbar.h\
           scrollzoomer.h

FORMS   += mainwindow.ui

INCLUDEPATH += /usr/include/qwt
LIBS += -L/usr/lib -lqwt
