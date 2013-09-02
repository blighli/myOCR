#ifndef LINESEGMENT_H
#define LINESEGMENT_H

#include <opencv2/opencv.hpp>

class LineSegment
{
public:
	LineSegment();
	LineSegment(const CvPoint& p0, const CvPoint& p1, int thetaThresh, int rhoThresh, int distThresh);	
	
	float disance(const CvPoint& p0, const CvPoint& p1);
	CvPoint intersect(const LineSegment& lineSeg);
	bool combine(const LineSegment& lineSeg);

	CvPoint point[2];
	float theta;
	float rho;
	float length;
private:
	void calcLineParam();
	float theta_threshold;
	float rho_threshold;
	float dist_threahold;
};


#endif //LINESEGMENT_H