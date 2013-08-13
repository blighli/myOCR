#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>

class ImageWidget : public QWidget
{
	Q_OBJECT
public:
	ImageWidget();
	~ImageWidget();
	void setImage(QImage* image);

protected:
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
	void drawRule(QPainter* painter);
	void drawImage(QPainter* painter);

private:
	QImage* image;
	int m_nImagePadding;
	int m_nTickStepSmall;
	int m_nTickStepMedium;
	int m_nTickStepLarge;
	int m_nTickSizeSmall;
	int m_nTickSizeMedium;
	int m_nTickSizeLarge;

	int m_bDrawPosLine;
	QPoint m_mousePos;
};

#endif //IMAGEWIDGET_H