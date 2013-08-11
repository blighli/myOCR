#include <QtGui>
#include "ImageWidget.h"

ImageWidget::ImageWidget()
{
	image = new QImage();
}

void ImageWidget::openImage(const QString& fileName)
{
	image->load(fileName);
	update();
}

void ImageWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	if(!image->isNull())
	{
		painter.drawImage(0, 0, *image);
	}
}