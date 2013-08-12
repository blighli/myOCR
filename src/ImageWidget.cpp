#include "ImageWidget.h"
#include <QtGui>

ImageWidget::ImageWidget()
{
	image = NULL;
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

void ImageWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	if(image)
	{
		painter.drawImage(0,0, *image);
	}
}