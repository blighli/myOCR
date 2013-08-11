#include <QtGui>
#include "MainWindow.h"
#include "ImageWidget.h"

MainWindow::MainWindow()
{
	resize(800, 600);

	imageWidget = new ImageWidget();
	setCentralWidget(imageWidget);

	menuFile = menuBar()->addMenu(tr("&File"));
	toolBarFile = addToolBar(tr("&File"));

	actionFileOpen = new QAction(tr("&Open"), this);
	connect(actionFileOpen, SIGNAL(triggered()), this, SLOT(openFile()));

	menuFile->addAction(actionFileOpen);
	toolBarFile->addAction(actionFileOpen);
}

void MainWindow::openFile()
{
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Open Image"), ".",
		tr("Image files (*.png)"));
    if (!fileName.isEmpty())
	{
		imageWidget->openImage(fileName);
	}
}