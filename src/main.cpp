#include <QApplication>
#include <QtCore/QtCore>
#include "MainWindow.h"
#include "Shared/AppInfo.h"

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	AppInfo::instance()->setAppPath(argv[0]);

	MainWindow mainWindow;
	mainWindow.show();
	
	return app.exec();
}