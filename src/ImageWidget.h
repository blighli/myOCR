#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>
#include "OCRMask.h"

class QMenu;

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

	void setMasks(QVector<OCRMask>* masks);

protected:
	void paintEvent(QPaintEvent *event);


	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
	void drawRule(QPainter* painter);
	void drawImage(QPainter* painter);
	void drawBoxes(QPainter* painter);
	void drawMasks(QPainter* painter);

private slots:
	void deleteMask();

private:
	QMenu* menuContext;
	QAction* actionDeleteMask;

	QImage* mImage;
	int mImagePadding;
	int mTickStepSmall;
	int mTickStepMedium;
	int mTickStepLarge;
	int mTickSizeSmall;
	int mTickSizeMedium;
	int mTickSizeLarge;

	bool mEnableMesure;
	bool mDrawMesureLines;
	QPoint mCurrentMousePos;
	QPoint mFirstCorner;

	bool mEnableMasks;
	QVector<OCRMask>* mMasks;
	OCRMask* mCurrentMask;
	int mMaskIndex;
	
	QRect* mBoxes;
	int mBoxCount;
};

#endif //IMAGEWIDGET_H