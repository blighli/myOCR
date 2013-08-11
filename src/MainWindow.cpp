#include "MainWindow.h"
#include "ImageWidget.h"

MainWindow::MainWindow()
{
	imageWidget = new ImageWidget();
	setCentralWidget(imageWidget);
}