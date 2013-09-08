#include "ImageAdapter.h"

QImage* ImageAdapter::IplImage2QImage(const IplImage *iplImage)
{
	int height = iplImage->height;
    int width = iplImage->width;
 
    if(iplImage->depth == IPL_DEPTH_8U && iplImage->nChannels == 3)
    {
		const uchar *qImageBuffer = (const uchar*)iplImage->imageData;
		QImage img(qImageBuffer, width, height, QImage::Format_RGB888);
		return new QImage(img.rgbSwapped());
    }
	else if(iplImage->depth == IPL_DEPTH_8U && iplImage->nChannels == 1)
	{
		const uchar *qImageBuffer = (const uchar*)iplImage->imageData;
		QImage img(qImageBuffer, width, height, QImage::Format_Indexed8);
		
		QVector<QRgb> colorTable;
		for(int i = 0; i < 256; i++){
			colorTable.push_back(qRgb(i, i, i));
		}
		img.setColorTable(colorTable);
		return new QImage(img);
    }
	else
	{
      return new QImage();
    }
}