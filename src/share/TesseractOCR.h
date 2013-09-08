#ifndef TESSERACTOCR_H
#define TESSERACTOCR_H

#include "Common.h"
#include "OCRMask.h"
#include <vector>
#include <string>

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
	void setMasks(std::vector<OCRMask>* masks);

	std::string recognizeText();
	std::vector<CvRect>* getBoxes();

private:
	void replaceString(std::string& str, const std::string& oldStr, const std::string& newStr);

	tesseract::TessBaseAPI* tessBaseAPI;
	std::vector<OCRMask>* mMasks;
	std::vector<CvRect>* mBoxes;
};

#endif //TESSERACTOCR_H