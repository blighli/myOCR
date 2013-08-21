#pragma once

#include <opencv2/opencv.hpp>
#include <math.h>
#include <vector>
#include <iterator>

using namespace cv;
using namespace std;
#define VERTICAL 10000
#define MAXCLASSNUM 200
#define MINLINELONG 150
#define MINEMERGEDISTANCE 50
#define MINCLASSNUM 0.006
		
struct Line
{
	int classIndex;
	bool isMerged;
	Point point1;
	Point point2;
	double slope;
};
class getContourAndCorrect
{
private:
	double classSlope[MAXCLASSNUM];
	int classNum;
	vector<Line> hLine;
public:
	vector<cv::Point2f> CorrectContourPoint;
	vector<Line> unCorrectContourLine;
	Mat CorrectedImage;
	Mat unCorrectImage;
	Mat sourceImage;

	getContourAndCorrect(void);
	~getContourAndCorrect(void);
public:
	void fetchContourAndCorrect(Mat src);

private:
	void ClusterBySlope(int eachClassNum[]);
	int findSameSlope(double slope);
	void removeSmallClass(int eachClassNum[]);
	bool findEmergeLine(Line before,Line after);
	Line renewPoint(Line before,Line after);
	void drawLine(String imageName);
	void ClusterByPosition();
	void getContourAndPerspective();
	IplImage*  convertImageToGreyscale(CvSize size,const IplImage *imageSrc);
	vector<Line>  getOnlyContours(vector<Line> finalLine);
	vector<Line> getFinalyContours(vector<Line>fourLine );
	void perspective(Mat result,vector<Line>finalLine);
};

