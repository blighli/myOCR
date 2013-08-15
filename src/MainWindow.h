#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <leptonica/allheaders.h>
#include <tesseract/baseapi.h>

class ImageWidget;
class QTextEdit;

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
	void processImage();

	void enableMarks();
	void saveMarks();
	void loadMarks();
	void clearMarks();

	void enableMesure();
	void enableChinese();
private:
	//user interface components
	ImageWidget* imageWidget;
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
	QAction* actionToggleMarks;
	QAction* actionSaveMarks;
	QAction* actionLoadMarks;
	QAction* actionClearMarks;
	QAction* actionToggleMesure;
	QAction* actionToggleChinese;
	QAction* actionAbout;

	//internal variables
	IplImage* cvImage;
	tesseract::TessBaseAPI* tessBaseAPI;
	Boxa* boxes;

	bool m_bNewChop;
};

#endif //MAINWINDOW_H