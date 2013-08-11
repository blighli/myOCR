#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class ImageWidget;
class QAction;
class QMenu;
class QTooBar;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();
private slots:
	void openFile();
private:
	ImageWidget* imageWidget;
	QMenu* menuFile;
	QToolBar* toolBarFile;
	QAction* actionFileOpen;

};

#endif //MAINWINDOW_H