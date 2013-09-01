#include "MainWindow.h"

#include <QtGui>
#include "ImageWidget.h"
#include "ImageAdapter.h"
#include "AppInfo.h"
#include "AbbyyOCR.h"
#include "CubeWidget.h"
#include "ParamWidget.h"
#include "ImageProcess.h"


MainWindow::MainWindow()
{
	mImageProcess = new ImageProcess();

	tessBaseAPI = NULL;
	mAbbyyOCR = NULL;
	
	boxes = NULL;

	cubeWidget = NULL;
	paramWidget = NULL;

	buildUI();
}

MainWindow::~MainWindow()
{
	if(mImageProcess)
	{
		delete mImageProcess;
	}

	if(tessBaseAPI)
	{
		tessBaseAPI->End();
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
	QScrollArea* scrollArea = new QScrollArea();
	scrollArea->setWidget(imageWidget);
	scrollArea->setWidgetResizable(true);

	//build text widget
	textEdit = new QTextEdit();
	textEdit->setFixedWidth(400);
	QFont fontTextEdit = textEdit->font();
	fontTextEdit.setPixelSize(18);
	textEdit->setFont(fontTextEdit);

	//layout the main ui
	QHBoxLayout* hbox = new QHBoxLayout();
	hbox->addWidget(scrollArea,1);
	hbox->addWidget(textEdit);
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

	actionOpenImage->setIcon(QIcon(":/open.png"));
	actionSaveImage->setIcon(QIcon(":/save.png"));
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

	toolBarFile->addAction(actionOpenImage);
	toolBarFile->addAction(actionSaveImage);
	toolBarImage->addAction(actionEnableMesure);
	toolBarImage->addSeparator();
	toolBarImage->addAction(actionEnableMasks);
	toolBarImage->addAction(actionSaveMasks);
	toolBarImage->addAction(actionLoadMasks);
	toolBarImage->addAction(actionClearMasks);
	toolBarImage->addSeparator();
	toolBarImage->addAction(actionProcessImage);
	toolBarImage->addAction(actionRecognizeText);
	toolBarImage->addSeparator();
	toolBarImage->addAction(actionEnableChinese);
	toolBarImage->addAction(actionShowCube);

	//QComboBox* recognizeMethods = new QComboBox();
	//recognizeMethods->addItem(tr("Chinese and English"),QVariant(0));
	//recognizeMethods->addItem(tr("Number Only"),QVariant(1));
	//recognizeMethods->addItem(tr("ABBYY"),QVariant(2));
	//toolBarImage->addWidget(recognizeMethods);
}

void MainWindow::openImageFile()
{
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Open Image"), AppInfo::instance()->appDir(),
		tr("Image files (*.png *.jpg *.bmp *.tiff)"));
    if (!fileName.isEmpty())
	{
		QTextCodec::setCodecForCStrings(QTextCodec::codecForName("gbk"));//解决中文路径的问题
		IplImage* cvImage = cvLoadImage(fileName.toStdString().c_str());
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
		tr("Save Image"), AppInfo::instance()->appDir(),
		tr("Image files (*.png *.jpg *.bmp *.tiff)"));
    if (!fileName.isEmpty())
	{
		QTextCodec::setCodecForCStrings(QTextCodec::codecForName("gbk"));//解决中文路径的问题
		IplImage* cvImage = mImageProcess->getProcessedImage();
		if(cvImage)
		{
			cvSaveImage(fileName.toStdString().c_str(), cvImage);
		}
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
	QVector<QRect>* masks = imageWidget->getMasks();
	if(masks->size() == 0)
	{
		QMessageBox msgBox;
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.setText(tr("No masks exists.\nPlease add some masks before save."));
		msgBox.exec();
		return;
	}
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Save Masks"), AppInfo::instance()->appDir(),
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

		for(int i=0; i<masks->size(); i++)
		{
			QRect rect = masks->at(i);
			xmlWriter->writeStartElement("mask");

			xmlWriter->writeAttribute("x", QString::number(rect.x()));
			xmlWriter->writeAttribute("y", QString::number(rect.y()));
			xmlWriter->writeAttribute("w", QString::number(rect.width()));
			xmlWriter->writeAttribute("h", QString::number(rect.height()));

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
		tr("Load Masks"), AppInfo::instance()->appDir(),
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

		QVector<QRect>* masks = imageWidget->getMasks();
		masks->clear();

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
					QRect rect;
					QXmlStreamAttributes attributes = xml.attributes();
					rect.setX(attributes.value("x").toString().toInt());
					rect.setY(attributes.value("y").toString().toInt());
					rect.setWidth(attributes.value("w").toString().toInt());
					rect.setHeight(attributes.value("h").toString().toInt());

					masks->push_back(rect);
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
	}
}

void MainWindow::clearMasks()
{
	imageWidget->getMasks()->clear();
	imageWidget->update();
}

void MainWindow::showParamWidget()
{
	if(paramWidget == NULL)
	{
		paramWidget = new ParamWidget();
		connect(paramWidget, SIGNAL(process()), this, SLOT(processImage()));
	}
	paramWidget->show();
	paramWidget->raise();
	paramWidget->activateWindow();
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

	param.debug = paramWidget->debugGroup->isChecked();
	
	param.useGray = paramWidget->grayGroup->isChecked();

	param.useCanny = paramWidget->cannyGroup->isChecked();
	param.cannyThreshold1 = paramWidget->cannyThreshold1->value();
	param.cannyThreshold2 = paramWidget->cannyThreshold2->value();
	
	param.useDilate = paramWidget->dilateGroup->isChecked();
	param.dilateIter = paramWidget->dilateIter->value();
	param.useErode = paramWidget->erodeGroup->isChecked();
	param.erodeIter = paramWidget->erodeIter->value();

	param.useHough = paramWidget->houghGroup->isChecked();
	param.houghRho = paramWidget->houghRho->value();
	param.houghTheta = paramWidget->houghTheta->value();
	param.houghThreshold = paramWidget->houghThreshold->value();
	param.houghParam1 = paramWidget->houghParam1->value();
	param.houghParam2 = paramWidget->houghParam2->value();

	param.useCombine = paramWidget->combineGroup->isChecked();
	param.combineTheta = paramWidget->combineTheta->value();
	param.combineRho = paramWidget->combineRho->value();
	param.combineDistance = paramWidget->combineDistance->value();

	param.useRectangle = paramWidget->rectangleGroup->isChecked();
	param.rectangleHMinLength = paramWidget->rectangleHMinLength->value();
	param.rectangleVMinLength = paramWidget->rectangleVMinLength->value();
	param.rectangleTop = paramWidget->rectangleTop->value();
	param.rectangleBottom = paramWidget->rectangleBottom->value();

	param.useBackGround = paramWidget->backGroundGroup->isChecked();

	param.useNormalize = paramWidget->normalizeGroup->isChecked();
	param.normalizeTop = paramWidget->normalizeTop->value();
	param.normalizeWidth = paramWidget->normalizeWidth->value();
	param.normalizeHeight = paramWidget->normalizeHeight->value();

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
	QVector<QRect>* masks = imageWidget->getMasks();

	if(mAbbyyOCR == NULL)
	{
		mAbbyyOCR = new AbbyyOCR();
	}
	mAbbyyOCR->setImage(cvImage);

	mAbbyyOCR->setMasks(masks);

	QString ret = mAbbyyOCR->recognizeText();
	textEdit->clear();
	textEdit->setText(QString("%1%2%3").arg(textEdit->toPlainText(),"Abbyy Result:\n",ret));
	textEdit->setText(QString("%1%2%3").arg(textEdit->toPlainText(),"\n","/////////////////////////\n\nTesseract Result:\n"));


	//设置环境变量TESSDATA_PREFIX
	if(tessBaseAPI == NULL)
	{
		tessBaseAPI = new tesseract::TessBaseAPI(); 
	}
	//或者在Init函数中设置datapath
	QString tessdata = AppInfo::instance()->appDir();
	if(actionEnableChinese->isChecked())
	{
		if (tessBaseAPI->Init(tessdata.toAscii(), "chi_sim+eng")) {
			QMessageBox msgBox;
			msgBox.setIcon(QMessageBox::Critical);
			msgBox.setText(tr("Could not initialize tesseract: chinese"));
			msgBox.exec();
			return;
		}
		if(true)
		{
			tessBaseAPI->SetVariable("chop_enable", "T");
			tessBaseAPI->SetVariable("use_new_state_cost", "F");
			tessBaseAPI->SetVariable("segment_segcost_rating", "F");
			tessBaseAPI->SetVariable("enable_new_segsearch", "0");
			tessBaseAPI->SetVariable("language_model_ngram_on", "0");
			tessBaseAPI->SetVariable("textord_force_make_prop_words", "F");
		}
	}
	else
	{
		if (tessBaseAPI->Init(tessdata.toAscii(), "eng")) {
			QMessageBox msgBox;
			msgBox.setIcon(QMessageBox::Critical);
			msgBox.setText(tr("Could not initialize tesseract: english"));
			msgBox.exec();
			return;
		}
		if(true)
		{
			tessBaseAPI->SetVariable("tessedit_char_whitelist", "0123456789.+-*/<>");
		}
	}

	tessBaseAPI->SetImage((uchar*)cvImage->imageData, cvImage->width, cvImage->height, cvImage->nChannels, cvImage->widthStep);

	if(masks->size() == 0)
	{

		boxes = tessBaseAPI->GetComponentImages(tesseract::RIL_SYMBOL, true, NULL, NULL);
		
		QRect* rects = new QRect[boxes->n];
		for(int i = 0; i< boxes->n; i++)
		{
			rects[i].setX(boxes->box[i]->x);
			rects[i].setY(boxes->box[i]->y);
			rects[i].setWidth(boxes->box[i]->w);
			rects[i].setHeight(boxes->box[i]->h);
		}
		imageWidget->setBoxes(rects, boxes->n);

		char *outText = tessBaseAPI->GetUTF8Text();

		QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));
		textEdit->setText(QString("%1%2").arg(textEdit->toPlainText(),outText));
	}
	else
	{
		QVector<QRect> rectBoxes;
		for(int i=0;i<masks->size();i++)
		{
			QRect rect = masks->at(i);
			tessBaseAPI->SetRectangle(rect.x(), rect.y(), rect.width(), rect.height());

			boxes = tessBaseAPI->GetComponentImages(tesseract::RIL_SYMBOL, true, NULL, NULL);
			if(boxes)
			{
				for(int i = 0; i< boxes->n; i++)
				{
					QRect rectBox(boxes->box[i]->x, boxes->box[i]->y, boxes->box[i]->w, boxes->box[i]->h);
					rectBox.translate(rect.x(), rect.y());
					rectBoxes.append(rectBox);
				}

				char* ocrResult = tessBaseAPI->GetUTF8Text();
				QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));
				QString txt = QString(ocrResult);
				txt.replace(" ","");
				txt.replace("\n\n","\n");
				textEdit->setText(QString("%1%2").arg(textEdit->toPlainText(),txt));
			}
		}

		int rectCount = rectBoxes.size();
		QRect* rects = new QRect[rectCount];
		for(int i = 0; i< rectCount; i++)
		{
			rects[i] = rectBoxes.at(i);
		}
		imageWidget->setBoxes(rects, rectCount);
	}
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
