#include "TesseractOCR.h"
#include "AppInfo.h"
#include <clocale>
#include <locale>

TesseractOCR::TesseractOCR()
{
	mMasks = NULL;
	tessBaseAPI = new tesseract::TessBaseAPI(); 
	mBoxes = new std::vector<CvRect>();
}

TesseractOCR::~TesseractOCR()
{
	tessBaseAPI->End();
	delete mBoxes;
}

bool TesseractOCR::init( Language lang )
{
	std::string tessdata = AppInfo::instance()->appDir();

	if(lang == TESSERACTOCR_CHINESE)
	{
		if (tessBaseAPI->Init(tessdata.c_str(), "chi_sim+eng"))
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
		if (tessBaseAPI->Init(tessdata.c_str(), "chi_sim+eng"))
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

void TesseractOCR::setMasks( std::vector<OCRMask>* masks )
{
	mMasks = masks;
}

std::string narrow(std::wstring const& text)
{
	std::setlocale(LC_ALL, "");
	const std::locale locale("");
	typedef std::codecvt<wchar_t, char, std::mbstate_t> converter_type;
	const converter_type& converter = std::use_facet<converter_type>(locale);
	std::vector<char> to(text.length() * converter.max_length());
	std::mbstate_t state;
	const wchar_t* from_next;
	char* to_next;
	const converter_type::result result = converter.out(state, text.data(), text.data() + text.length(), from_next, &to[0], &to[0] + to.size(), to_next);
	if (result == converter_type::ok || result == converter_type::noconv) {
		const std::string s(&to[0], to_next);
		return s;
	}
	return "";
}

std::string TesseractOCR::recognizeText()
{
	std::string allText;
	if(mMasks)
	{
		mBoxes->clear();
		for(int i=0;i<mMasks->size();i++)
		{
			OCRMask mask = mMasks->at(i);
			tessBaseAPI->SetRectangle(mask.rect.x, mask.rect.y, mask.rect.width, mask.rect.height);

			if(mask.key == "购货单位" || mask.key == "销货单位"
				|| mask.key == "发票代码"|| mask.key == "发票号码")
			{
				tessBaseAPI->SetVariable("tessedit_char_whitelist", "0123456789");
			}
			else if(mask.key == "密码区")
			{
				tessBaseAPI->SetVariable("tessedit_char_whitelist", "0123456789+-*/<>");
			}
			else if(mask.key == "金额" || mask.key == "税额")
			{
				tessBaseAPI->SetVariable("tessedit_char_whitelist", "0123456789.");
			}
			else if(mask.key == "开票日期")
			{
				std::wstring wstr = L"年月日";
				std::string str = narrow(wstr)+ "0123456789";

				tessBaseAPI->SetVariable("tessedit_char_whitelist", str.c_str());
			}
			else
			{
				//tessBaseAPI->SetVariable("tessedit_char_whitelist", "0123456789.+-*/<>");
			}

			Boxa* boxes = tessBaseAPI->GetComponentImages(tesseract::RIL_WORD, true, NULL, NULL);
			if(boxes)
			{
				for(int i = 0; i< boxes->n; i++)
				{
					CvRect box = cvRect(boxes->box[i]->x, boxes->box[i]->y, boxes->box[i]->w, boxes->box[i]->h);
					box.x += mask.rect.x;
					box.y += mask.rect.y;
					mBoxes->push_back(box);
				}

				char* ocrText = tessBaseAPI->GetUTF8Text();
				std::string value(ocrText);

				if(mask.key == "购货单位"  || mask.key == "销货单位")
				{
					replaceString(value,"\n\n", "\n");
					replaceString(value," ", "");

					int start = value.find("\n");
					int end = value.find("\n", start + 1);
					if( start!= -1 && end != -1 )
					{
						value = value.substr(start + 1, end - start);
					}
					
				}
				else if(mask.key == "发票代码"|| mask.key == "发票号码")
				{
					replaceString(value, " ", "");
				}
				else if(mask.key == "密码区")
				{
					replaceString(value,"\n\n", "\n");
					replaceString(value," ", "");
					replaceString(value,">1<", "*");
				}
				else if(mask.key == "金额" || mask.key == "税额")
				{
					int start = value.find(" ");
					int end = value.length();
					value = value.substr(start + 1, end - start);
					replaceString(value," ", "");
				}
				else if(mask.key == "开票日期")
				{
					replaceString(value," ", "");
				}

				(*mMasks)[i].value = value;
				allText += value;
			}
		}
	}
	return allText;
}

std::vector<CvRect>* TesseractOCR::getBoxes()
{
	return mBoxes;
}

void TesseractOCR::replaceString( std::string& str, const std::string& oldStr, const std::string& newStr )
{
	size_t pos = 0;
	while((pos = str.find(oldStr, pos)) != std::string::npos)
	{
		str.replace(pos, oldStr.length(), newStr);
		pos += newStr.length();
	}
}
