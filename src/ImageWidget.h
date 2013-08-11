#include <QWidget>

class QImage;

class ImageWidget : public QWidget
{
	Q_OBJECT
public:
	ImageWidget();
	void openImage(const QString& fileName);

protected:
	void paintEvent(QPaintEvent *event);

private:
	QImage* image;
};