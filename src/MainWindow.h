#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>

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
private slots:
	void openFile();
	void startOCR();
private:
	ImageWidget* imageWidget;
	QTextEdit* textEdit;

	QMenu* menuFile;
	QToolBar* toolBarFile;
	QAction* actionFileOpen;
	QAction* actionFileOCR;

	IplImage* cvImage;
};

#endif //MAINWINDOW_H