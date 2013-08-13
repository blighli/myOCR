#include "ImageWidget.h"
#include <QtGui>

ImageWidget::ImageWidget()
{
	image = NULL;
	m_nImagePadding = 20;
	m_nTickStepSmall = 10;
	m_nTickStepMedium = 50;
	m_nTickStepLarge = 100;
	m_nTickSizeSmall = 4;
	m_nTickSizeMedium = 10;
	m_nTickSizeLarge = 20;
}

ImageWidget::~ImageWidget()
{
	delete image;
}

void ImageWidget::setImage(QImage* image)
{
	this->image = image;
	update();
}

void ImageWidget::drawRule(QPainter* painter)
{
	int nWindowWidth = this->size().width();
	int nWindowHeight = this->size().height();

	QPoint ruleTopBegin = QPoint(m_nImagePadding, m_nImagePadding);
	QPoint ruleTopEnd = QPoint(nWindowWidth, m_nImagePadding);
	QPoint ruleLeftBegin = QPoint(m_nImagePadding, m_nImagePadding);
	QPoint ruleLeftEnd = QPoint(m_nImagePadding, nWindowHeight);

	//绘制标尺的外廓线
	painter->drawLine(ruleTopBegin, ruleTopEnd);
	painter->drawLine(ruleLeftBegin, ruleLeftEnd);
	//绘制标尺的刻度: 顶部标尺 + 左侧标尺
	int nFontSize = 14;
	QFont font = painter->font();
	font.setPixelSize(nFontSize);
	painter->setFont(font);

	int nTickTextPadding = m_nTickSizeSmall;
	for(int tickX = ruleTopBegin.rx(); tickX <= ruleTopEnd.rx(); tickX += m_nTickStepSmall)
	{
		int nTickSize = m_nTickSizeSmall;
		int nCurrentTick = tickX - ruleTopBegin.rx();
		if(nCurrentTick % m_nTickStepLarge == 0)
		{
			nTickSize = m_nTickSizeLarge;
			QString tickText = QString::number(nCurrentTick);
			painter->drawText(tickX + nTickTextPadding, nFontSize, tickText);
		}
		else if(nCurrentTick % m_nTickStepMedium == 0)
		{
			nTickSize = m_nTickSizeMedium;
		}
		QPoint tickHead = QPoint(tickX, ruleTopBegin.ry());
		QPoint tickTail = QPoint(tickX, ruleTopBegin.ry() - nTickSize);
		painter->drawLine(tickHead, tickTail);
	}
	for(int tickY = ruleLeftBegin.ry(); tickY <= ruleLeftEnd.ry(); tickY += m_nTickStepSmall)
	{
		int nTickSize = m_nTickSizeSmall;
		int nCurrentTick = tickY - ruleLeftBegin.ry();
		if(nCurrentTick % m_nTickStepLarge == 0)
		{
			nTickSize = m_nTickSizeLarge;
			QString tickText = QString::number(nCurrentTick);
			
			painter->save();
			painter->translate(m_nTickSizeLarge - nFontSize, tickY + nTickTextPadding);
			painter->rotate(90);
			painter->drawText(0, 0, tickText);
			painter->restore();
		}
		else if(nCurrentTick % m_nTickStepMedium == 0)
		{
			nTickSize = m_nTickSizeMedium;
		}
		QPoint tickHead = QPoint(ruleLeftBegin.rx(), tickY);
		QPoint tickTail = QPoint(ruleLeftBegin.rx() - nTickSize, tickY);
		painter->drawLine(tickHead, tickTail);
	}
}

void ImageWidget::drawImage(QPainter* painter)
{
	if(image)
	{
		painter->drawImage(m_nImagePadding,m_nImagePadding, *image);
	}
}

void ImageWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);

	drawImage(&painter);
	drawRule(&painter);
}