
#include "getContourAndCorrect.h"


getContourAndCorrect::getContourAndCorrect(void)
{
	 classNum=1;
}

getContourAndCorrect::~getContourAndCorrect(void)
{
}

void getContourAndCorrect:: fetchContourAndCorrect(Mat src)
{	 
	sourceImage=src;
	Mat dst;
	vector<Vec4i> lines;
	Line addNewLine;
	int eachClassNum[MAXCLASSNUM]={0};
	cvtColor( src, dst, CV_GRAY2BGR );
	adaptiveThreshold(src, dst, 255, CV_THRESH_BINARY, CV_THRESH_BINARY_INV,25, 10);
	
    HoughLinesP( dst, lines, 1, CV_PI/180, 80, 30, 10 );
    for( size_t i = 0; i < lines.size(); i++ )
    {
		double a=fabs(sqrt((lines[i][0]- lines[i][2])*(lines[i][0]-lines[i][2])*1.0+(lines[i][1]- lines[i][3])*(lines[i][1]- lines[i][3])));
		if(a>MINLINELONG)
		{
			addNewLine.classIndex=0;
			addNewLine.isMerged=false;
			addNewLine.point1=Point(lines[i][0], lines[i][1]);
			addNewLine.point2=Point(lines[i][2], lines[i][3]);
			if(lines[i][0]==lines[i][2])
			{
				addNewLine.slope=VERTICAL;
			}
			else	
			{
				addNewLine.slope=(lines[i][3]-lines[i][1])*1.0/(lines[i][2]-lines[i][0]);
			}
			hLine.push_back(addNewLine);
			//line( mtx, Point(lines[i][0], lines[i][1]),Point(lines[i][2], lines[i][3]), Scalar(0,0,0), 3, 8 );
		}
    }
	ClusterBySlope(eachClassNum);
    removeSmallClass(eachClassNum);
    ClusterByPosition();
	getContourAndPerspective();
}
void getContourAndCorrect::ClusterBySlope(int eachClassNum [])
{
	vector<Line>::iterator iter=hLine.begin();
	classSlope[1]=(*iter).slope;
	(*iter).classIndex=1;	
	iter++;
	eachClassNum[1]++;
	while(iter!=hLine.end())
	{
		int tempSlopeIndes=findSameSlope((*iter).slope);
		if(tempSlopeIndes)
		{
			(*iter).classIndex=tempSlopeIndes;
			eachClassNum[tempSlopeIndes]++;
		}
		else
		{
			classNum++;
			classSlope[classNum]=(*iter).slope;
			(*iter).classIndex=classNum;
			eachClassNum[classNum]++;
		}
		iter++;
	}
}
int getContourAndCorrect:: findSameSlope(double slope)
{
	int i,index;
	for(i=1;i<=classNum;i++)
	{
		if(fabs(classSlope[i]-slope)<0.1)
		{
			index=i;
			break;
		}
	}
	if(i>classNum)
		return 0;
	return index;
}
void getContourAndCorrect::removeSmallClass(int eachClassNum[])
{
	
	double pc;
	for(int i=1;i<=classNum;i++)
	{
		pc=(eachClassNum[i])*1.0/hLine.size();
		if(pc<MINCLASSNUM)
		{
			classSlope[i]=-10000;
		}
	}
	vector<Line>::iterator iter=hLine.begin();

	while(iter!=hLine.end())
	{
		if(classSlope[(*iter).classIndex]<-9999)
		{
			(*iter).classIndex=0;
		}		
	      iter++;
	}
	//drawLine();
}
void getContourAndCorrect::ClusterByPosition()
{
	vector<Line>::iterator iter;
	vector<Line>::iterator afterIter;
	for(iter=hLine.begin();iter!=hLine.end();iter++)
	{
		for(afterIter=iter+1;(afterIter!=hLine.end());afterIter++)
		{
			if(((*iter).classIndex==(*afterIter).classIndex)&&((*iter).classIndex!=0))
			{		
				if(findEmergeLine((*iter),(*afterIter)))
				{			
					(*iter).isMerged=true;			
					(*afterIter)=renewPoint((*iter),(*afterIter));
					break;		
				}
			}
		}
	}
}
//聚类后重新得到最后比较少杂线的线段
void getContourAndCorrect::getContourAndPerspective()
{
	vector<Line> classedLine;
	vector<Line>::iterator iter;
	for(iter=hLine.begin();iter!=hLine.end();iter++)
	{
		if((*iter).classIndex!=0&&(*iter).isMerged==false)
		{
			classedLine.push_back((*iter));
		}
	}

	for(vector<Line>::iterator calssedIndex=classedLine.begin();calssedIndex!=classedLine.end();calssedIndex++)
	{
		(*calssedIndex).slope=((*calssedIndex).point2.y-((*calssedIndex).point1.y))*1.0/(((*calssedIndex).point2.x)-(*calssedIndex).point1.x);
		if(fabs((*calssedIndex).slope-0)<5)
		{
			(*calssedIndex).classIndex=1;
		}
		if(((*calssedIndex).slope>9999)||fabs((*calssedIndex).slope)>=5)
		{
			(*calssedIndex).classIndex=2;
		}
		if((fabs((*calssedIndex).point1.y*1.0-0)/sourceImage.rows<0.16)||(fabs((*calssedIndex).point1.y*1.0-0)/sourceImage.rows>0.94))
		{
			(*calssedIndex).classIndex=0;
		}
	
		//垂直两边
		if(((fabs(1.0*(*calssedIndex).point1.x/sourceImage.cols)<0.0118)&&(fabs(1.0*(*calssedIndex).point2.x/sourceImage.cols)<0.0118))||(((fabs(1.0*(*calssedIndex).point1.x/sourceImage.cols)>0.992))&&((fabs(1.0*(*calssedIndex).point2.x/sourceImage.cols)>0.992))))
		{
			(*calssedIndex).classIndex=0;
		}
		double lineLong;
		lineLong=sqrt(fabs(((*calssedIndex).point2.y-(*calssedIndex).point1.y)*((*calssedIndex).point2.y-(*calssedIndex).point1.y)*1.0+((*calssedIndex).point2.x-(*calssedIndex).point1.x)*((*calssedIndex).point2.x-(*calssedIndex).point1.x)));
		if(lineLong/sourceImage.cols<0.12)
		{
			(*calssedIndex).classIndex=0;
		}
	
	}	
	vector<Line> fourLine (getOnlyContours(classedLine));	
	for(vector<Line>::iterator fourIndex=fourLine.begin();fourIndex!=fourLine.end();fourIndex++)
	{		
		(*fourIndex).slope=((*fourIndex).point2.y-((*fourIndex).point1.y))*1.0/(((*fourIndex).point2.x)-(*fourIndex).point1.x);
	}
	Mat result=sourceImage;		

	vector<Line> finalLine(getFinalyContours(fourLine));
	for(vector<Line>::iterator fourIndex=finalLine.begin();fourIndex!=finalLine.end();fourIndex++)	
	{		
		line( result, Point((*fourIndex).point1.x,(*fourIndex).point1.y),Point((*fourIndex).point2.x,(*fourIndex).point2.y), Scalar(0,0,0), 3, 8 );	
	}
		//透视
	perspective(result,finalLine);

		
	
	imwrite("resultOnSourcImg.jpg",result);
	unCorrectImage=result;
    unCorrectContourLine=finalLine;
}
void getContourAndCorrect::perspective(Mat result,vector<Line>finalLine)
{
	vector<cv::Point2f> points;	
	Size markerSize(result.size());

	points.push_back((finalLine[0].point1));	
	points.push_back((finalLine[0].point2));
	points.push_back((finalLine[1].point2));	
	points.push_back((finalLine[1].point1));

	int lineWidth=sqrt((finalLine[0].point2.y-finalLine[0].point1.y)*(finalLine[0].point2.y-finalLine[0].point1.y)*1.0+(finalLine[0].point2.x-finalLine[0].point1.x)*(finalLine[0].point2.x-finalLine[0].point1.x));
	int lineHeight=sqrt((finalLine[2].point2.y-finalLine[2].point1.y)*(finalLine[2].point2.y-finalLine[2].point1.y)*1.0+(finalLine[2].point2.x-finalLine[2].point1.x)*(finalLine[2].point2.x-finalLine[2].point1.x));
	
	CorrectContourPoint.push_back(Point2f(finalLine[0].point1.x,finalLine[0].point1.y));
	CorrectContourPoint.push_back(Point2f(finalLine[0].point1.x+lineWidth,finalLine[0].point1.y));
	CorrectContourPoint.push_back(Point2f(finalLine[0].point1.x+lineWidth,finalLine[0].point1.y+lineHeight));
	CorrectContourPoint.push_back(Point2f(finalLine[0].point1.x,finalLine[0].point1.y+lineHeight));

	Mat per; 
	Mat M = getPerspectiveTransform(points,CorrectContourPoint);   
	warpPerspective(result, per,M, markerSize);
	

	CorrectedImage=per;
	imwrite("perspective.jpg",per);
}
vector<Line> getContourAndCorrect::getFinalyContours(vector<Line>fourLine )
{
	vector<Line> stretchedLine(fourLine);
	int samex,samey;
	int lindex,vindex=0;
	for(int levi=0;levi<=1;levi++)
	{
		lindex=0;
		vindex++;
		for(int veri=2;veri<=3;veri++)
		{
			lindex++;
			if(fourLine[veri].point1.x==fourLine[veri].point2.x)
			{
				samex=fourLine[veri].point1.x;
				samey=fourLine[levi].slope*samex+fourLine[levi].point1.y-fourLine[levi].slope*fourLine[levi].point1.x;
			}
			else
			{
				samex=fabs((fourLine[levi].point1.y-fourLine[veri].point1.y+fourLine[veri].slope*fourLine[veri].point1.x-fourLine[levi].slope*fourLine[levi].point1.x)/(fourLine[levi].slope-fourLine[veri].slope));
				samey=fourLine[levi].slope*samex+fourLine[levi].point1.y-fourLine[levi].slope*fourLine[levi].point1.x;
			}
			if(1==lindex)
			{
				stretchedLine[levi].point1.x=samex;
				stretchedLine[levi].point1.y=samey;
			}
			if(2==lindex)
			{
				stretchedLine[levi].point2.x=samex;
				stretchedLine[levi].point2.y=samey;
			}
			if(1==vindex)
			{
				stretchedLine[veri].point1.x=samex;
				stretchedLine[veri].point1.y=samey;
			}
			if(2==vindex)
			{
				stretchedLine[veri].point2.x=samex;
				stretchedLine[veri].point2.y=samey;
			}
		}
	}

	return stretchedLine;
}
vector<Line> getContourAndCorrect::getOnlyContours(vector<Line> finalLine)
{
	vector <Line>::iterator iter;
	Line lLineMin;
	Line lLineMax;
	Line vLineMin;
	Line vLineMax;
	vector <Line> fourLine;
	double lDisMin=99999,lDisMax=0,vDisMin=99999,vDisMax=0;
	for(iter=finalLine.begin();iter!=finalLine.end();iter++)
	{
		
		if((*iter).classIndex==1)
		{
			if(((*iter).point1.y+(*iter).point2.y)<lDisMin)
			{
				lDisMin=((*iter).point1.y+(*iter).point2.y);
				lLineMin=(*iter);
			}
			if(((*iter).point1.y+(*iter).point2.y)>lDisMax)
			{
				lDisMax=((*iter).point1.y+(*iter).point2.y);
				lLineMax=(*iter);
			}
		}
		if((*iter).classIndex==2)
		{
			if(((*iter).point1.x+(*iter).point2.x)<vDisMin)
			{
				vDisMin=((*iter).point1.x+(*iter).point2.x);
				vLineMin=(*iter);
			}
			if(((*iter).point1.x+(*iter).point2.x)>vDisMax)
			{
				vDisMax=((*iter).point1.x+(*iter).point2.x);
				vLineMax=(*iter);
			}
		}
	}
	fourLine.push_back(lLineMin);
	fourLine.push_back(lLineMax);
	fourLine.push_back(vLineMin);
	fourLine.push_back(vLineMax);
	return fourLine;
}
bool getContourAndCorrect::findEmergeLine(Line before,Line after)
{
	bool isFound=false;
	double distace=10000;
	if(after.slope>9999)//垂直
	{
		distace=fabs((before.point2.x-after.point2.x)*1.0);
	}
	else //不垂直
	{
		distace=(fabs((after.point2.y-before.point2.y)+after.slope*(before.point2.x-after.point2.x)))/sqrt(after.slope*after.slope+1);
	}
	if(distace<=MINEMERGEDISTANCE)
		isFound=true;
	return isFound;
}
Line getContourAndCorrect:: renewPoint(Line before,Line after)
{
	int distance11,distance12,distance21,distance22,distance1,distance2;
	int maxDistance=0;
	distance11=fabs(sqrt((before.point1.y- after.point1.y)*(before.point1.y- after.point1.y)*1.0+(before.point1.x- after.point1.x)*(before.point1.x- after.point1.x))*1.0);
	if(maxDistance<distance11)
		maxDistance=distance11;
	distance12=fabs(sqrt((before.point1.y- after.point2.y)*(before.point1.y- after.point2.y)*1.0+(before.point1.x- after.point2.x)*(before.point1.x- after.point2.x))*1.0);
	if(maxDistance<distance12)
		maxDistance=distance12;
	distance21=fabs(sqrt((before.point2.y- after.point1.y)*(before.point2.y- after.point1.y)*1.0+(before.point2.x- after.point1.x)*(before.point2.x- after.point1.x))*1.0);
	if(maxDistance<distance21)
		maxDistance=distance21;
	distance22=fabs(sqrt((before.point2.y- after.point2.y)*(before.point2.y- after.point2.y)*1.0+(before.point2.x- after.point2.x)*(before.point2.x- after.point2.x))*1.0);
	if(maxDistance<distance22)
		maxDistance=distance22;

	distance1=fabs(sqrt((before.point1.y- before.point2.y)*(before.point1.y- before.point2.y)*1.0+(before.point1.x- before.point2.x)*(before.point1.x- before.point2.x))*1.0);
	if(maxDistance<distance1)
		maxDistance=distance1;
	distance2=fabs(sqrt((after.point1.y- after.point2.y)*(after.point1.y- after.point2.y)*1.0+(after.point1.x- after.point2.x)*(after.point1.x- after.point2.x))*1.0);
	if(maxDistance<distance2)
		maxDistance=distance2;
		
	
	
	if(distance11==maxDistance)
	{
		after.point2.x=before.point1.x;
		after.point2.y=before.point1.y;
	}
	else if(distance12==maxDistance)
	{
		after.point1.x=before.point1.x;
		after.point1.y=before.point1.y;
	}
	else if(distance21==maxDistance)
	{
		after.point2.x=before.point2.x;
		after.point2.y=before.point2.y;
	}
	else if(distance22==maxDistance)
	{
		after.point1.x=before.point2.x;
		after.point1.y=before.point2.y;
	}
	else if(distance1==maxDistance)
	{
		after.point1.x=before.point1.x;
		after.point1.y=before.point1.y;
		after.point2.x=before.point2.x;
		after.point2.y=before.point2.y;
	}
	else//(distance2==maxDistance)
	{
		after= after;
	}
	
	return after;
}
/*void drawLine(String path)
{
	Mat src,mtx;
	int sum=0;
	String write,huabanName;

	src=imread(imageNmae, 0);
	if(path!="resultImage.jpg")
	{
	   huabanName="F://image//write.jpg";
	}
	else
		huabanName=imageNmae;
	mtx=imread(huabanName);
	
	resize(mtx, mtx,src.size(),cv::INTER_CUBIC);

	
	vector<Line>::iterator iter=hLine.begin();
	while(iter!=hLine.end())
	{
		if((!(*iter).isMerged)&&((*iter).classIndex!=0))
		{
			sum++;
			line( mtx, Point((*iter).point1.x,(*iter).point1.y),Point((*iter).point2.x,(*iter).point2.y), Scalar(0,0,0), 3, 8 );
			//cout<<"$"<<(*iter).point1.x<<","<<(*iter).point1.y<<";"<<(*iter).point2.x<<","<<(*iter).point2.y<<";"<<endl;
		}
		iter++;
	}
	cout<<"result"<<sum<<endl;
	imwrite(path,mtx);
}
*/
