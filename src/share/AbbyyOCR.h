#ifndef ABBYYOCR_H
#define ABBYYOCR_H

#include "Common.h"
#include <QtCore/QtCore>
#include <abbyy/MobilOCR.h>

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

	TFinePatternsPtr englishPattern;
	TFinePatternsPtr chinesePattern;
	TFineDictionaryPtr englishDict;

	CFineImage mImage;
	RECT* mMasks;
	int mMaskCount;

	char* readFile(const QString& fileName, int* size = 0);
};

#endif //ABBYYOCR_H