#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <leptonica/allheaders.h>
#include <tesseract/baseapi.h>

class ImageWidget;
class QTextEdit;
class QAction;
class QMenu;
class QTooBar;
class QLabel;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();
	~MainWindow();
private slots:
	void openFile();
	void saveFile();
	void startOCR();
	void startCV();

	void newChop();
	void saveChop();
	void loadChop();
	void viewChop();

private:
	ImageWidget* imageWidget;
	QTextEdit* textEdit;

	QMenu* menuFile;
	QMenu* menuImage;
	QMenu* menuHelp;

	QToolBar* toolBarFile;
	QToolBar* toolBarImage;
	
	QAction* actionFileOpen;
	QAction* actionFileSave;
	QAction* actionImageOCR;
	QAction* actionImageCV;
	QAction* actionImageNewChop;
	QAction* actionImageSaveChop;
	QAction* actionImageLoadChop;
	QAction* actionImageViewChop;
	QAction* actionHelpAbout;

	IplImage* cvImage;
	tesseract::TessBaseAPI* tessBaseAPI;
	Boxa* boxes;
};

#endif //MAINWINDOW_H