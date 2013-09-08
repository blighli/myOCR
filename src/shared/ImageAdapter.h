#ifndef IMAGEADAPTER_H
#define IMAGEADAPTER_H

#include <opencv2/opencv.hpp>
#include <QImage>

class ImageAdapter
{
public:
	static QImage* IplImage2QImage(const IplImage *iplImage);
};

#endif //IMAGEADAPTER_H