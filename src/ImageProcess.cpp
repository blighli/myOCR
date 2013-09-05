#include "ImageProcess.h"
#include <vector>

#define min3v(v1, v2, v3)   ((v1)>(v2)? ((v2)>(v3)?(v3):(v2)):((v1)>(v3)?(v3):(v2)))
#define max3v(v1, v2, v3)   ((v1)<(v2)? ((v2)<(v3)?(v3):(v2)):((v1)<(v3)?(v3):(v1)))

ImageProcess::ImageProcess()
{
	mMasks = NULL;
	mOriginalImage = NULL;
	mProcessedImage = NULL;
	mProcessed = false;
}
ImageProcess::~ImageProcess()
{
	if(mOriginalImage)
	{
		cvReleaseImage(&mOriginalImage);
	}
	if(mProcessedImage)
	{
		cvReleaseImage(&mProcessedImage);
	}
}

void ImageProcess::setImage( IplImage* image )
{
	if(mOriginalImage)
	{
		cvReleaseImage(&mOriginalImage);
	}
	mOriginalImage = image;

	if(mProcessedImage)
	{
		cvReleaseImage(&mProcessedImage);
	}
	mProcessedImage = cvCloneImage(mOriginalImage);

	mProcessed = false;
}

IplImage* ImageProcess::getOriginalImage()
{
	return mOriginalImage;
}

IplImage* ImageProcess::getProcessedImage()
{
	return mProcessedImage;
}


CvRect ImageProcess::redStamp( IplImage* image, int minPixCount, int minPixStack, int minRowHitStack, int minRowMissStack )
{
	int top = 0;
	int bottom = 0;
	int left = 0;
	int right = 0;

	int rowHitStack = 0;
	int rowMissStack = 0;

	for(int y=0; y<mProcessedImage->height; y++)
	{

		int pixCount = 0;
		int pixStack = 0;
		int rowLeft = 0;
		int rowRight = 0;

		for(int x=0; x<mProcessedImage->width; x++)
		{
			uchar* ptr = (uchar*)(mProcessedImage->imageData + y * mProcessedImage->widthStep + x * 3);

			int b = ptr[0];
			int g = ptr[1];
			int r = ptr[2];

			//int gray = 255;
			if(r > 100 && r > b*2 && r > g*2)
			{
				//gray = 0;

				pixCount++;
				pixStack++;

				if(pixStack > minPixStack)
				{
					if(rowLeft == 0)
					{
						rowLeft = x - minPixStack;
					}
					rowRight = x;
				}
			}
			else
			{
				pixStack = 0;
			}

			//ptr[0] = gray;
			//ptr[1] = gray;
			//ptr[2] = gray;
		}

		if(pixCount > minPixCount)
		{
			rowHitStack++;
			rowMissStack = 0;

			if(rowHitStack > minRowHitStack)
			{
				if(top == 0)
				{
					top = y - minRowHitStack;
				}


				if(left == 0 || rowLeft < left)
				{
					left = rowLeft;
				}
				if(rowRight > right)
				{
					right = rowRight;
				}

			}
		}
		else
		{
			rowMissStack++;
			rowHitStack = 0;

			if(rowMissStack > minRowMissStack && top != 0)
			{
				bottom = y - rowMissStack;
				break;
			}
		}
	}

	return cvRect(left, top, right - left, bottom - top);
}


void ImageProcess::rgb2hsl(int red, int green, int blue, int& hue, int& saturation, int& luminance)
{
	float h=0, s=0, l=0;

	float r = red/255.f;
	float g = green/255.f;
	float b = blue/255.f;

	float maxVal = max3v(r, g, b);
	float minVal = min3v(r, g, b);

	if(maxVal == minVal)  
	{
		h = 0; // undefined  
	}
	else if(maxVal==r && g>=b)
	{
		h = 40.0f*(g-b)/(maxVal-minVal);
	}
	else if(maxVal==r && g<b)
	{
		h = 40.0f*(g-b)/(maxVal-minVal) + 240.0f;
	}
	else if(maxVal==g)  
	{
		h = 40.0f*(b-r)/(maxVal-minVal) + 80.0f;  
	}
	else if(maxVal==b)
	{
		h = 40.0f*(r-g)/(maxVal-minVal) + 160.0f;
	}
	
	// luminance  
	l = (maxVal+minVal)/2.0f;

	// saturation  
	if(l == 0 || maxVal == minVal)  
	{
		s = 0;
	}  
	else if(0<l && l<=0.5f)  
	{
		s = (maxVal-minVal)/(maxVal+minVal);
	}
	else if(l>0.5f)
	{
		s = (maxVal-minVal)/(2 - (maxVal+minVal)); //(maxVal-minVal > 0)?  
	}

	hue = (h>240)? 240 : ((h<0)?0:h);
	saturation = ((s>1)? 1 : ((s<0)?0:s))*240;
	luminance = ((l>1)? 1 : ((l<0)?0:l))*240;
}

