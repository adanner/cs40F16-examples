#include "mainwindow.h"
#include <QtWidgets/QApplication>
#include <QtOpenGL/QGLFormat>

int main(int argc, char *argv[])
{
    /*OpenGL format must be set before launching application*/
    QGLFormat format;
    format.setDoubleBuffer(true);  /* use double buffering (default) */
    format.setDepth(true); /* enable depth buffer (default) */
    format.setProfile(QGLFormat::CoreProfile); /*disallow deprecated features*/
    QGLFormat::setDefaultFormat(format);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
