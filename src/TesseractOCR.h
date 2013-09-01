#ifndef TESSERACTOCR_H
#define TESSERACTOCR_H

#include <QtCore/QtCore>
#include <leptonica/allheaders.h>
#include <tesseract/baseapi.h>
#include <opencv2/opencv.hpp>

class TesseractOCR
{
public:

	enum Language
	{
		TESSERACTOCR_CHINESE,
		TESSERACTOCR_ENGLISH,
		TESSERACTOCR_CHINESE_ENGLISH,
		TESSERACTOCR_DIGIT
	};

	TesseractOCR();
	~TesseractOCR();

	bool init(Language lang);

	void setImage(IplImage* image);
	void setMasks(QVector<QRect>* masks);

	QString recognizeText();
	QVector<QRect>* getBoxes();

private:
	tesseract::TessBaseAPI* tessBaseAPI;
	QVector<QRect>* mMasks;
	QVector<QRect>* mBoxes;
};

#endif //TESSERACTOCR_H