// Coding: UTF-8(BOM)

#include "mainwindow.h"
#include "donewdialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

//    DoNewDialog v;
//    v.show();

    return a.exec();
}