void ImageProcess::blueText(IplImage* image, int minHue, int maxHue)
{
	for(int y=0; y<mProcessedImage->height; y++)
	{
		for(int x=0; x<mProcessedImage->width; x++)
		{
			uchar* ptr = (uchar*)(mProcessedImage->imageData + y * mProcessedImage->widthStep + x * 3);

			int b = ptr[0];
			int g = ptr[1];
			int r = ptr[2];

			int hue = 0;
			int saturation = 0;
			int luminance = 0;

			rgb2hsl(r,g,b,hue,saturation,luminance);


			int gray = 255;
			if(hue > minHue && hue < maxHue && luminance < 200)
			{
				gray = 0;
			}

			ptr[0] = gray;
			ptr[1] = gray;
			ptr[2] = gray;
		}
	}
}


void ImageProcess::run( ImageProcessParam* param )
{
	if(mOriginalImage == NULL || mProcessedImage == NULL || param == NULL)
	{
		return;
	}
	
	if(mProcessed)
	{
		cvReleaseImage(&mProcessedImage);
		mProcessedImage = cvCloneImage(mOriginalImage);
	}
	else
	{
		mProcessed = true;
	}

	//Assert rgb image
	if(mProcessedImage->nChannels != 3)
	{
		return;
	}

	//blueText(mProcessedImage, 120, 170);
	
	//CvRect redStampRect = redStamp(mProcessedImage);
	//cvRectangleR(mProcessedImage, redStampRect, CV_RGB(255, 0, 0));

	//CvRect tableRect;
	//tableRect.x = redStampRect.x - redStampRect.width * 2.95;
	//tableRect.width = redStampRect.x + redStampRect.width * 3.95 - tableRect.x;
	//tableRect.y = redStampRect.y + redStampRect.height * 1.10;
	//tableRect.height = redStampRect.y + redStampRect.height * 6.05 - tableRect.y;
	//cvRectangleR(mProcessedImage, tableRect, CV_RGB(255, 0, 0));

	//return;

	if(param->useGray)
	{
		IplImage* grayImage = cvCreateImage(cvGetSize(mProcessedImage), 8, 1);
		cvCvtColor(mProcessedImage, grayImage, CV_RGB2GRAY);
		cvReleaseImage(&mProcessedImage);
		mProcessedImage = grayImage;
	}
	else
	{
		return;
	}

	if(param->useCanny)
	{
		IplImage* cannyImage = cvCreateImage(cvGetSize(mProcessedImage), 8, 1);
		cvCanny(mProcessedImage, cannyImage, param->cannyThreshold1, param->cannyThreshold2, 3);
		cvReleaseImage(&mProcessedImage);
		mProcessedImage = cannyImage;
	}
	else
	{
		return;
	}

	if(param->useDilate && param->dilateIter>0)
	{
		cvDilate(mProcessedImage, mProcessedImage, 0, param->dilateIter);
	}

	if(param->useErode && param->erodeIter>0)
	{
		cvErode(mProcessedImage, mProcessedImage, 0, param->erodeIter);
	}

	CvSeq* lines = NULL;
	if(param->useHough)
	{
		lines = hough(param);
	}
	else
	{
		return;
	}


	std::vector<LineSegment> lineSegList;
	if(param->useCombine)
	{
		combine(param, lines, lineSegList);
	}
	else
	{
		return;
	}

	int minHRho = 5000;
	int maxHRho = 0;
	int minVRho = 5000;
	int maxVRho = 0;
	LineSegment minH;
	LineSegment maxH;
	LineSegment minV;
	LineSegment maxV;
	if(param->useRectangle)
	{
		rectangle(param, lineSegList, minHRho, minH, maxHRho, maxH, minVRho, minV, maxVRho, maxV);
	}
	else
	{
		return;
	}

	if(param->useNormalize)
	{
		normalize(param, minH, minV, maxV, maxH);
	}

}

