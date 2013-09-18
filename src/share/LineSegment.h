#ifndef LINESEGMENT_H
#define LINESEGMENT_H

#include "Common.h"

class LineSegment
{
public:
	LineSegment(CvPoint* point);
	bool cross(const LineSegment& lineSeg, CvPoint* crossPoint, int degree, int gap);

private:
	float distance(const CvPoint& p0, const CvPoint& p1);
	float minValue(float a, float b);
	CvPoint point[2];
};


#endif //LINESEGMENT_H