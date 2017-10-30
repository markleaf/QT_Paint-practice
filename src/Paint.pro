#-------------------------------------------------
#
# Project created by QtCreator 2016-08-12T18:47:57
#
#-------------------------------------------------

QT       += core gui
QT       += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Paint
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    donewdialog.cpp \
    paintarea.cpp

HEADERS  += mainwindow.h \
    donewdialog.h \
    paintarea.h

FORMS    += mainwindow.ui \
    donewdialog.ui

RESOURCES += \
    Paint.qrc
