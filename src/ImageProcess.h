#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H

#include <opencv2/opencv.hpp>
#include "LineSegment.h"
#include <QtCore/QtCore>
#include "OCRMask.h"

typedef struct ImageProcessParam
{
	bool debug;

	bool gray;

	bool threshold;
	int thresholdBlockSize;
	int thresholdMeanBias;

	bool hough;
	int houghRho;
	int houghTheta;
	int houghThreshold;
	int houghParam1;
	int houghParam2;

	bool corner;
	int cornerDegree;
	int cornerGap;

	bool background;

	bool normalize;
	int normalizeTopMargin;
	int normalizeSideMargin;
	int normalizeWidth;
	int normalizeHeight;

	bool blueText;
	int blueTextMin;
	int blueTextMax;
}
ImageProcessParam;

class ImageProcess
{
public:
	ImageProcess();
	~ImageProcess();

	void setImage(IplImage* image);
	void setMasks(QVector<OCRMask>* masks);
	int run(ImageProcessParam* param);


	IplImage* getOriginalImage();
	IplImage* getProcessedImage();

private:
	int limit(int value, int min, int max);
	void rgb2hsl(int red, int green, int blue, int& hue, int& saturation, int& luminance);
	
	void blueText(IplImage* image, int minHue, int maxHue);

	int countInRect(IplImage* image, CvRect* rect);
	int adjustRect(IplImage* image, CvRect* rect);


	CvRect makeRect(int centerX, int centerY, int width, int height = 0);
	CvRect findRedStampRect( IplImage* image, int minPixCount = 10, int minPixStack = 3, int minRowHitStack = 5, int minRowMissStack = 10);
	const CvRect& findTableRect(const CvRect &redStampRect);

	void findCornerRects(CvRect* cornerRects, const CvRect& rect, float rate);

	CvSeq* hough(IplImage* image, ImageProcessParam* param );
	bool findCornerPoint(ImageProcessParam* param, CvSeq* lines, CvPoint* point);
	void normalize( ImageProcessParam* param, CvPoint** cornerPoints);

	QVector<OCRMask>* mMasks;

	IplImage* mOriginalImage;
	IplImage* mProcessedImage;
	bool mProcessed;
};

#endif //IMAGEPROCESS_H