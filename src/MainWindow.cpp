#include <QtGui>
#include "MainWindow.h"
#include "ImageWidget.h"

#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include "ImageAdapter.h"

#include <QDebug>

MainWindow::MainWindow()
{
	cvImage = NULL;
	labelOCRText = new QLabel();

	resize(800, 600);

	imageWidget = new ImageWidget();
	setCentralWidget(imageWidget);

	menuFile = menuBar()->addMenu(tr("&File"));
	toolBarFile = addToolBar(tr("&File"));

	actionFileOpen = new QAction(tr("&Open"), this);
	actionFileOCR = new QAction(tr("&OCR"), this);

	connect(actionFileOpen, SIGNAL(triggered()), this, SLOT(openFile()));
	connect(actionFileOCR, SIGNAL(triggered()), this, SLOT(startOCR()));

	menuFile->addAction(actionFileOpen);
	menuFile->addAction(actionFileOCR);

	toolBarFile->addAction(actionFileOpen);
	toolBarFile->addAction(actionFileOCR);
}

void MainWindow::openFile()
{
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Open Image"), ".",
		tr("Image files (*.png *.jpg *.bmp *.tiff)"));
    if (!fileName.isEmpty())
	{
		QTextCodec::setCodecForCStrings(QTextCodec::codecForName("gbk"));
		cvImage = cvLoadImage(fileName.toStdString().c_str());
		if(cvImage)
		{
			QImage* image = new QImage(QImage((uchar*)cvImage->imageData, cvImage->width, cvImage->height, QImage::Format_RGB888).rgbSwapped());
			imageWidget->setImage(image);
		}
	}
}


void MainWindow::startOCR()
{
	if(!cvImage)
	{
		QMessageBox::information(this, "OCR not started", "No Image Found!");
	}
	else
	{
		labelOCRText->setText("Starting OCR ...");
		labelOCRText->resize(400, 300);
		labelOCRText->show();

		char *outText;
		tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
		if (api->Init(NULL, "chi_sim")) {
			fprintf(stderr, "Could not initialize tesseract.\n");
			exit(1);
		}

		api->SetImage((uchar*)cvImage->imageData, cvImage->width, cvImage->height, cvImage->nChannels, cvImage->widthStep);
		// Get OCR result
		outText = api->GetUTF8Text();

		QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));
		labelOCRText->setText(outText);

		// Destroy used object and release memory
		api->End();
	}
}