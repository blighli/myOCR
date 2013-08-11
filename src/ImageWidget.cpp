#include <QtGui>
#include "ImageWidget.h"

ImageWidget::ImageWidget()
{
	image = NULL;
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
		painter.drawImage(0, 0, *image);
	}
}