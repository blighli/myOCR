#include "AbbyyOCR.h"
#include "AppInfo.h"

AbbyyOCR::AbbyyOCR()
{
	QString abbyyDataDir = AppInfo::instance()->appDir() + "/abbyydata";
	mChinesePatternPath = abbyyDataDir + "/ChineseJapanese.rom";
	mEnglishPatternPath = abbyyDataDir + "/European.rom";
	mEnglishDictPath = abbyyDataDir + "/English.edc";
	mLicensePath = abbyyDataDir + "/Sample.license";
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
	QFile englishPatternFile(mEnglishPatternPath);
	englishPatternFile.open(QIODevice::ReadOnly);
	QByteArray englishPatternByteArray = englishPatternFile.readAll();
	englishPatternFile.close();
	TFinePatternsPtr englishPattern = englishPatternByteArray.data();

	QFile chinesePatternFile(mChinesePatternPath);
	chinesePatternFile.open(QIODevice::ReadOnly);
	QByteArray chinesePatternByteArray = chinesePatternFile.readAll();
	chinesePatternFile.close();
	TFinePatternsPtr chinesePattern = chinesePatternByteArray.data();

	QFile englishDictFile(mEnglishDictPath);
	englishDictFile.open(QIODevice::ReadOnly);
	QByteArray englishDictByteArray = englishDictFile.readAll();
	englishDictFile.close();
	TFineDictionaryPtr englishDict = englishDictByteArray.data();

	QFile licenseFile(mLicensePath);
	licenseFile.open(QIODevice::ReadOnly);
	QByteArray licenseByteArray = licenseFile.readAll();
	licenseFile.close();

	TLanguageID languages[3] = { LID_ChineseSimplified, LID_English, LID_Undefined };

	TFinePatternsPtr cjkPatterns[2];
	cjkPatterns[0] = chinesePattern;
	cjkPatterns[1] = 0;

	TFineDictionaryPtr dictionaries[2];
	dictionaries[0] = englishDict;
	dictionaries[1] = 0;

	CFineLicenseInfo licenseInfo;
	licenseInfo.LicenseData = (BYTE*)licenseByteArray.data();
	licenseInfo.DataLength = licenseByteArray.size();
	licenseInfo.ApplicationId = 0;

	TFineErrorCode errorCode = FineInitialize(0,0,0);
	errorCode = FineSetLicenseInfo( &licenseInfo );

	CFineLayout* layout = 0;
	errorCode = FineRecognizeImage(languages, englishPattern, cjkPatterns, dictionaries, &mImage, 
		FIPO_Default, FRM_Full, FRCL_Level3, &layout, 0, 0, 0);

	QString recognizedText;
	for( int blockIndex = 0; blockIndex < layout->TextBlocksCount; blockIndex++ ) {
		const CFineTextBlock& block = layout->TextBlocks[blockIndex];
		for( int lineIndex = 0; lineIndex < block.LinesCount; lineIndex++ ) {
			const CFineTextLine& line = block.Lines[lineIndex];
			for( int ch = 0; ch < line.CharCount; ch++ ) {
				recognizedText += QChar(line.Chars[ch].Unicode);
			}
			recognizedText += "\n";
		}
	}

	return recognizedText;
}

QByteArray AbbyyOCR::readFile( const QString& fileName )
{
	QFile file(fileName);
	file.open(QIODevice::ReadOnly);
	QByteArray byteArray = file.readAll();
	file.close();
	return byteArray;
}
