#ifndef OCRMASK_H
#define OCRMASK_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

class OCRMask
{
public:
	CvRect rect;
	std::string key;
	std::string value;

	OCRMask();
};

#endif //OCRMASK_H