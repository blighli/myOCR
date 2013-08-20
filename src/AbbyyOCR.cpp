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
	TFinePatternsPtr englishPattern = readFile(mEnglishPatternPath).data();
	TFinePatternsPtr chinesePattern = readFile(mChinesePatternPath).data();
	TFineDictionaryPtr englishDict = readFile(mEnglishDictPath).data();

	TLanguageID languages[3] = { LID_ChineseSimplified, LID_English, LID_Undefined };

	TFinePatternsPtr cjkPatterns[2];
	cjkPatterns[0] = chinesePattern;
	cjkPatterns[1] = 0;

	TFineDictionaryPtr dictionaries[2];
	dictionaries[0] = englishDict;
	dictionaries[1] = 0;

	QByteArray licenseByteArray = readFile(mLicensePath);
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
