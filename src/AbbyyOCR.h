#ifndef ABBYYOCR_H
#define ABBYYOCR_H

#include <QtCore/QtCore>
#include <abbyy/MobilOCR.h>
#include <opencv2/opencv.hpp>

class AbbyyOCR
{
public:
	AbbyyOCR();
	~AbbyyOCR();
	void setImage(IplImage* image);
	void setMasks(QVector<QRect>* masks);

	QString recognizeText();
private:
	QString mChinesePatternPath;
	QString mEnglishPatternPath;
	QString mEnglishDictPath;
	QString mLicensePath;

	CFineImage mImage;
	RECT* mMasks;
	int mMaskCount;
	QByteArray readFile(const QString& fileName);
};

#endif //ABBYYOCR_H