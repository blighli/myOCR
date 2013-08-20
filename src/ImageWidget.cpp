#include "ImageWidget.h"
#include <QtGui>

ImageWidget::ImageWidget()
{
	mImage = NULL;
	mBoxes = NULL;
	mBoxCount = 0;

	mDrawMesureLines = false;
	mEnableMesure =false;

	mEnableMasks = false;
	mMasks = new QVector<QRect>();
	mCurrentMask = NULL;

	mImagePadding = 20;
	mTickStepSmall = 10;
	mTickStepMedium = 50;
	mTickStepLarge = 100;
	mTickSizeSmall = 4;
	mTickSizeMedium = 10;
	mTickSizeLarge = 20;
}

ImageWidget::~ImageWidget()
{
	delete mImage;
	delete[] mBoxes;
	delete mMasks;
}

void ImageWidget::enableMesure(bool enabled)
{
	mEnableMesure = enabled;
}

void ImageWidget::enableMasks(bool enabled)
{
	mEnableMasks = enabled;
}

QVector<QRect>* ImageWidget::getMasks()
{
	return mMasks;
}

void ImageWidget::setImage(QImage* image)
{
	if(mImage)
	{
		delete mImage;
	}
	mImage = image;
	setBoxes(NULL, 0);

	setMinimumSize(mImage->width() + mImagePadding, mImage->height() + mImagePadding);

	update();
}

void ImageWidget::setBoxes(QRect* boxes, int boxCount)
{
	if(mBoxes)
	{
		delete[] mBoxes;
		mBoxCount = 0;
	}

	mBoxes = boxes;
	mBoxCount = boxCount;

	for(int i = 0; i< mBoxCount; i++)
	{
		mBoxes[i].translate(mImagePadding, mImagePadding);
	}

	update();
}

void ImageWidget::drawRule(QPainter* painter)
{
	int windowWidth = width();
	int windowHeight = height();

	QPoint ruleTopBegin = QPoint(mImagePadding, mImagePadding);
	QPoint ruleTopEnd = QPoint(windowWidth, mImagePadding);
	QPoint ruleLeftBegin = QPoint(mImagePadding, mImagePadding);
	QPoint ruleLeftEnd = QPoint(mImagePadding, windowHeight);

	//绘制标尺的外廓线
	painter->drawLine(ruleTopBegin, ruleTopEnd);
	painter->drawLine(ruleLeftBegin, ruleLeftEnd);
	//绘制标尺的刻度: 顶部标尺 + 左侧标尺
	int nFontSize = 14;
	QFont font = painter->font();
	font.setPixelSize(nFontSize);
	painter->setFont(font);

	int nTickTextPadding = mTickSizeSmall;
	for(int tickX = ruleTopBegin.x(); tickX <= ruleTopEnd.x(); tickX += mTickStepSmall)
	{
		int nTickSize = mTickSizeSmall;
		int nCurrentTick = tickX - ruleTopBegin.x();
		if(nCurrentTick % mTickStepLarge == 0)
		{
			nTickSize = mTickSizeLarge;
			QString tickText = QString::number(nCurrentTick);
			painter->drawText(tickX + nTickTextPadding, nFontSize, tickText);
		}
		else if(nCurrentTick % mTickStepMedium == 0)
		{
			nTickSize = mTickSizeMedium;
		}
		QPoint tickHead = QPoint(tickX, ruleTopBegin.y());
		QPoint tickTail = QPoint(tickX, ruleTopBegin.y() - nTickSize);
		painter->drawLine(tickHead, tickTail);
	}
	for(int tickY = ruleLeftBegin.y(); tickY <= ruleLeftEnd.y(); tickY += mTickStepSmall)
	{
		int nTickSize = mTickSizeSmall;
		int nCurrentTick = tickY - ruleLeftBegin.y();
		if(nCurrentTick % mTickStepLarge == 0)
		{
			nTickSize = mTickSizeLarge;
			QString tickText = QString::number(nCurrentTick);
			
			painter->save();
			painter->translate(mTickSizeLarge - nFontSize, tickY + nTickTextPadding);
			painter->rotate(90);
			painter->drawText(0, 0, tickText);
			painter->restore();
		}
		else if(nCurrentTick % mTickStepMedium == 0)
		{
			nTickSize = mTickSizeMedium;
		}
		QPoint tickHead = QPoint(ruleLeftBegin.x(), tickY);
		QPoint tickTail = QPoint(ruleLeftBegin.x() - nTickSize, tickY);
		painter->drawLine(tickHead, tickTail);
	}

	if(mDrawMesureLines && mEnableMesure)
	{

		QPoint vLineHead(mCurrentMousePos.x(), ruleTopBegin.y());
		QPoint hLineHead(ruleLeftBegin.x(), mCurrentMousePos.y());

		painter->save();

		QPen pen = painter->pen();
		pen.setStyle(Qt::DashLine);
		painter->setPen(pen);

		painter->drawLine(vLineHead, mCurrentMousePos);
		painter->drawLine(hLineHead, mCurrentMousePos);

		QString posText = QString("(%1,%2)").arg(mCurrentMousePos.x() - mImagePadding).arg(mCurrentMousePos.y() - mImagePadding);
		painter->drawText(mCurrentMousePos, posText);

		painter->restore();
	}
}

