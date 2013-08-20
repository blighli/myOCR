#include "AbbyyOCR.h"
#include "AppInfo.h"

AbbyyOCR::AbbyyOCR()
{
	QString abbyyDataDir = AppInfo::instance()->appDir() + "/abbyydata";
	mChinesePatternPath = abbyyDataDir + "/ChineseJapanese.rom";
	mEnglishPatternPath = abbyyDataDir + "/European.rom";
	mEnglishDictPath = abbyyDataDir + "/English.edc";
	mLicensePath = abbyyDataDir + "/Sample.license";

	mMasks = 0;
	mMaskCount = 0;

	englishPattern = readFile(mEnglishPatternPath);
	chinesePattern = readFile(mChinesePatternPath);
	englishDict = readFile(mEnglishDictPath);

	int licenseSize = 0;
	char* licenseData = readFile(mLicensePath, &licenseSize);

	CFineLicenseInfo licenseInfo;
	licenseInfo.LicenseData = (BYTE*)licenseData;
	licenseInfo.DataLength = licenseSize;
	licenseInfo.ApplicationId = 0;

	TFineErrorCode errorCode = FineInitialize(0,0,0);
	errorCode = FineSetLicenseInfo( &licenseInfo );

	delete[] licenseData;
}

AbbyyOCR::~AbbyyOCR()
{
	FineDeinitialize();

	if(mMasks)
	{
		delete[] mMasks;
	}
	delete[] englishPattern;
	delete[] chinesePattern;
	delete[] englishDict;
}

void AbbyyOCR::setImage( IplImage* image )
{
	mImage.ImageWidth = image->width;
	mImage.ImageHeight = image->height;
	mImage.ImageByteWidth = image->widthStep;
	mImage.BitsPerPixel = image->nChannels * 8;
	mImage.Resolution = 0;
	mImage.Image = (BYTE*)image->imageData;
}

QString AbbyyOCR::recognizeText()
{
	TLanguageID languages[3] = { LID_ChineseSimplified, LID_English, LID_Undefined };

	TFinePatternsPtr cjkPatterns[2];
	cjkPatterns[0] = chinesePattern;
	cjkPatterns[1] = 0;

	TFineDictionaryPtr dictionaries[2];
	dictionaries[0] = englishDict;
	dictionaries[1] = 0;

	CFineLayout* layout = 0;
	QString recognizedText;
	if(mMasks)
	{
		for(int i=0;i<mMaskCount;i++)
		{
			TFineErrorCode errorCode = FineRecognizeRegion(languages, englishPattern, cjkPatterns, dictionaries, &mImage, 1, mMasks+i,
				FIPO_Default, FRM_Full, FRCL_Level3, &layout, 0, 0, 0);

			if(layout)
			{
				for( int blockIndex = 0; blockIndex < layout->TextBlocksCount; blockIndex++ ) {
					const CFineTextBlock& block = layout->TextBlocks[blockIndex];
					for( int lineIndex = 0; lineIndex < block.LinesCount; lineIndex++ ) {
						const CFineTextLine& line = block.Lines[lineIndex];
						for( int ch = 0; ch < line.CharCount; ch++ ) {
							recognizedText += QChar(line.Chars[ch].Unicode);
							//recognizedText += "(" + QString::number(line.Chars[ch].Quality) + "%)";
						}
						recognizedText += "\n";
					}
				}
			}
		}
	}
	else
	{
		TFineErrorCode errorCode = FineRecognizeImage(languages, englishPattern, cjkPatterns, dictionaries, &mImage, 
			FIPO_Default, FRM_Full, FRCL_Level3, &layout, 0, 0, 0);

		for( int blockIndex = 0; blockIndex < layout->TextBlocksCount; blockIndex++ ) {
			const CFineTextBlock& block = layout->TextBlocks[blockIndex];
			for( int lineIndex = 0; lineIndex < block.LinesCount; lineIndex++ ) {
				const CFineTextLine& line = block.Lines[lineIndex];
				for( int ch = 0; ch < line.CharCount; ch++ ) {
					recognizedText += QChar(line.Chars[ch].Unicode);
					//recognizedText += "(" + QString::number(line.Chars[ch].Quality) + "%)";
				}
				recognizedText += "\n";
			}
		}
	}

	return recognizedText;
}

char* AbbyyOCR::readFile( const QString& fileName, int* size )
{
	QFile file(fileName);
	file.open(QIODevice::ReadOnly);
	QByteArray byteArray = file.readAll();
	file.close();

	int fileSize = byteArray.size();
	if(size != 0)
	{
		*size = fileSize;
	}
	char* fileContent = new char[fileSize];
	memcpy(fileContent, byteArray.constData(), fileSize);
	return fileContent;
}

void AbbyyOCR::setMasks( QVector<QRect>* masks)
{
	if(mMasks)
	{
		delete[] mMasks;
		mMasks = 0;
		mMaskCount = 0;
	}
	if(masks)
	{
		mMaskCount = masks->size();
		mMasks = new RECT[mMaskCount];
		for(int i=0;i<masks->size();i++)
		{
			QRect rect = masks->at(i);
			mMasks[i].left = rect.left();
			mMasks[i].right = rect.right();
			mMasks[i].top = rect.top();
			mMasks[i].bottom = rect.bottom();
		}
	}
}
