#include "LineSegment.h"


LineSegment::LineSegment(CvPoint* point)
{
	this->point[0] = point[0];
	this->point[1] = point[1];
}

float LineSegment::distance( const CvPoint& p0, const CvPoint& p1 )
{
	float dist = sqrtf((p0.x - p1.x) * (p0.x - p1.x) + (p0.y - p1.y) * (p0.y - p1.y));
	return dist;
}

float LineSegment::minValue(float a, float b)
{
	return a<b?a:b;
}

bool LineSegment::cross(const LineSegment& lineSeg, CvPoint* crossPoint)
{
	float len1 = distance(point[0], point[1]);
	float len2 = distance(lineSeg.point[0], lineSeg.point[1]);

	float dx1 = point[0].x - point[1].x;
	float dy1 = point[0].y - point[1].y;
	float dx2 = lineSeg.point[0].x - lineSeg.point[1].x;
	float dy2 = lineSeg.point[0].y - lineSeg.point[1].y;

	float cosTheta = (dx1 * dx2 + dy1 * dy2) / (len1 * len2);

	if( abs(cosTheta) > 0.17)
	{
		return false;
	}

	float d1 = distance(point[0], lineSeg.point[0]);
	float d2 = distance(point[1], lineSeg.point[0]);
	float d3 = distance(point[0], lineSeg.point[1]);
	float d4 = distance(point[1], lineSeg.point[1]);

	float minDistance = minValue(minValue(d1,d2),minValue(d3,d4));
	if(minDistance > 10)
	{
		return false;
	}


	float x1 = point[0].x;
	float x2 = point[1].x;
	float x3 = lineSeg.point[0].x;
	float x4 = lineSeg.point[1].x;

	float y1 = point[0].y;
	float y2 = point[1].y;
	float y3 = lineSeg.point[0].y;
	float y4 = lineSeg.point[1].y;

	if(x1 == x2)
	{
		float x = (int)x1;
		float b = (y3-y4)/(x3-x4);
		float y = b*x-b*x4+y4;

		crossPoint->x = (int)x;
		crossPoint->y = (int)y;

	}
	else if(x3 == x4)
	{
		float x  = (int)x3;
		float a = (y1-y2)/(x1-x2);
		float y = a*x-a*x2+y2;

		crossPoint->x = (int)x;
		crossPoint->y = (int)y;
	}
	else
	{
		float a = (y1-y2)/(x1-x2);
		float b = (y3-y4)/(x3-x4);

		float x = (a*x2-b*x4-y2+y4)/(a-b);
		float y = a*x-a*x2+y2;

		crossPoint->x = (int)x;
		crossPoint->y = (int)y;
	}

	return true;
}