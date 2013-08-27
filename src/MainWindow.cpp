#include "MainWindow.h"

#include <QtGui>
#include "ImageWidget.h"
#include "ImageAdapter.h"
#include "AppInfo.h"
#include "AbbyyOCR.h"
#include "CubeWidget.h"
#include "getContourAndCorrect.h"
#include "ParamWidget.h"


MainWindow::MainWindow()
{
	mImage = NULL;
	mOriginalImage = NULL;
	tessBaseAPI = NULL;
	mAbbyyOCR = NULL;
	boxes = NULL;
	mEnableMasks = false;

	cubeWidget = NULL;
	paramWidget = NULL;

	resize(960, 600);


	imageWidget = new ImageWidget();
	textEdit = new QTextEdit();
	textEdit->setFixedWidth(300);
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

MainWindow::~MainWindow()
{
	if(mImage)
	{
		cvReleaseImage(&mImage);
	}
	if(mOriginalImage)
	{
		cvReleaseImage(&mOriginalImage);
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

void MainWindow::openImageFile()
{
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Open Image"), AppInfo::instance()->appDir(),
		tr("Image files (*.png *.jpg *.bmp *.tiff)"));
    if (!fileName.isEmpty())
	{
		QTextCodec::setCodecForCStrings(QTextCodec::codecForName("gbk"));//解决中文路径的问题
		if(mImage)
		{
			cvReleaseImage(&mImage);
		}
		if(mOriginalImage)
		{
			cvReleaseImage(&mOriginalImage);
		}
		mOriginalImage = cvLoadImage(fileName.toStdString().c_str());
		mImage = cvCloneImage(mOriginalImage);
		if(mImage)
		{
			QImage* image = ImageAdapter::IplImage2QImage(mImage);
			imageWidget->setImage(image);
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
		if(mImage)
		{
			cvSaveImage(fileName.toStdString().c_str(), mImage);
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
	mEnableMasks = actionEnableMasks->isChecked();
	imageWidget->enableMasks(mEnableMasks);
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

class LineSeg
{
public:
	LineSeg()
	{

	}
	LineSeg(const CvPoint& p0, const CvPoint& p1, int thetaThresh, int rhoThresh, int distThresh)
	{
		point[0] = p0;
		point[1] = p1;

		theta_threshold = M_PI / 180.0 * thetaThresh;
		rho_threshold = rhoThresh;
		dist_threahold = distThresh;

		length = disance(p0, p1);

		calcLineParam();
	}

	void calcLineParam()
	{
		theta = atan2f(point[0].y - point[1].y, point[0].x - point[1].x);
		if(theta < 0)
		{
			theta += M_PI;
		}
		rho = fabs((point[0].x * point[1].y - point[0].y * point[1].x) / length);
	}

	float disance(const CvPoint& p0, const CvPoint& p1)
	{
		float dist = sqrtf((p0.x - p1.x) * (p0.x - p1.x) + (p0.y - p1.y) * (p0.y - p1.y));
		return dist;
	}

	CvPoint intersect(const LineSeg& lineSeg)
	{
		float x1 = point[0].x;
		float x2 = point[1].x;
		float x3 = lineSeg.point[0].x;
		float x4 = lineSeg.point[1].x;

		float y1 = point[0].y;
		float y2 = point[1].y;
		float y3 = lineSeg.point[0].y;
		float y4 = lineSeg.point[1].y;

		float a = (y1-y2)/(x1-x2);
		float b = (y3-y4)/(x3-x4);

		float x = (a*x2-b*x4-y2+y4)/(a-b);
		float y = a*x-a*x2+y2;

		CvPoint point;
		point.x = (int)x;
		point.y = (int)y;

		return point;
	}

	bool combine(const LineSeg& lineSeg)
	{
		float thetaDistance = fabs(theta - lineSeg.theta);
		if(thetaDistance > M_PI / 2)
		{
			thetaDistance = M_PI - thetaDistance;
		}

		float rhoDistance = fabs(rho - lineSeg.rho);

		float minDist = 4000.0;
		for(int i = 0; i<2; i++)
		{
			if(minDist == 0)
			{
				break;
			}
			for(int j=0; j<2; j++)
			{
				if(minDist == 0)
				{
					break;
				}
				float dist = disance(point[i], lineSeg.point[j]);
				if(dist < minDist)
				{
					minDist = dist;
				}

				//计算是否交叠
				if(i == j)
				{
					int cosine1 = (point[i].x - lineSeg.point[j].x)*(lineSeg.point[1-j].x - lineSeg.point[j].x) 
						+ (point[i].y - lineSeg.point[j].y)*(lineSeg.point[1-j].y - lineSeg.point[j].y);

					int cosine2 = (point[i].x - lineSeg.point[1-j].x)*(lineSeg.point[j].x - lineSeg.point[1-j].x) 
						+ (point[i].y - lineSeg.point[1-j].y)*(lineSeg.point[j].y - lineSeg.point[1-j].y) ;

					if(cosine1 > 0 && cosine2 > 0)
					{
						minDist = 0;
						break;
					}

					cosine1 = (lineSeg.point[i].x - point[j].x)*(point[1-j].x - point[j].x) 
						+ (lineSeg.point[i].y - point[j].y)*(point[1-j].y - point[j].y);

					cosine2 = (lineSeg.point[i].x - point[1-j].x)*(point[j].x - point[1-j].x) 
						+ (lineSeg.point[i].y - point[1-j].y)*(point[j].y - point[1-j].y) ;

					if(cosine1 > 0 && cosine2 > 0)
					{
						minDist = 0;
						break;
					}
				}
			}
		}
		

		if(thetaDistance > theta_threshold || rhoDistance > rho_threshold || minDist > dist_threahold)
		{
			return false;
		}

		float maxDist = length;
		CvPoint p0 = point[0];
		CvPoint p1 = point[1];
		if(lineSeg.length > maxDist)
		{
			maxDist = lineSeg.length;
			p0 = lineSeg.point[0];
			p1 = lineSeg.point[1];
		}

		for(int i = 0; i<2; i++)
		{
			for(int j=0; j<2; j++)
			{
				float dist = disance(point[i], lineSeg.point[j]);
				if(dist > maxDist)
				{
					maxDist = dist;
					p0 = point[i];
					p1 = lineSeg.point[j];
				}
			}
		}

		length = maxDist;
		point[0] = p0;
		point[1] = p1;
		calcLineParam();

		return true;
	}

	CvPoint point[2];
	float theta;
	float rho;
	float length;

	float theta_threshold;
	float rho_threshold;
	float dist_threahold;
};

void MainWindow::processImage()
{
	if(mOriginalImage == NULL)
	{
		QMessageBox msgBox;
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.setText(tr("No image loaded.\nPlease load an image file before OpenCV."));
		msgBox.exec();
		return;
	}
	
	bool cannyGroup = paramWidget->cannyGroup->isChecked();
	int cannyThreshold1 = paramWidget->cannyThreshold1->value();
	int cannyThreshold2 = paramWidget->cannyThreshold2->value();
	
	bool dilateGroup = paramWidget->dilateGroup->isChecked();
	int dilateIter = paramWidget->dilateIter->value();
	bool erodeGroup = paramWidget->erodeGroup->isChecked();
	int erodeIter = paramWidget->erodeIter->value();

	bool houghGroup = paramWidget->houghGroup->isChecked();
	int houghRho = paramWidget->houghRho->value();
	int houghTheta = paramWidget->houghTheta->value();
	int houghThreshold = paramWidget->houghThreshold->value();
	int houghParam1 = paramWidget->houghParam1->value();
	int houghParam2 = paramWidget->houghParam2->value();

	bool combineGroup = paramWidget->combineGroup->isChecked();
	int combineTheta = paramWidget->combineTheta->value();
	int combineRho = paramWidget->combineRho->value();
	int combineDistance = paramWidget->combineDistance->value();

	bool rectangleGroup = paramWidget->rectangleGroup->isChecked();
	int rectangleHorizontalLength = paramWidget->rectangleHorizontalLength->value();
	int rectangleVerticalLength = paramWidget->rectangleVerticalLength->value();

	bool backGroundGroup = paramWidget->backGroundGroup->isChecked();

	cvReleaseImage(&mImage);
	mImage = cvCloneImage(mOriginalImage);

	if(mImage->nChannels == 3)
	{
		IplImage* grayImage = cvCreateImage(cvGetSize(mImage), 8, 1);
		cvCvtColor(mImage, grayImage, CV_RGB2GRAY);
		cvReleaseImage(&mImage);
		mImage = grayImage;
	}
	
	if(mImage->nChannels == 1 && cannyGroup)
	{
		IplImage* contourImage = cvCreateImage(cvGetSize(mImage), 8, 1);
		cvCanny(mImage, contourImage, cannyThreshold1, cannyThreshold2, 3);
		cvReleaseImage(&mImage);
		mImage = contourImage;
	}

	if(dilateGroup && dilateIter>0)
	{
		IplImage* dialatedImage = cvCreateImage(cvGetSize(mImage), 8, 1);
		cvDilate(mImage, dialatedImage, 0, dilateIter);
		cvReleaseImage(&mImage);
		mImage = dialatedImage;
	}

	if(erodeGroup && erodeIter>0)
	{
		IplImage* erodedImage = cvCreateImage(cvGetSize(mImage), 8, 1);
		cvErode(mImage, erodedImage, 0, erodeIter);
		cvReleaseImage(&mImage);
		mImage = erodedImage;
	}
	
	if(houghGroup)
	{
		//检测线段
		IplImage* lineImage = cvCreateImage(cvGetSize(mImage), 8, 3);
		if(backGroundGroup)
		{
			cvCvtColor(mImage, lineImage, CV_GRAY2BGR);
		}
		CvMemStorage* storage = cvCreateMemStorage(0);
		CvSeq* lines = 0;
		lines = cvHoughLines2( mImage, storage, CV_HOUGH_PROBABILISTIC, houghRho, CV_PI/180 * houghTheta, houghThreshold, houghParam1, houghParam2 );
		
		std::vector<LineSeg> lineSegList;
		for( int i = 0; i < lines ->total; i++ )  //lines存储的是直线  
		{  
			CvPoint* line = ( CvPoint* )cvGetSeqElem( lines, i );  //lines序列里面存储的是像素点坐标  
			
			if(combineGroup == false)
			{
				cvLine( lineImage, line[0], line[1], CV_RGB(255, 0, 0), 1, CV_AA );
			}
			else
			{
				LineSeg lineSeg(line[0], line[1], combineTheta, combineRho, combineDistance);
				bool combined = false;
				for(int n=0; n<lineSegList.size(); n++)
				{
					if(lineSegList[n].combine(lineSeg))
					{
						combined = true;
						break;
					}
				}
				if(combined == false)
				{
					lineSegList.push_back(lineSeg);
				}
			}
		}

		if(combineGroup)
		{
			int minHRho = 5000;
			int maxHRho = 0;
			int minVRho = 5000;
			int maxVRho = 0;

			LineSeg minH;
			LineSeg maxH;
			LineSeg minV;
			LineSeg maxV;


			for(int n=0; n<lineSegList.size(); n++)
			{
				if(rectangleGroup)
				{

					if(lineSegList[n].theta < M_PI * 0.25 || lineSegList[n].theta > M_PI * 0.75)//水平
					{
						if(lineSegList[n].length > rectangleHorizontalLength)
						{
							if(lineSegList[n].rho < minHRho && lineSegList[n].rho > 300)
							{
								minHRho = lineSegList[n].rho;
								minH = lineSegList[n];
							}
							if(lineSegList[n].rho > maxHRho && lineSegList[n].rho < mImage->height - 100)
							{
								maxHRho = lineSegList[n].rho;
								maxH = lineSegList[n];
							}
						}

					}
					else
					{
						if(lineSegList[n].length > rectangleVerticalLength)
						{
							if(lineSegList[n].rho < minVRho && lineSegList[n].rho > 80)
							{
								minVRho = lineSegList[n].rho;
								minV = lineSegList[n];
							}
							if(lineSegList[n].rho > maxVRho && lineSegList[n].rho < mImage->width - 20)
							{
								maxVRho = lineSegList[n].rho;
								maxV = lineSegList[n];
							}
						}
					}
				}
				else
				{
					cvLine( lineImage, lineSegList[n].point[0], lineSegList[n].point[1], CV_RGB(255, 0, 0), 1, CV_AA );

					CvFont font;
					cvInitFont(&font, FONT_HERSHEY_PLAIN, 1, 1);
					cvPutText(lineImage, (QString::number((int)(lineSegList[n].theta / M_PI*180)) + "," + QString::number((int)(lineSegList[n].rho))).toStdString().c_str(), lineSegList[n].point[0], &font,CV_RGB(255, 255, 255));
				}
			}


			if(rectangleGroup)
			{
				CvPoint points[4];
				points[0] = minH.intersect(minV);
				points[1] = minH.intersect(maxV);
				points[2] = maxH.intersect(maxV);
				points[3] = maxH.intersect(minV);

				cvLine( lineImage, points[0], points[1], CV_RGB(255, 0, 0), 3, CV_AA );
				cvLine( lineImage, points[1], points[2], CV_RGB(255, 0, 0), 3, CV_AA );
				cvLine( lineImage, points[2], points[3], CV_RGB(255, 0, 0), 3, CV_AA );
				cvLine( lineImage, points[3], points[0], CV_RGB(255, 0, 0), 3, CV_AA );
			}
		}

		cvReleaseImage(&mImage);
		mImage = lineImage;
	}




	QImage* image = ImageAdapter::IplImage2QImage(mImage);
	imageWidget->setImage(image);
}

void MainWindow::recognizeText()
{
	if(!mImage)
	{
		QMessageBox msgBox;
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.setText(tr("No image loaded.\nPlease load an image file before OCR."));
		msgBox.exec();
		return;
	}
	else
	{
		QVector<QRect>* masks = imageWidget->getMasks();

		if(mAbbyyOCR == NULL)
		{
			mAbbyyOCR = new AbbyyOCR();
		}
		mAbbyyOCR->setImage(mImage);

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

		tessBaseAPI->SetImage((uchar*)mImage->imageData, mImage->width, mImage->height, mImage->nChannels, mImage->widthStep);

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
					textEdit->setText(QString("%1%2").arg(textEdit->toPlainText(),ocrResult));
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
}

void MainWindow::showCube()
{
	if(cubeWidget == NULL)
	{
		cubeWidget = new CubeWidget();
	}
	cubeWidget->setImage(mImage);
	cubeWidget->show();
	cubeWidget->raise();
	cubeWidget->activateWindow();
}
