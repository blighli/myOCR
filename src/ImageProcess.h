#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H

#include <opencv2/opencv.hpp>
#include "LineSegment.h"
#include <QtCore/QtCore>
#include "OCRMask.h"

typedef struct ImageProcessParam
{
	bool debug;

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
	int rectangleHMinLength;
	int rectangleVMinLength;
	int rectangleTop;
	int rectangleBottom;

	bool useBackGround;

	bool useNormalize;
	int normalizeTop;
	int normalizeWidth;
	int normalizeHeight;
}
ImageProcessParam;

class ImageProcess
{
public:
	ImageProcess();
	~ImageProcess();

	void setImage(IplImage* image);
	void setMasks(QVector<OCRMask>* masks);
	void run(ImageProcessParam* param);

	IplImage* getOriginalImage();
	IplImage* getProcessedImage();

private:
	CvRect redStamp( IplImage* image, int minPixCount = 10, int minPixStack = 3, int minRowHitStack = 5, int minRowMissStack = 10);
	void blueText(IplImage* image, int minHue, int maxHue);

	void rgb2hsl(int red, int green, int blue, int& hue, int& saturation, int& luminance);
	int countInRect(IplImage* image, CvRect* rect);
	int adjustRect(IplImage* image, CvRect* rect);

	CvSeq* hough( ImageProcessParam* param );
	void combine( ImageProcessParam* param, CvSeq* lines, std::vector<LineSegment>& lineSegList);
	void rectangle( ImageProcessParam* param, std::vector<LineSegment> &lineSegList, int &minHRho, LineSegment &minH, int &maxHRho, LineSegment &maxH, int &minVRho, LineSegment &minV, int &maxVRho, LineSegment &maxV );
	void normalize( ImageProcessParam* param, LineSegment &minH, LineSegment minV, LineSegment maxV, LineSegment &maxH );

	QVector<OCRMask>* mMasks;

	IplImage* mOriginalImage;
	IplImage* mProcessedImage;
	bool mProcessed;
};

#endif //IMAGEPROCESS_H