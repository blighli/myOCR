#include "TesseractOCR.h"
#include "AppInfo.h"

using namespace tesseract;

TesseractOCR::TesseractOCR()
{
	mMasks = NULL;
	tessBaseAPI = new tesseract::TessBaseAPI(); 
	mBoxes = new QVector<QRect>();
}

TesseractOCR::~TesseractOCR()
{
	tessBaseAPI->End();
	delete mBoxes;
}

bool TesseractOCR::init( Language lang )
{
	QString tessdata = AppInfo::instance()->appDir();

	if(lang == TESSERACTOCR_CHINESE)
	{
		if (tessBaseAPI->Init(tessdata.toAscii(), "chi_sim+eng"))
		{
			return false;
		}
		tessBaseAPI->SetVariable("chop_enable", "T");
		tessBaseAPI->SetVariable("use_new_state_cost", "F");
		tessBaseAPI->SetVariable("segment_segcost_rating", "F");
		tessBaseAPI->SetVariable("enable_new_segsearch", "0");
		tessBaseAPI->SetVariable("language_model_ngram_on", "0");
		tessBaseAPI->SetVariable("textord_force_make_prop_words", "F");
	}
	else if(lang == TESSERACTOCR_ENGLISH)
	{
		if (tessBaseAPI->Init(tessdata.toAscii(), "eng"))
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}

void TesseractOCR::setImage( IplImage* image )
{
	tessBaseAPI->SetImage((uchar*)image->imageData, image->width, image->height, image->nChannels, image->widthStep);
}

void TesseractOCR::setMasks( QVector<OCRMask>* masks )
{
	mMasks = masks;
}

QString TesseractOCR::recognizeText()
{
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));
	QString allText;
	if(mMasks)
	{
		mBoxes->clear();
		for(int i=0;i<mMasks->size();i++)
		{
			OCRMask mask = mMasks->at(i);
			tessBaseAPI->SetRectangle(mask.rect.x(), mask.rect.y(), mask.rect.width(), mask.rect.height());

			if(mask.key == QString::fromLocal8Bit("购货单位")  || mask.key == QString::fromLocal8Bit("销货单位")
				|| mask.key == QString::fromLocal8Bit("发票代码")|| mask.key == QString::fromLocal8Bit("发票号码"))
			{
				tessBaseAPI->SetVariable("tessedit_char_whitelist", "0123456789");
			}
			else if(mask.key == QString::fromLocal8Bit("密码区"))
			{
				tessBaseAPI->SetVariable("tessedit_char_whitelist", "0123456789.+-*/<>");
			}
			else if(mask.key == QString::fromLocal8Bit("金额") || mask.key == QString::fromLocal8Bit("税额"))
			{
				tessBaseAPI->SetVariable("tessedit_char_whitelist", "0123456789.");
			}
			else
			{
				tessBaseAPI->SetVariable("tessedit_char_whitelist", "0123456789.+-*/<>");
			}

			Boxa* boxes = tessBaseAPI->GetComponentImages(tesseract::RIL_WORD, true, NULL, NULL);
			if(boxes)
			{
				for(int i = 0; i< boxes->n; i++)
				{
					QRect box(boxes->box[i]->x, boxes->box[i]->y, boxes->box[i]->w, boxes->box[i]->h);
					box.translate(mask.rect.x(), mask.rect.y());
					mBoxes->append(box);
				}

				char* ocrText = tessBaseAPI->GetUTF8Text();
				QString value(ocrText);

				if(mask.key == QString::fromLocal8Bit("购货单位")  || mask.key == QString::fromLocal8Bit("销货单位"))
				{
					value.replace("\n\n", "\n");
					value.replace(" ", "");

					int start = value.indexOf("\n");
					int end = value.indexOf("\n", start + 1);
					
					value = value.mid(start + 1, end - start);
				}
				else if(mask.key == QString::fromLocal8Bit("密码区"))
				{
					value.replace("\n\n", "\n");
				}

				(*mMasks)[i].value = value;
				allText += value;
			}
		}
	}
	return allText;
}

QVector<QRect>* TesseractOCR::getBoxes()
{
	return mBoxes;
}
