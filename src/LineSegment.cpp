#include "LineSegment.h"


LineSegment::LineSegment()
{

}

LineSegment::LineSegment( const CvPoint& p0, const CvPoint& p1, int thetaThresh, int rhoThresh, int distThresh )
{
	point[0] = p0;
	point[1] = p1;

	theta_threshold = CV_PI / 180.0 * thetaThresh;
	rho_threshold = rhoThresh;
	dist_threahold = distThresh;

	length = disance(p0, p1);

	calcLineParam();
}

void LineSegment::calcLineParam()
{
	theta = atan2f(point[0].y - point[1].y, point[0].x - point[1].x);
	if(theta < 0)
	{
		theta += CV_PI;
	}
	rho = fabs((point[0].x * point[1].y - point[0].y * point[1].x) / length);
}

float LineSegment::disance( const CvPoint& p0, const CvPoint& p1 )
{
	float dist = sqrtf((p0.x - p1.x) * (p0.x - p1.x) + (p0.y - p1.y) * (p0.y - p1.y));
	return dist;
}

CvPoint LineSegment::intersect( const LineSegment& lineSeg )
{
	float x1 = point[0].x;
	float x2 = point[1].x;
	float x3 = lineSeg.point[0].x;
	float x4 = lineSeg.point[1].x;

	float y1 = point[0].y;
	float y2 = point[1].y;
	float y3 = lineSeg.point[0].y;
	float y4 = lineSeg.point[1].y;

	float a = (y1-y2)/(x1-x2);
	float b = (y3-y4)/(x3-x4);

	float x = (a*x2-b*x4-y2+y4)/(a-b);
	float y = a*x-a*x2+y2;

	CvPoint point;
	point.x = (int)x;
	point.y = (int)y;

	return point;
}

bool LineSegment::combine( const LineSegment& lineSeg )
{
	float thetaDistance = fabs(theta - lineSeg.theta);
	if(thetaDistance > CV_PI / 2)
	{
		thetaDistance = CV_PI - thetaDistance;
	}

	float rhoDistance = fabs(rho - lineSeg.rho);

	float minDist = 4000.0;
	for(int i = 0; i<2; i++)
	{
		if(minDist == 0)
		{
			break;
		}
		for(int j=0; j<2; j++)
		{
			if(minDist == 0)
			{
				break;
			}
			float dist = disance(point[i], lineSeg.point[j]);
			if(dist < minDist)
			{
				minDist = dist;
			}

			//¼ÆËãÊÇ·ñ½»µþ
			if(i == j)
			{
				int cosine1 = (point[i].x - lineSeg.point[j].x)*(lineSeg.point[1-j].x - lineSeg.point[j].x) 
					+ (point[i].y - lineSeg.point[j].y)*(lineSeg.point[1-j].y - lineSeg.point[j].y);

				int cosine2 = (point[i].x - lineSeg.point[1-j].x)*(lineSeg.point[j].x - lineSeg.point[1-j].x) 
					+ (point[i].y - lineSeg.point[1-j].y)*(lineSeg.point[j].y - lineSeg.point[1-j].y) ;

				if(cosine1 > 0 && cosine2 > 0)
				{
					minDist = 0;
					break;
				}

				cosine1 = (lineSeg.point[i].x - point[j].x)*(point[1-j].x - point[j].x) 
					+ (lineSeg.point[i].y - point[j].y)*(point[1-j].y - point[j].y);

				cosine2 = (lineSeg.point[i].x - point[1-j].x)*(point[j].x - point[1-j].x) 
					+ (lineSeg.point[i].y - point[1-j].y)*(point[j].y - point[1-j].y) ;

				if(cosine1 > 0 && cosine2 > 0)
				{
					minDist = 0;
					break;
				}
			}
		}
	}


	if(thetaDistance > theta_threshold || rhoDistance > rho_threshold || minDist > dist_threahold)
	{
		return false;
	}

	float maxDist = length;
	CvPoint p0 = point[0];
	CvPoint p1 = point[1];
	if(lineSeg.length > maxDist)
	{
		maxDist = lineSeg.length;
		p0 = lineSeg.point[0];
		p1 = lineSeg.point[1];
	}

	for(int i = 0; i<2; i++)
	{
		for(int j=0; j<2; j++)
		{
			float dist = disance(point[i], lineSeg.point[j]);
			if(dist > maxDist)
			{
				maxDist = dist;
				p0 = point[i];
				p1 = lineSeg.point[j];
			}
		}
	}

	length = maxDist;
	point[0] = p0;
	point[1] = p1;
	calcLineParam();

	return true;
}
