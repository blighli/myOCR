#include "MainWindow.h"

#include <QtGui>
#include "ImageWidget.h"
#include "Share/ImageAdapter.h"
#include "Share/AppInfo.h"
#include "Share/AbbyyOCR.h"
#include "CubeWidget.h"
#include "ParamWidget.h"
#include "Share/ImageProcess.h"
#include "Share/TesseractOCR.h"
#include "OCRWidget.h"
#include "twain/qtwain.h"
#include "twain/dib.h"


MainWindow::MainWindow()
{
	mTwain = new QTwain();

	mOCRMasks = new std::vector<OCRMask>();
	mImageProcess = new ImageProcess();
	mAbbyyOCR = NULL;
	mTesseractOCR = NULL;

	cubeWidget = NULL;
	paramWidget = NULL;

	buildUI();
}

MainWindow::~MainWindow()
{
	if(mOCRMasks)
	{
		delete mOCRMasks;
	}
	if(mImageProcess)
	{
		delete mImageProcess;
	}

	if(mTesseractOCR)
	{
		delete mTesseractOCR;
	}

	if(mAbbyyOCR)
	{
		delete mAbbyyOCR;
	}

	if(cubeWidget)
	{
		delete cubeWidget;
	}

	if(paramWidget)
	{
		delete paramWidget;
	}
}

