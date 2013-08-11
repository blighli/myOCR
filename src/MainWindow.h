#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class ImageWidget;
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
	void openImage(const QString& fileName);
private:
	ImageWidget* imageWidget;
	QMenu* menuFile;
	QToolBar* toolBarFile;
	QAction* actionFileOpen;
	QAction* actionFileOCR;

	QImage* image;
	QLabel* labelOCRText;

};

#endif //MAINWINDOW_H