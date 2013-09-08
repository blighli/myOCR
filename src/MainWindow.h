#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <leptonica/allheaders.h>
#include <tesseract/baseapi.h>
#include "OCRMask.h"
#include <vector>
#include <string>

class ImageWidget;
class QTextEdit;
class AbbyyOCR;
class CubeWidget;
class ParamWidget;
class ImageProcess;
class TesseractOCR;
class OCRWidget;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();
	~MainWindow();
private slots:
	void openImageFile();
	void saveImageFile();
	void recognizeText();

	void showParamWidget();
	void processImage();

	void enableMasks();
	void saveMasks();
	void loadMasks();
	void clearMasks();

	void enableMesure();
	void enableChinese();

	void showCube();
private:
	void buildUI();
	//user interface components
	ImageWidget* imageWidget;
	OCRWidget* ocrWidget;
	CubeWidget* cubeWidget;
	ParamWidget* paramWidget;
	QTextEdit* textEdit;

	QMenu* menuFile;
	QMenu* menuImage;
	QMenu* menuHelp;

	QToolBar* toolBarFile;
	QToolBar* toolBarImage;
	
	QAction* actionOpenImage;
	QAction* actionSaveImage;
	QAction* actionRecognizeText;
	QAction* actionProcessImage;
	QAction* actionEnableMasks;
	QAction* actionSaveMasks;
	QAction* actionLoadMasks;
	QAction* actionClearMasks;
	QAction* actionEnableMesure;
	QAction* actionEnableChinese;
	QAction* actionShowCube;
	QAction* actionAbout;

	//internal variables
	AbbyyOCR* mAbbyyOCR;
	TesseractOCR* mTesseractOCR;
	ImageProcess* mImageProcess;

	std::vector<OCRMask>* mOCRMasks;
};

#endif //MAINWINDOW_H