void MainWindow::buildUI()
{
	resize(960, 720);

	//build image widget
	imageWidget = new ImageWidget();
	imageWidget->setMasks(mOCRMasks);

	QScrollArea* scrollArea = new QScrollArea();
	scrollArea->setWidget(imageWidget);
	scrollArea->setWidgetResizable(true);

	ocrWidget = new OCRWidget();
	ocrWidget->setMasks(mOCRMasks);

	paramWidget = new ParamWidget();
	connect(paramWidget, SIGNAL(process()), this, SLOT(processImage()));

	//build text widget
	textEdit = new QTextEdit();
	textEdit->setFixedWidth(400);
	QFont fontTextEdit = textEdit->font();
	fontTextEdit.setPixelSize(18);
	textEdit->setFont(fontTextEdit);

	//layout the main ui

	QTabWidget* tabWidget = new QTabWidget();
	tabWidget->addTab(ocrWidget, "Masks");
	tabWidget->addTab(paramWidget, "Image Process");
	tabWidget->addTab(textEdit, "Text Recognize");


	connect(imageWidget, SIGNAL(maskChanged()), ocrWidget, SLOT(update()));

	QHBoxLayout* hbox = new QHBoxLayout();
	hbox->addWidget(scrollArea,1);
	hbox->addWidget(tabWidget);
	QWidget* centerWidget = new QWidget();
	centerWidget->setLayout(hbox);
	setCentralWidget(centerWidget);


	//build the main menu
	menuFile = menuBar()->addMenu(tr("File"));
	menuImage = menuBar()->addMenu(tr("Image"));
	menuHelp = menuBar()->addMenu(tr("Help"));


	//build the main toolbar
	toolBarFile = addToolBar(tr("File"));
	toolBarFile->setMovable(false);
	toolBarImage = addToolBar(tr("Image"));
	toolBarImage->setMovable(false);


	//build actions
	actionOpenImage = new QAction(tr("Open Image"), this);
	actionSaveImage = new QAction(tr("Save Image"), this);
	actionRecognizeText = new QAction(tr("Recognize Text"), this);
	actionProcessImage = new QAction(tr("Process Image"), this);
	actionAbout = new QAction(tr("About"), this);
	actionEnableMasks = new QAction(tr("Enable Masks"), this);
	actionEnableMasks->setCheckable(true);
	actionSaveMasks = new QAction(tr("Save Masks"), this);
	actionLoadMasks = new QAction(tr("Load Masks"), this);
	actionClearMasks = new QAction(tr("Clear Masks"), this);
	actionEnableMesure = new QAction(tr("Enable Mesure"), this);
	actionEnableMesure->setCheckable(true);
	actionEnableChinese = new QAction(tr("Enable Chinese"), this);
	actionEnableChinese->setCheckable(true);
	actionShowCube = new QAction(tr("Show Cube"), this);
	actionScan = new QAction(tr("Scan"), this);

	actionOpenImage->setIcon(QIcon(":/open.png"));
	actionSaveImage->setIcon(QIcon(":/save.png"));
	actionScan->setIcon(QIcon(":/scan.png"));
	actionRecognizeText->setIcon(QIcon(":/ocr.png"));
	actionProcessImage->setIcon(QIcon(":/paint.png"));
	actionEnableMasks->setIcon(QIcon(":/new2.png"));
	actionSaveMasks->setIcon(QIcon(":/save2.png"));
	actionLoadMasks->setIcon(QIcon(":/load2.png"));
	actionClearMasks->setIcon(QIcon(":/clear2.png"));
	actionEnableMesure->setIcon(QIcon(":/mesure.png"));
	actionEnableChinese->setIcon(QIcon(":/chinese.png"));
	actionShowCube->setIcon(QIcon(":/cube.png"));

	connect(actionOpenImage, SIGNAL(triggered()), this, SLOT(openImageFile()));
	connect(actionSaveImage, SIGNAL(triggered()), this, SLOT(saveImageFile()));
	connect(actionRecognizeText, SIGNAL(triggered()), this, SLOT(recognizeText()));
	connect(actionProcessImage, SIGNAL(triggered()), this, SLOT(showParamWidget()));
	connect(actionEnableMasks, SIGNAL(triggered()), this, SLOT(enableMasks()));
	connect(actionSaveMasks, SIGNAL(triggered()), this, SLOT(saveMasks()));
	connect(actionLoadMasks, SIGNAL(triggered()), this, SLOT(loadMasks()));
	connect(actionClearMasks, SIGNAL(triggered()), this, SLOT(clearMasks()));
	connect(actionEnableMesure, SIGNAL(triggered()), this, SLOT(enableMesure()));
	connect(actionEnableChinese, SIGNAL(triggered()), this, SLOT(enableChinese()));
	connect(actionShowCube, SIGNAL(triggered()), this, SLOT(showCube()));
	connect(actionScan, SIGNAL(triggered()), this, SLOT(scanImage()));

	menuFile->addAction(actionScan);
	menuFile->addAction(actionOpenImage);
	menuFile->addAction(actionSaveImage);
	menuImage->addAction(actionEnableMesure);
	menuImage->addSeparator();
	menuImage->addAction(actionEnableMasks);
	menuImage->addAction(actionSaveMasks);
	menuImage->addAction(actionLoadMasks);
	menuImage->addAction(actionClearMasks);
	menuImage->addSeparator();
	menuImage->addAction(actionProcessImage);
	menuImage->addAction(actionRecognizeText);
	menuImage->addSeparator();
	menuImage->addAction(actionEnableChinese);
	menuImage->addAction(actionShowCube);
	menuHelp->addAction(actionAbout);

	toolBarFile->addAction(actionScan);
	toolBarFile->addAction(actionOpenImage);
	toolBarFile->addAction(actionSaveImage);
	toolBarImage->addSeparator();
	toolBarImage->addAction(actionEnableMesure);
	toolBarImage->addSeparator();
	toolBarImage->addAction(actionEnableMasks);
	toolBarImage->addAction(actionSaveMasks);
	toolBarImage->addAction(actionLoadMasks);
	toolBarImage->addAction(actionClearMasks);
	toolBarImage->addAction(actionProcessImage);
	toolBarImage->addSeparator();
	toolBarImage->addAction(actionRecognizeText);
	//toolBarImage->addSeparator();
	//toolBarImage->addAction(actionEnableChinese);
	//toolBarImage->addAction(actionShowCube);

	connect(mTwain, SIGNAL(dibAcquired(CDIB*)), this, SLOT(onDibAcquired(CDIB*))); 

	//QComboBox* recognizeMethods = new QComboBox();
	//recognizeMethods->addItem(tr("Chinese and English"),QVariant(0));
	//recognizeMethods->addItem(tr("Number Only"),QVariant(1));
	//recognizeMethods->addItem(tr("ABBYY"),QVariant(2));
	//toolBarImage->addWidget(recognizeMethods);
}

void MainWindow::openImageFile()
{
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Open Image"), AppInfo::instance()->appDir().c_str(),
		tr("Image files (*.png *.jpg *.bmp *.tiff)"));
    if (!fileName.isEmpty())
	{
		IplImage* cvImage = cvLoadImage(fileName.toLocal8Bit().constData());
		if(cvImage)
		{
			mImageProcess->setImage(cvImage);
			QImage* qtImage= ImageAdapter::IplImage2QImage(cvImage);
			imageWidget->setImage(qtImage);
		}
	}
}

void MainWindow::saveImageFile()
{
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Save Image"), AppInfo::instance()->appDir().c_str(),
		tr("Image files (*.png *.jpg *.bmp *.tiff)"));
    if (!fileName.isEmpty())
	{
		IplImage* cvImage = mImageProcess->getProcessedImage();
		if(cvImage)
		{
			cvSaveImage(fileName.toLocal8Bit().constData(), cvImage);
		}
	}
}

void MainWindow::scanImage()
{
	mTwain->selectSource();
	mTwain->acquire();
}

void MainWindow::showEvent(QShowEvent* thisEvent)
{
	mTwain->setParent(this);
}

bool MainWindow::winEvent(MSG* pMsg, long* result)
{
	mTwain->processMessage(*pMsg); 
	return false;
}

