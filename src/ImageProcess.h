#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H

#include <opencv2/opencv.hpp>
#include "LineSegment.h"

typedef struct ImageProcessParam
{
	bool useGray;

	bool useCanny;
	int cannyThreshold1;
	int cannyThreshold2;

	bool useDilate;
	int dilateIter;
	bool useErode;
	int erodeIter;

	bool useHough;
	int houghRho;
	int houghTheta;
	int houghThreshold;
	int houghParam1;
	int houghParam2;

	bool useCombine;
	int combineTheta;
	int combineRho;
	int combineDistance;

	bool useRectangle;
	int rectangleHorizontalLength;
	int rectangleVerticalLength;

	bool useBackGround;

	bool useNormalize;
}
ImageProcessParam;

class ImageProcess
{
public:
	ImageProcess();
	~ImageProcess();

	void setImage(IplImage* image);
	void run(ImageProcessParam* param);

	IplImage* getOriginalImage();
	IplImage* getProcessedImage();

	
private:
	IplImage* mOriginalImage;
	IplImage* mProcessedImage;
	bool mProcessed;
};

#endif //IMAGEPROCESS_H