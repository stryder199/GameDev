#include <QtGui/QApplication>
#ifndef __APPLE__
#define GLEW_STATIC
#include <GL/glew.h>
#endif
#include <QGLFormat>
#include <QMessageBox>
#include "mainwindow.h"
int main(int argc, char *argv[])
{
	bool result;

	QApplication a(argc, argv);

	// Specify an OpenGL 3.2 format using the Core profile.
	// That is, no old-school fixed pipeline functionality
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
	QGLFormat glFormat;
	glFormat.setVersion( 3, 2 );
	glFormat.setProfile( QGLFormat::CoreProfile ); // Requires >=Qt-4.8.0
	glFormat.setSampleBuffers( true );

	QGLFormat::setDefaultFormat(glFormat);
#endif

	QString sDir = QCoreApplication::applicationDirPath();
	std::string Dir = sDir.toLocal8Bit().constData();
	a.addLibraryPath(sDir+"/plugins");

	MainWindow w;
	w.show();

	return a.exec();
}