#include <QApplication>
#include "MainWindow.h"
#include "AppInfo.h"

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	QString appPath = QString::fromAscii(argv[0]);
	AppInfo::instance()->setAppPath(appPath);

	MainWindow mainWindow;
	mainWindow.show();
	
	return app.exec();
}