void MainWindow::onDibAcquired(CDIB* pDib)
{
	if(pDib)
	{
		int width = pDib->Width();
		int height = pDib->Height();
		int depth = pDib->GetBitmapInfo()->bmiHeader.biBitCount == 1?IPL_DEPTH_1U : IPL_DEPTH_8U;
		int nChannels = 3;

		IplImage* cvImage = cvCreateImage(cvSize(width, height), depth, nChannels);
		memcpy(cvImage->imageData, pDib->GetLinePtr(0), width * height * nChannels);
		//Convert dib to iplImage herer!!

		if(cvImage)
		{
			mImageProcess->setImage(cvImage);
			QImage* qtImage= ImageAdapter::IplImage2QImage(cvImage);
			imageWidget->setImage(qtImage);
		}
		delete pDib;
	}
} 

void MainWindow::enableMesure()
{
	imageWidget->enableMesure(actionEnableMesure->isChecked());
}

void MainWindow::enableChinese()
{
	
}

void MainWindow::enableMasks()
{
	bool enableMasks = actionEnableMasks->isChecked();
	imageWidget->enableMasks(enableMasks);
}

void MainWindow::saveMasks()
{
	if(mOCRMasks->size() == 0)
	{
		QMessageBox msgBox;
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.setText(tr("No masks exists.\nPlease add some masks before save."));
		msgBox.exec();
		return;
	}
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Save Masks"), AppInfo::instance()->appDir().c_str(),
		tr("XML files (*.xml)"));
    if (!fileName.isEmpty())
	{
		QFile file(fileName);
		if(!file.open(QIODevice::WriteOnly))
		{
			QMessageBox msgBox;
			msgBox.setIcon(QMessageBox::Warning);
			msgBox.setText(tr("Cannot open file to write.\nMaybe this file is read only."));
			msgBox.exec();
			return;
		}
		
		QXmlStreamWriter* xmlWriter = new QXmlStreamWriter();
		xmlWriter->setDevice(&file);
		xmlWriter->writeStartDocument();
		xmlWriter->writeStartElement("masks");

		for(int i=0; i<mOCRMasks->size(); i++)
		{
			OCRMask mask = mOCRMasks->at(i);
			xmlWriter->writeStartElement("mask");

			xmlWriter->writeAttribute("key", QString::fromLocal8Bit(mask.key.c_str()));
			xmlWriter->writeAttribute("x", QString::number(mask.rect.x));
			xmlWriter->writeAttribute("y", QString::number(mask.rect.y));
			xmlWriter->writeAttribute("w", QString::number(mask.rect.width));
			xmlWriter->writeAttribute("h", QString::number(mask.rect.height));

			xmlWriter->writeEndElement();
		}

		xmlWriter->writeEndElement();
		xmlWriter->writeEndDocument();
        delete xmlWriter;
	}
}

void MainWindow::loadMasks()
{
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Load Masks"), AppInfo::instance()->appDir().c_str(),
		tr("XML files (*.xml)"));
    if (!fileName.isEmpty())
	{
		QFile file(fileName);
		if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QMessageBox msgBox;
			msgBox.setIcon(QMessageBox::Warning);
			msgBox.setText(tr("Cannot open file to read.\nMaybe this file does not exist."));
			msgBox.exec();
			return;
		}

		mOCRMasks->clear();

		QXmlStreamReader xml;
		xml.setDevice(&file);

		while(!xml.atEnd() && !xml.hasError())
		{
			QXmlStreamReader::TokenType token = xml.readNext();
			if(token == QXmlStreamReader::StartDocument)
			{
				continue;
			}
			if(token == QXmlStreamReader::StartElement)
			{
				if(xml.name() == "masks")
				{
					continue;
				}
				if(xml.name() == "mask")
				{
					QXmlStreamAttributes attributes = xml.attributes();
					OCRMask mask;
					mask.key = attributes.value("key").toString().toLocal8Bit();
					mask.rect.x = (attributes.value("x").toString().toInt());
					mask.rect.y = (attributes.value("y").toString().toInt());
					mask.rect.width = (attributes.value("w").toString().toInt());
					mask.rect.height = (attributes.value("h").toString().toInt());
					mOCRMasks->push_back(mask);
				}
			}
		}
		if(xml.hasError())
		{
			QMessageBox msgBox;
			msgBox.setIcon(QMessageBox::Warning);
			msgBox.setText(tr("Parse XML failed.\nMaybe this file is not the right one."));
			msgBox.exec();
		}
		xml.clear();
		imageWidget->update();
		ocrWidget->update();
	}
}

void MainWindow::clearMasks()
{
	mOCRMasks->clear();
	imageWidget->update();
	ocrWidget->update();
}

void MainWindow::showParamWidget()
{
	processImage();
}



