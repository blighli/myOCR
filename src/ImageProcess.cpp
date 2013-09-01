#include "ImageProcess.h"

ImageProcess::ImageProcess()
{
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

	if(param->useGray)
	{
		IplImage* grayImage = cvCreateImage(cvGetSize(mProcessedImage), 8, 1);
		cvCvtColor(mProcessedImage, grayImage, CV_RGB2GRAY);
		cvReleaseImage(&mProcessedImage);
		mProcessedImage = grayImage;
	}

	//Assert gray is performed
	if(mProcessedImage->nChannels != 1)
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
}
