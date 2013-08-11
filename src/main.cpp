#include <QApplication>
#include <QTextCodec>

#include "MainWindow.h"

int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

	MainWindow mainWindow;
	mainWindow.show();
	
	return app.exec();
}