CvSeq* ImageProcess::hough( ImageProcessParam* param )
{
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* lines = cvHoughLines2( mProcessedImage, storage, CV_HOUGH_PROBABILISTIC, param->houghRho, CV_PI/180 * param->houghTheta, param->houghThreshold, param->houghParam1, param->houghParam2 );

	if(param->debug)
	{
		cvReleaseImage(&mProcessedImage);
		if(param->useBackGround)
		{
			mProcessedImage = cvCloneImage(mOriginalImage);
		}
		else
		{
			mProcessedImage = cvCreateImage(cvGetSize(mOriginalImage), 8, 3);
		}

		for( int i = 0; i < lines ->total; i++ )
		{  
			CvPoint* line = ( CvPoint* )cvGetSeqElem( lines, i );
			cvLine( mProcessedImage, line[0], line[1], CV_RGB(255, 0, 0), 1, CV_AA );
		}
	}
	
	return lines;
}

void ImageProcess::combine( ImageProcessParam* param, CvSeq* lines, std::vector<LineSegment>& lineSegList)
{
	for( int i = 0; i < lines ->total; i++ )
	{  
		CvPoint* line = ( CvPoint* )cvGetSeqElem( lines, i );
		LineSegment lineSeg(line[0], line[1], param->combineTheta, param->combineRho, param->combineDistance);
		bool combined = false;
		for(int n=0; n<lineSegList.size(); n++)
		{
			if(lineSegList[n].combine(lineSeg))
			{
				combined = true;
				break;
			}
		}
		if(combined == false)
		{
			lineSegList.push_back(lineSeg);
		}
	}

	if(param->debug)
	{
		cvReleaseImage(&mProcessedImage);
		if(param->useBackGround)
		{
			mProcessedImage = cvCloneImage(mOriginalImage);
		}
		else
		{
			mProcessedImage = cvCreateImage(cvGetSize(mOriginalImage), 8, 3);
		}

		for(int n=0; n<lineSegList.size(); n++)
		{
			cvLine( mProcessedImage, lineSegList[n].point[0], lineSegList[n].point[1], CV_RGB(255, 0, 0), 1, CV_AA );
		}
	}
}

void ImageProcess::rectangle( ImageProcessParam* param, std::vector<LineSegment> &lineSegList, int &minHRho, LineSegment &minH, int &maxHRho, LineSegment &maxH, int &minVRho, LineSegment &minV, int &maxVRho, LineSegment &maxV )
{
	for(int n=0; n<lineSegList.size(); n++)
	{
		if(lineSegList[n].theta < CV_PI * 0.25 || lineSegList[n].theta > CV_PI * 0.75)//ˮƽ
		{
			if(lineSegList[n].length > param->rectangleHMinLength)
			{
				if(lineSegList[n].rho < minHRho && lineSegList[n].rho > param->rectangleTop)
				{
					minHRho = lineSegList[n].rho;
					minH = lineSegList[n];
				}
				if(lineSegList[n].rho > maxHRho && lineSegList[n].rho < mProcessedImage->height - param->rectangleBottom)
				{
					maxHRho = lineSegList[n].rho;
					maxH = lineSegList[n];
				}
			}

		}
		else
		{
			if(lineSegList[n].length > param->rectangleVMinLength 
				&& lineSegList[n].point[0].y > param->rectangleTop && lineSegList[n].point[1].y > param->rectangleTop
				&& lineSegList[n].point[0].y < mProcessedImage->height - param->rectangleBottom && lineSegList[n].point[1].y < mProcessedImage->height - param->rectangleBottom
				)
			{

				if(lineSegList[n].rho < minVRho)
				{

					minVRho = lineSegList[n].rho;
					minV = lineSegList[n];

				}
				if(lineSegList[n].rho > maxVRho)
				{
					maxVRho = lineSegList[n].rho;
					maxV = lineSegList[n];
				}
			}
		}
	}

	if(param->debug)
	{
		cvReleaseImage(&mProcessedImage);
		if(param->useBackGround)
		{
			mProcessedImage = cvCloneImage(mOriginalImage);
		}
		else
		{
			mProcessedImage = cvCreateImage(cvGetSize(mOriginalImage), 8, 3);
		}

		for(int n=0; n<lineSegList.size(); n++)
		{
			cvLine( mProcessedImage, maxV.point[0], maxV.point[1], CV_RGB(255, 0, 0), 1, CV_AA );
			cvLine( mProcessedImage, minV.point[0], minV.point[1], CV_RGB(255, 0, 0), 1, CV_AA );
			cvLine( mProcessedImage, maxH.point[0], maxH.point[1], CV_RGB(255, 0, 0), 1, CV_AA );
			cvLine( mProcessedImage, minH.point[0], minH.point[1], CV_RGB(255, 0, 0), 1, CV_AA );
		}
	}
}

