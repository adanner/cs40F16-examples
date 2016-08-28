#include "mainwindow.h"
#include <QApplication>
#include <QGLFormat>

int main(int argc, char *argv[])
{
    /*TODO: Set format before initializing application*/

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
