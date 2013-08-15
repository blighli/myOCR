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
	void setBoxes(QRect* mBoxes, int boxCount);
	void enableMesure(bool enabled);
	void enableMasks(bool enabled);

	QVector<QRect>* getMasks();

protected:
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
	void drawRule(QPainter* painter);
	void drawImage(QPainter* painter);
	void drawMasks(QPainter* painter);

private:
	QImage* mImage;
	int mImagePadding;
	int mTickStepSmall;
	int mTickStepMedium;
	int mTickStepLarge;
	int mTickSizeSmall;
	int mTickSizeMedium;
	int mTickSizeLarge;

	bool mEnbaleMesure;
	bool mDrawMesureLines;
	QPoint mCurrentMousePos;

	bool mEnbaleMasks;
	QVector<QRect>* mMasks;
	QRect* mCurrentMask;
	
	QRect* mBoxes;
	int mBoxCount;
};

#endif //IMAGEWIDGET_H