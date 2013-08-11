#include <QtGui>
#include "MainWindow.h"
#include "ImageWidget.h"

#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

MainWindow::MainWindow()
{
	image = new QImage();
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
		tr("Image files (*.png)"));
    if (!fileName.isEmpty())
	{
		openImage(fileName);
		imageWidget->setImage(image);
	}
}

void MainWindow::openImage(const QString& fileName)
{
	image->load(fileName);
}

void MainWindow::startOCR()
{
	if(image->isNull())
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

		QByteArray ba;
		QBuffer buffer(&ba);
		buffer.open(QIODevice::WriteOnly);
		image->save(&buffer, "BMP");

		Pix *pix = pixReadMemBmp((const l_uint8*)ba.data(), ba.size());
		api->SetImage(pix);
		// Get OCR result
		outText = api->GetUTF8Text();
		labelOCRText->setText(outText);

		// Destroy used object and release memory
		api->End();
		//delete [] outText;
		pixDestroy(&pix);
	}
}