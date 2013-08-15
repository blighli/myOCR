#include "MainWindow.h"

#include <QtGui>
#include "ImageWidget.h"
#include "ImageAdapter.h"


MainWindow::MainWindow()
{
	cvImage = NULL;
	boxes = NULL;
	tessBaseAPI = NULL;
	m_bNewChop = false;

	resize(960, 600);

	imageWidget = new ImageWidget();
	textEdit = new QTextEdit();
	textEdit->setFixedWidth(200);
	QFont fontTextEdit = textEdit->font();
	fontTextEdit.setPixelSize(18);
	textEdit->setFont(fontTextEdit);

	QHBoxLayout* hbox = new QHBoxLayout();

	QScrollArea* scrollArea = new QScrollArea();
	scrollArea->setWidget(imageWidget);
	scrollArea->setWidgetResizable(true);
	hbox->addWidget(scrollArea,1);
	hbox->addWidget(textEdit);
	
	QWidget* centerWidget = new QWidget();
	centerWidget->setLayout(hbox);
	setCentralWidget(centerWidget);

	menuFile = menuBar()->addMenu(tr("File"));
	menuImage = menuBar()->addMenu(tr("Image"));
	menuHelp = menuBar()->addMenu(tr("Help"));

	toolBarFile = addToolBar(tr("File"));
	toolBarFile->setMovable(false);
	toolBarImage = addToolBar(tr("Image"));
	toolBarImage->setMovable(false);
	
	actionOpenImage = new QAction(tr("Open Image"), this);
	actionSaveImage = new QAction(tr("Save Image"), this);
	actionRecognizeText = new QAction(tr("Recognize Text"), this);
	actionProcessImage = new QAction(tr("Process Image"), this);
	actionAbout = new QAction(tr("About"), this);
	actionToggleMarks = new QAction(tr("Enable Marks"), this);
	actionToggleMarks->setCheckable(true);
	actionSaveMarks = new QAction(tr("Save Marks"), this);
	actionLoadMarks = new QAction(tr("Load Marks"), this);
	actionClearMarks = new QAction(tr("Clear Marks"), this);
	actionToggleMesure = new QAction(tr("Enable Mesure"), this);
	actionToggleMesure->setCheckable(true);
	actionToggleChinese = new QAction(tr("Enable Chinese"), this);
	actionToggleChinese->setCheckable(true);
	
	actionOpenImage->setIcon(QIcon(":/open.png"));
	actionSaveImage->setIcon(QIcon(":/save.png"));
	actionRecognizeText->setIcon(QIcon(":/ocr.png"));
	actionProcessImage->setIcon(QIcon(":/paint.png"));
	actionToggleMarks->setIcon(QIcon(":/new2.png"));
	actionSaveMarks->setIcon(QIcon(":/save2.png"));
	actionLoadMarks->setIcon(QIcon(":/load2.png"));
	actionClearMarks->setIcon(QIcon(":/clear2.png"));
	actionToggleMesure->setIcon(QIcon(":/mesure.png"));
	actionToggleChinese->setIcon(QIcon(":/chinese.png"));

	connect(actionOpenImage, SIGNAL(triggered()), this, SLOT(openImageFile()));
	connect(actionSaveImage, SIGNAL(triggered()), this, SLOT(saveImageFile()));
	connect(actionRecognizeText, SIGNAL(triggered()), this, SLOT(recognizeText()));
	connect(actionProcessImage, SIGNAL(triggered()), this, SLOT(processImage()));
	connect(actionToggleMarks, SIGNAL(triggered()), this, SLOT(enableMarks()));
	connect(actionSaveMarks, SIGNAL(triggered()), this, SLOT(saveMarks()));
	connect(actionLoadMarks, SIGNAL(triggered()), this, SLOT(loadMarks()));
	connect(actionClearMarks, SIGNAL(triggered()), this, SLOT(clearMarks()));
	connect(actionToggleMesure, SIGNAL(triggered()), this, SLOT(enableMesure()));
	connect(actionToggleChinese, SIGNAL(triggered()), this, SLOT(enableChinese()));
	
	menuFile->addAction(actionOpenImage);
	menuFile->addAction(actionSaveImage);
	menuImage->addAction(actionToggleMesure);
	menuImage->addSeparator();
	menuImage->addAction(actionToggleMarks);
	menuImage->addAction(actionSaveMarks);
	menuImage->addAction(actionLoadMarks);
	menuImage->addAction(actionClearMarks);
	menuImage->addSeparator();
	menuImage->addAction(actionProcessImage);
	menuImage->addAction(actionRecognizeText);
	menuImage->addSeparator();
	menuImage->addAction(actionToggleChinese);
	menuHelp->addAction(actionAbout);
	
	toolBarFile->addAction(actionOpenImage);
	toolBarFile->addAction(actionSaveImage);
	toolBarImage->addAction(actionToggleMesure);
	toolBarImage->addSeparator();
	toolBarImage->addAction(actionToggleMarks);
	toolBarImage->addAction(actionSaveMarks);
	toolBarImage->addAction(actionLoadMarks);
	toolBarImage->addAction(actionClearMarks);
	toolBarImage->addSeparator();
	toolBarImage->addAction(actionProcessImage);
	toolBarImage->addAction(actionRecognizeText);
	toolBarImage->addSeparator();
	toolBarImage->addAction(actionToggleChinese);
}

