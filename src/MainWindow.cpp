#include "MainWindow.h"

#include <QtGui>
#include <leptonica/allheaders.h>
#include <tesseract/baseapi.h>

#include "ImageWidget.h"


MainWindow::MainWindow()
{
	cvImage = NULL;

	resize(800, 600);

	imageWidget = new ImageWidget();
	textEdit = new QTextEdit();
	textEdit->setMinimumWidth(200);
	textEdit->setMaximumWidth(600);
	QFont fontTextEdit = textEdit->font();
	fontTextEdit.setPixelSize(18);
	textEdit->setFont(fontTextEdit);

	QHBoxLayout* hbox = new QHBoxLayout();
	hbox->addWidget(imageWidget,2);
	hbox->addWidget(textEdit,1);
	
	QWidget* centerWidget = new QWidget();
	centerWidget->setLayout(hbox);

	this->setCentralWidget(centerWidget);

	menuFile = menuBar()->addMenu(tr("&File"));
	toolBarFile = addToolBar(tr("&File"));

	actionFileOpen = new QAction(tr("&Open"), this);
	actionFileOCR = new QAction(tr("&OCR"), this);

	actionFileOpen->setIcon(QIcon(":/open.png"));
	actionFileOCR->setIcon(QIcon(":/ocr.png"));

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
		QTextCodec::setCodecForCStrings(QTextCodec::codecForName("gbk"));//解决中文路径的问题
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
		QMessageBox msgBox;
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.setText(tr("No image loaded.\nPlease load an image file before OCR."));
		msgBox.exec();
		return;
	}
	else
	{
		//设置环境变量TESSDATA_PREFIX
		tesseract::TessBaseAPI* api = new tesseract::TessBaseAPI(); 
		//或者在Init函数中设置datapath
		if (api->Init(NULL, "chi_sim")) {
			QMessageBox msgBox;
			msgBox.setIcon(QMessageBox::Critical);
			msgBox.setText(tr("Could not initialize tesseract."));
			msgBox.exec();
			return;
		}

		api->SetImage((uchar*)cvImage->imageData, cvImage->width, cvImage->height, cvImage->nChannels, cvImage->widthStep);

		char *outText = api->GetUTF8Text();

		QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));
		textEdit->setText(outText);

		// Destroy used object and release memory
		api->End();
	}
}