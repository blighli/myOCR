#include <QMainWindow>

class ImageWidget;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();
private:
	ImageWidget* imageWidget;

};