#ifndef OCRMASK_H
#define OCRMASK_H

#include "Common.h"
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