void MainWindow::processImage()
{
	if(mImageProcess->getOriginalImage() == NULL)
	{
		QMessageBox msgBox;
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.setText(tr("No image loaded.\nPlease load an image file before OpenCV."));
		msgBox.exec();
		return;
	}

	ImageProcessParam param;

	param.debug = true;
	
	param.gray = paramWidget->grayGroup->isChecked();

	param.threshold = paramWidget->threshold->isChecked();
	param.thresholdBlockSize = paramWidget->thresholdBlockSize->value();
	param.thresholdMeanBias = paramWidget->thresholdMeanBias->value();

	param.hough = paramWidget->houghGroup->isChecked();
	param.houghRho = paramWidget->houghRho->value();
	param.houghTheta = paramWidget->houghTheta->value();
	param.houghThreshold = paramWidget->houghThreshold->value();
	param.houghParam1 = paramWidget->houghParam1->value();
	param.houghParam2 = paramWidget->houghParam2->value();

	param.corner = paramWidget->cornerGroup->isChecked();
	param.cornerDegree = paramWidget->cornerDegree->value();
	param.cornerGap = paramWidget->cornerGap->value();

	param.background = paramWidget->backgroundGroup->isChecked();

	param.normalize = paramWidget->normalizeGroup->isChecked();
	param.normalizeTopMargin = paramWidget->normalizeTopMargin->value();
	param.normalizeSideMargin = paramWidget->normalizeSideMargin->value();
	param.normalizeWidth = paramWidget->normalizeWidth->value();
	param.normalizeHeight = paramWidget->normalizeHeight->value();

	param.blueText =  paramWidget->blueTextGroup->isChecked();
	param.blueTextMin = paramWidget->blueTextMin->value();
	param.blueTextMax = paramWidget->blueTextMax->value();

	mImageProcess->setMasks(mOCRMasks);
	mImageProcess->run(&param);

	IplImage* cvImage = mImageProcess->getProcessedImage();
	QImage* qtImage = ImageAdapter::IplImage2QImage(cvImage);
	imageWidget->setImage(qtImage);
}

void MainWindow::recognizeText()
{
	IplImage* cvImage = mImageProcess->getProcessedImage();
	if(!cvImage)
	{
		QMessageBox msgBox;
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.setText(tr("No image loaded.\nPlease load an image file before OCR."));
		msgBox.exec();
		return;
	}

	/*
	if(mAbbyyOCR == NULL)
	{
		mAbbyyOCR = new AbbyyOCR();
	}
	mAbbyyOCR->setImage(cvImage);
	mAbbyyOCR->setMasks(masks);
	QString abbyyText = mAbbyyOCR->recognizeText();
	*/

	if(mTesseractOCR == NULL)
	{
		mTesseractOCR = new TesseractOCR();
	}
	bool initSuccess = false;
	if(actionEnableChinese->isChecked())
	{
		initSuccess = mTesseractOCR->init(TesseractOCR::TESSERACTOCR_CHINESE);
	}
	else
	{
		initSuccess = mTesseractOCR->init(TesseractOCR::TESSERACTOCR_ENGLISH);
	}
	if(initSuccess == false)
	{
		QMessageBox msgBox;
		msgBox.setIcon(QMessageBox::Critical);
		msgBox.setText(tr("Can not initialize tesseract"));
		msgBox.exec();
		return;
	}
	mTesseractOCR->setImage(cvImage);
	mTesseractOCR->setMasks(mOCRMasks);

	QString tesseractText = QString::fromLocal8Bit( mTesseractOCR->recognizeText().c_str() );

	ocrWidget->update();
	textEdit->clear();
	//textEdit->setText(QString("%1\n%2\n%3\n%4").arg("Abbyy OCR:", abbyyText, "Tesseract OCR:", tesseractText));
	textEdit->setText(QString("%1").arg(tesseractText));

	int rectCount = mTesseractOCR->getBoxes()->size();
	QRect* rects = new QRect[rectCount];
	for(int i = 0; i< rectCount; i++)
	{
		rects[i].setX(mTesseractOCR->getBoxes()->at(i).x);
		rects[i].setY(mTesseractOCR->getBoxes()->at(i).y);
		rects[i].setWidth(mTesseractOCR->getBoxes()->at(i).width);
		rects[i].setHeight(mTesseractOCR->getBoxes()->at(i).height);
	}
	imageWidget->setBoxes(rects, rectCount);
	
}

void MainWindow::showCube()
{
	if(cubeWidget == NULL)
	{
		cubeWidget = new CubeWidget();
	}
	IplImage* cvImage = mImageProcess->getProcessedImage();
	cubeWidget->setImage(cvImage);
	cubeWidget->show();
	cubeWidget->raise();
	cubeWidget->activateWindow();
}