void ImageProcess::normalize( ImageProcessParam* param, LineSegment &minH, LineSegment minV, LineSegment maxV, LineSegment &maxH )
{
	CvPoint points[4];
	points[0] = minH.intersect(minV);
	points[1] = minH.intersect(maxV);
	points[2] = maxH.intersect(maxV);
	points[3] = maxH.intersect(minV);

	CvPoint2D32f src[4], dst[4];
	src[0].x = points[0].x;
	src[0].y = points[0].y;
	src[1].x = points[1].x;
	src[1].y = points[1].y;
	src[2].x = points[2].x;
	src[2].y = points[2].y;
	src[3].x = points[3].x;
	src[3].y = points[3].y;

	dst[0].x = 0;
	dst[0].y = param->normalizeTop;
	dst[1].x = param->normalizeWidth;
	dst[1].y = param->normalizeTop;
	dst[2].x = param->normalizeWidth;
	dst[2].y = param->normalizeTop + param->normalizeHeight;
	dst[3].x = 0;
	dst[3].y = param->normalizeTop + param->normalizeHeight;

	CvMat* warp_mat = cvCreateMat( 3, 3, CV_32FC1 );

	cvGetPerspectiveTransform(src, dst, warp_mat);

	cvReleaseImage(&mProcessedImage);
	mProcessedImage = cvCreateImage(cvSize(param->normalizeWidth, param->normalizeTop + param->normalizeHeight), 8, 3);
	cvWarpPerspective(mOriginalImage, mProcessedImage, warp_mat);

	blueText(mProcessedImage, 130, 170);

	if(mMasks)
	{
		for(int i = 0;i<mMasks->size(); i++)
		{
			OCRMask& mask = (*mMasks)[i];
			CvRect rect = cvRect(mask.rect.x(), mask.rect.y(), mask.rect.width(), mask.rect.height());

			adjustRect(mProcessedImage, &rect);

			mask.rect.moveTo(rect.x, rect.y);
		}
	}
}

int ImageProcess::countInRect( IplImage* image, CvRect* rect )
{
	int count = 0;

	for(int y=rect->y; y<rect->y + rect->height && y < image->height; y++)
	{
		for(int x=rect->x; x<rect->x + rect->width && x <image->width; x++)
		{
			uchar* ptr = (uchar*)(mProcessedImage->imageData + y * mProcessedImage->widthStep + x * 3);

			if(ptr[0] == 0)
			{
				count++;
			}
		}
	}

	return count;
}

void ImageProcess::setMasks( QVector<OCRMask>* masks )
{
	mMasks = masks;
}

int ImageProcess::adjustRect( IplImage* image, CvRect* rect )
{
	const int moveStep = 1;
	int count = countInRect(image, rect);

	CvRect right = (*rect);
	right.x += moveStep;
	int rightCount = countInRect(image, &right);

	CvRect down = (*rect);
	down.y += moveStep;
	int downCount = countInRect(image, &down);

	if(count > rightCount && count > downCount)
	{
		return 0;
	}
	else
	{
		if(rightCount > downCount)
		{
			(*rect) = right;
		}
		else
		{
			(*rect) = down;
		}
		adjustRect(image, rect);
	}

	return 0;
}
