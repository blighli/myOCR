#include "ImageProcess.h"

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

CvRect ImageProcess::findRedStampRect( IplImage* image, int minPixCount, int minPixStack, int minRowHitStack, int minRowMissStack )
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


const CvRect& ImageProcess::findTableRect(const CvRect &redStampRect )
{
	CvRect tableRect;
	tableRect.x = redStampRect.x - redStampRect.width * 2.95;
	tableRect.width = redStampRect.x + redStampRect.width * 3.95 - tableRect.x;
	tableRect.y = redStampRect.y + redStampRect.height * 1.10;
	tableRect.height = redStampRect.y + redStampRect.height * 6.05 - tableRect.y;
	return tableRect;
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

int ImageProcess::limit(int value, int min, int max)
{
	if(value < min)
	{
		value = min;
	}
	else if(value > max)
	{
		value = max;
	}
	return value;
}

CvRect ImageProcess::makeRect(int centerX, int centerY, int width, int height)
{
	if(height == 0)
	{
		height = width;
	}

	int left = centerX- width / 2;
	int top = centerY - height / 2;
	
	left = left>0?left:0;
	top = top>0?top:0;

	return cvRect(left, top, width, height);
}

void ImageProcess::findCornerRects(CvRect* cornerRects, const CvRect& rect, float rate)
{
	int cornerRectWidth = rect.width * rate;
	cornerRects[0] = makeRect(rect.x, rect.y, cornerRectWidth, cornerRectWidth);
	cornerRects[1] = makeRect(rect.x + rect.width, rect.y, cornerRectWidth, cornerRectWidth);
	cornerRects[2] = makeRect(rect.x + rect.width, rect.y + rect.height, cornerRectWidth, cornerRectWidth);
	cornerRects[3] = makeRect(rect.x, rect.y + rect.height, cornerRectWidth, cornerRectWidth);
}

void findBorder(IplImage* image)
{
	int borderWidth = 4;
	for(int y=0; y<image->height; y++)
	{
		for(int x=0; x<image->width; x++)
		{
			uchar* ptr = (uchar*)(image->imageData + y * image->widthStep + x);


		}
	}
}


int ImageProcess::run( ImageProcessParam* param )
{
	if(mOriginalImage == NULL || mProcessedImage == NULL || param == NULL)
	{
		return -1;
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
		return -1;
	}

	CvRect redStampRect = findRedStampRect(mProcessedImage);
	////cvRectangleR(mProcessedImage, redStampRect, CV_RGB(255, 0, 0));

	CvRect tableRect = findTableRect(redStampRect);
	////cvRectangleR(mProcessedImage, tableRect, CV_RGB(255, 0, 0));

	CvRect cornerRects[4];
	findCornerRects(cornerRects, tableRect, 0.1);
	//for(int i=0; i<4; i++)
	//{
	//	//cvRectangleR(mProcessedImage, cornerRects[i], CV_RGB(255, 0, 0));
	//}
	CvPoint* cornerPoints[4];
	for(int i=0; i<4; i++)
	{
		cornerPoints[i] = NULL;
	}

	for(int i=0; i<4; i++)
	{
		cvSetImageROI(mProcessedImage, cornerRects[i]);

		IplImage* grayImage = cvCreateImage(cvGetSize(mProcessedImage), 8, 1);
		IplImage* outImage = cvCreateImage(cvGetSize(mProcessedImage), 8, 3);

		//灰度化
		if(param->gray)
		{
			cvCvtColor(mProcessedImage, grayImage, CV_RGB2GRAY);
			if(param->debug)
			{
				cvCvtColor(grayImage, outImage, CV_GRAY2BGR);
			}
		}
		
		//二值化
		if(param->threshold && param->gray)
		{
			if( param->thresholdBlockSize % 2 == 0)
			{
				param->thresholdBlockSize++;
			}
			cvAdaptiveThreshold(grayImage, grayImage, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY_INV, param->thresholdBlockSize, param->thresholdMeanBias);
			if(param->debug)
			{
				cvCvtColor(grayImage, outImage, CV_GRAY2BGR);
			}
		}
		

		//霍夫变换
		CvSeq* lines = NULL;
		if(param->hough && param->threshold && param->gray)
		{
			lines = hough(grayImage, param);
			if(param->debug)
			{
				if(param->background)
				{
					cvCopy(mProcessedImage, outImage);
				}
				else
				{
					cvSetZero(outImage);
				}
				for( int i = 0; i < lines ->total; i++ )
				{  
					CvPoint* line = ( CvPoint* )cvGetSeqElem( lines, i );
					cvLine( outImage, line[0], line[1], CV_RGB(255, 0, 0), 1, CV_AA );
				}
			}
		}

		//寻找角点
		if(param->corner && param->hough && param->threshold && param->gray)
		{
			CvPoint point;
			if(findCornerPoint(param, lines, &point, param->cornerDegree, param->cornerGap))
			{
				cornerPoints[i] = new CvPoint;
				cornerPoints[i]->x = point.x;
				cornerPoints[i]->y = point.y;


				if(param->debug)
				{
					if(param->background)
					{
						cvCopy(mProcessedImage, outImage);
					}
					else
					{
						cvSetZero(outImage);
					}

					int radius = 30;
					cvCircle(outImage, point, radius, CV_RGB(255, 0, 0));
					cvLine( outImage, cvPoint(point.x - radius/2, point.y), cvPoint(point.x + radius/2, point.y), CV_RGB(255, 0, 0), 1, CV_AA );
					cvLine( outImage, cvPoint(point.x, point.y - radius/2), cvPoint(point.x, point.y + radius/2), CV_RGB(255, 0, 0), 1, CV_AA );
				}
			}
		}

		if(param->debug)
		{
			cvCopy(outImage, mProcessedImage);
		}
	}

	cvResetImageROI(mProcessedImage);

	for(int i=0; i<4; i++)
	{
		if(cornerPoints[i] == NULL)
		{
			return -1;
		}
		else
		{
			cornerPoints[i]->x += cornerRects[i].x;
			cornerPoints[i]->y += cornerRects[i].y;
		}
	}

	if(param->normalize)
	{
		normalize(param, cornerPoints);
	}

	for(int i=0; i<4; i++)
	{
		if(cornerPoints[i] != NULL)
		{
			delete cornerPoints[i];
		}
	}

	/*if(param->blueText)
	{
		blueText(mProcessedImage, param->blueTextMin, param->blueTextMax);
	}*/

	if(param->blueText)
	{
		if( param->blueTextMin % 2 == 0)
		{
			param->blueTextMin++;
		}
		IplImage* grayImage = cvCreateImage(cvGetSize(mProcessedImage), 8, 1);
		cvCvtColor(mProcessedImage, grayImage, CV_RGB2GRAY);
		cvAdaptiveThreshold(grayImage, grayImage, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, param->blueTextMin, param->blueTextMax);
		cvReleaseImage(&mProcessedImage);
		mProcessedImage = grayImage;
	}

	if(mMasks)
	{
		for(int i = 0;i<mMasks->size(); i++)
		{
			OCRMask& mask = (*mMasks)[i];
			CvRect rect = cvRect(mask.rect.x, mask.rect.y, mask.rect.width, mask.rect.height);

			adjustRect(mProcessedImage, &rect);

			mask.rect.x = rect.x;
			mask.rect.y = rect.y;
		}
	}
	return 0;
}

CvSeq* ImageProcess::hough(IplImage* image, ImageProcessParam* param )
{
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* lines = cvHoughLines2( image, storage, CV_HOUGH_PROBABILISTIC, param->houghRho, CV_PI/180 * param->houghTheta,
		param->houghThreshold, param->houghParam1, param->houghParam2 );
	return lines;
}

bool ImageProcess::findCornerPoint(ImageProcessParam* param, CvSeq* lines, CvPoint* point, int degree, int gap)
{
	int left = 9999;
	int found = false;

	for( int i = 0; i < lines ->total; i++ )
	{  
		for( int j = 0; j < lines ->total; j++ )
		{ 
			if(i > j)
			{
				LineSegment lineA(( CvPoint* )cvGetSeqElem( lines, i ));
				LineSegment lineB(( CvPoint* )cvGetSeqElem( lines, j ));

				CvPoint crossPoint;
				if( lineA.cross(lineB, &crossPoint, degree, gap) )
				{
					found = true;
					if(crossPoint.x < left)
					{
						left = crossPoint.x;
						*point = crossPoint;
					}
				}
			}
		}
	}

	return found;
}

void ImageProcess::normalize( ImageProcessParam* param, CvPoint** cornerPoints)
{
	CvPoint2D32f src[4], dst[4];
	src[0].x = cornerPoints[0]->x;
	src[0].y = cornerPoints[0]->y;
	src[1].x = cornerPoints[1]->x;
	src[1].y = cornerPoints[1]->y;
	src[2].x = cornerPoints[2]->x;
	src[2].y = cornerPoints[2]->y;
	src[3].x = cornerPoints[3]->x;
	src[3].y = cornerPoints[3]->y;

	dst[0].x = param->normalizeSideMargin;
	dst[0].y = param->normalizeTopMargin;
	dst[1].x = param->normalizeWidth + param->normalizeSideMargin;
	dst[1].y = param->normalizeTopMargin;
	dst[2].x = param->normalizeWidth + param->normalizeSideMargin;
	dst[2].y = param->normalizeTopMargin + param->normalizeHeight;
	dst[3].x = param->normalizeSideMargin;
	dst[3].y = param->normalizeTopMargin + param->normalizeHeight;

	CvMat* warp_mat = cvCreateMat( 3, 3, CV_32FC1 );

	cvGetPerspectiveTransform(src, dst, warp_mat);

	cvReleaseImage(&mProcessedImage);
	mProcessedImage = cvCreateImage(cvSize(param->normalizeWidth + param->normalizeSideMargin * 2, param->normalizeTopMargin + param->normalizeSideMargin + param->normalizeHeight), 8, 3);
	cvWarpPerspective(mOriginalImage, mProcessedImage, warp_mat);
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

void ImageProcess::setMasks( std::vector<OCRMask>* masks )
{
	mMasks = masks;
}

int ImageProcess::adjustRect( IplImage* image, CvRect* rect )
{
	const int moveStep = 10;
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

