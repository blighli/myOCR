#ifndef IMAGEADAPTER_H
#define IMAGEADAPTER_H

#include <leptonica/allheaders.h>
#include <QImage>

class ImageAdapter
{
public:
	PIX* qImage2PIX(const QImage& qImage);
	QImage PIX2qImage(PIX* pixImage);
};

#endif //IMAGEADAPTER_H