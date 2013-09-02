#ifndef TESSERACTOCR_H
#define TESSERACTOCR_H

#include <QtCore/QtCore>
#include <leptonica/allheaders.h>
#include <tesseract/baseapi.h>
#include <opencv2/opencv.hpp>
#include "OCRMask.h"

class TesseractOCR
{
public:

	enum Language
	{
		TESSERACTOCR_CHINESE,
		TESSERACTOCR_ENGLISH
	};

	TesseractOCR();
	~TesseractOCR();

	bool init(Language lang);

	void setImage(IplImage* image);
	void setMasks(QVector<OCRMask>* masks);

	QString recognizeText();
	QVector<QRect>* getBoxes();

private:
	tesseract::TessBaseAPI* tessBaseAPI;
	QVector<OCRMask>* mMasks;
	QVector<QRect>* mBoxes;
};

#endif //TESSERACTOCR_H