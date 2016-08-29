#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{

	QApplication a(argc, argv);
	QSurfaceFormat format;
	format.setVersion(4,1);
	format.setProfile(QSurfaceFormat::CoreProfile);
	QSurfaceFormat::setDefaultFormat(format);
	MainWindow w;
	w.show();

	return a.exec();
}
