#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <leptonica/allheaders.h>
#include <tesseract/baseapi.h>
#include "Share/OCRMask.h"
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
class QTwain;
class CDIB;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();
	~MainWindow();
protected:
	void showEvent(QShowEvent* thisEvent);
	bool winEvent(MSG* pMsg, long* result);

private slots:
	void onDibAcquired(CDIB* pDib);

	void openImageFile();
	void saveImageFile();
	void scanImage();
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

	QAction* actionScan;

	//internal variables
	QTwain* mTwain;

	AbbyyOCR* mAbbyyOCR;
	TesseractOCR* mTesseractOCR;
	ImageProcess* mImageProcess;

	std::vector<OCRMask>* mOCRMasks;
};

#endif //MAINWINDOW_H