MainWindow::~MainWindow()
{
	if(cvImage)
	{
		cvReleaseImage(&cvImage);
	}
	if(tessBaseAPI)
	{
		tessBaseAPI->End();
	}
}

void MainWindow::openImageFile()
{
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Open Image"), ".",
		tr("Image files (*.png *.jpg *.bmp *.tiff)"));
    if (!fileName.isEmpty())
	{
		QTextCodec::setCodecForCStrings(QTextCodec::codecForName("gbk"));//解决中文路径的问题
		if(cvImage)
		{
			cvReleaseImage(&cvImage);
		}
		cvImage = cvLoadImage(fileName.toStdString().c_str());
		if(cvImage)
		{
			QImage* image = ImageAdapter::IplImage2QImage(cvImage);
			imageWidget->setImage(image);
		}
	}
}

void MainWindow::saveImageFile()
{
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Save Image"), ".",
		tr("Image files (*.png *.jpg *.bmp *.tiff)"));
    if (!fileName.isEmpty())
	{
		QTextCodec::setCodecForCStrings(QTextCodec::codecForName("gbk"));//解决中文路径的问题
		if(cvImage)
		{
			cvSaveImage(fileName.toStdString().c_str(), cvImage);
		}
	}
}

void MainWindow::enableMesure()
{
	imageWidget->allowTrack(actionToggleMesure->isChecked());
}

void MainWindow::enableChinese()
{
	
}

void MainWindow::enableMarks()
{
	m_bNewChop = actionToggleMarks->isChecked();
	imageWidget->allowNewChop(m_bNewChop);
}

void MainWindow::saveMarks()
{
	QVector<QRect>* chops = imageWidget->getChops();
	if(chops->size() == 0)
	{
		QMessageBox msgBox;
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.setText(tr("No chops exists.\nPlease add some chops before save."));
		msgBox.exec();
		return;
	}
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Save Chops"), ".",
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
		xmlWriter->writeStartElement("chops");

		for(int i=0; i<chops->size(); i++)
		{
			QRect rect = chops->at(i);
			xmlWriter->writeStartElement("chop");

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

void MainWindow::loadMarks()
{
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Load Chops"), ".",
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

		QVector<QRect>* chops = imageWidget->getChops();
		chops->clear();

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
				if(xml.name() == "chops")
				{
					continue;
				}
				if(xml.name() == "chop")
				{
					QRect rect;
					QXmlStreamAttributes attributes = xml.attributes();
					rect.setX(attributes.value("x").toString().toInt());
					rect.setY(attributes.value("y").toString().toInt());
					rect.setWidth(attributes.value("w").toString().toInt());
					rect.setHeight(attributes.value("h").toString().toInt());

					chops->push_back(rect);
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

void MainWindow::clearMarks()
{
	imageWidget->getChops()->clear();
	imageWidget->update();
}

void MainWindow::processImage()
{
	if(cvImage == NULL)
	{
		QMessageBox msgBox;
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.setText(tr("No image loaded.\nPlease load an image file before OpenCV."));
		msgBox.exec();
		return;
	}

	IplImage* grayImage = cvCreateImage(cvGetSize(cvImage), 8, 1);
	if(cvImage->nChannels == 3)
	{
		cvCvtColor(cvImage, grayImage, CV_RGB2GRAY);
		cvReleaseImage(&cvImage);
		cvImage = grayImage;
	}
	
	//cvAdaptiveThreshold(cvImage, cvImage, 255);

	QImage* image = ImageAdapter::IplImage2QImage(cvImage);
	imageWidget->setImage(image);
}

void MainWindow::recognizeText()
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
		if(tessBaseAPI == NULL)
		{
			tessBaseAPI = new tesseract::TessBaseAPI(); 
		}
		//或者在Init函数中设置datapath
		if(actionToggleChinese->isChecked())
		{
			if (tessBaseAPI->Init(NULL, "chi_sim")) {
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
			if (tessBaseAPI->Init(NULL, "eng")) {
				QMessageBox msgBox;
				msgBox.setIcon(QMessageBox::Critical);
				msgBox.setText(tr("Could not initialize tesseract: english"));
				msgBox.exec();
				return;
			}
		}

		tessBaseAPI->SetImage((uchar*)cvImage->imageData, cvImage->width, cvImage->height, cvImage->nChannels, cvImage->widthStep);

		QVector<QRect>* chops = imageWidget->getChops();
		if(chops->size() == 0)
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
			textEdit->setText(outText);
		}
		else
		{
			textEdit->clear();

			for(int i=0;i<chops->size();i++)
			{
				QRect rect = chops->at(i);
				tessBaseAPI->SetRectangle(rect.x(), rect.y(), rect.width(), rect.height());
				char* ocrResult = tessBaseAPI->GetUTF8Text();
				int conf = tessBaseAPI->MeanTextConf();
				QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));
				textEdit->setText(QString("%1%2%3").arg(textEdit->toPlainText(),"\n",ocrResult));
			}
		}
	}
}