void ImageWidget::drawImage(QPainter* painter)
{
	if(mImage)
	{
		painter->drawImage(mImagePadding,mImagePadding, *mImage);
	}
}

void ImageWidget::drawBoxes(QPainter* painter)
{
	if(mBoxes)
	{
		painter->drawRects(mBoxes, mBoxCount);
	}
}

void ImageWidget::drawMasks(QPainter* painter)
{
	for(int i=0;i<mMasks->size();i++)
	{
		QRect rect = mMasks->at(i);
		rect.translate(mImagePadding, mImagePadding);
		painter->drawRect(rect);
	}
	if(mEnableMasks && mCurrentMask)
	{
		QRect rect = *mCurrentMask;
		rect.translate(mImagePadding, mImagePadding);
		painter->drawRect(rect);
	}
}

void ImageWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);

	drawImage(&painter);
	drawRule(&painter);
	drawBoxes(&painter);
	drawMasks(&painter);
}

void ImageWidget::mousePressEvent(QMouseEvent *event)
{
	mDrawMesureLines = true;
	mCurrentMousePos = event->pos();

	if(mImage && mEnableMasks && mCurrentMask == NULL)
	{
		if(mCurrentMousePos.x() >= mImage->width() + mImagePadding)
		{
			mCurrentMousePos.setX(mImage->width() + mImagePadding - 1);
		}
		if(mCurrentMousePos.x() < mImagePadding)
		{
			mCurrentMousePos.setX(mImagePadding);
		}
		if(mCurrentMousePos.y() >= mImage->height() + mImagePadding)
		{
			mCurrentMousePos.setY(mImage->height() + mImagePadding - 1);
		}
		if(mCurrentMousePos.y() < mImagePadding)
		{
			mCurrentMousePos.setY(mImagePadding);
		}

		mFirstCorner = QPoint(mCurrentMousePos.x() - mImagePadding, mCurrentMousePos.y() - mImagePadding);
		mCurrentMask = new QRect(mFirstCorner, mFirstCorner);
	}

	update();
}


void ImageWidget::mouseReleaseEvent(QMouseEvent *event)
{
	mDrawMesureLines = false;

	if(mImage && mEnableMasks && mCurrentMask)
	{
		mMasks->push_back(*mCurrentMask);
		delete mCurrentMask;
		mCurrentMask = NULL;
	}

	update();
}

void ImageWidget::mouseMoveEvent(QMouseEvent *event)
{
	mCurrentMousePos = event->pos();

	if(mImage && mEnableMasks && mCurrentMask)
	{
		if(mCurrentMousePos.x() >= mImage->width() + mImagePadding)
		{
			mCurrentMousePos.setX(mImage->width() + mImagePadding - 1);
		}
		if(mCurrentMousePos.x() < mImagePadding)
		{
			mCurrentMousePos.setX(mImagePadding);
		}
		if(mCurrentMousePos.y() >= mImage->height() + mImagePadding)
		{
			mCurrentMousePos.setY(mImage->height() + mImagePadding - 1);
		}
		if(mCurrentMousePos.y() < mImagePadding)
		{
			mCurrentMousePos.setY(mImagePadding);
		}

		QPoint lastCorner(mCurrentMousePos.x() - mImagePadding, mCurrentMousePos.y() - mImagePadding);

		mCurrentMask->setLeft(mFirstCorner.x() < lastCorner.x() ? mFirstCorner.x() : lastCorner.x());
		mCurrentMask->setRight(mFirstCorner.x() > lastCorner.x() ? mFirstCorner.x() : lastCorner.x());
		mCurrentMask->setTop(mFirstCorner.y() < lastCorner.y() ? mFirstCorner.y() : lastCorner.y());
		mCurrentMask->setBottom(mFirstCorner.y() > lastCorner.y() ? mFirstCorner.y() : lastCorner.y());

	}

	update();
}
