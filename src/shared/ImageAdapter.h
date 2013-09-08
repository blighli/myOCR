#ifndef IMAGEADAPTER_H
#define IMAGEADAPTER_H

#include "Common.h"
#include <QImage>

class ImageAdapter
{
public:
	static QImage* IplImage2QImage(const IplImage *iplImage);
};

#endif //IMAGEADAPTER_H