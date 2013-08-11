#include "ImageAdapter.h"

PIX* ImageAdapter::qImage2PIX(const QImage& qImage)
{
	PIX * pixs;

	QImage myImage = qImage.rgbSwapped();
	int width = myImage.width();
	int height = myImage.height();
	int depth = myImage.depth();
	int wpl = myImage.bytesPerLine() / 4;

	pixs = pixCreate(width, height, depth);
	pixSetWpl(pixs, wpl);
	pixSetColormap(pixs, NULL);
	l_uint32 *datas = pixs->data;

	for (int y = 0; y < height; y++) {
		l_uint32 *lines = datas + y * wpl;
		QByteArray a((const char*)myImage.scanLine(y), myImage.bytesPerLine());
		for (int j = 0; j < a.size(); j++) {
			*((l_uint8 *)lines + j) = a[j];
		}
	}

	const qreal toDPM = 1.0 / 0.0254;
	int resolutionX = myImage.dotsPerMeterX() / toDPM;
	int resolutionY = myImage.dotsPerMeterY() / toDPM;

	if (resolutionX < 300) resolutionX = 300;
	if (resolutionY < 300) resolutionY = 300;
	pixSetResolution(pixs, resolutionX, resolutionY);

	return pixEndianByteSwapNew(pixs);
}

QImage ImageAdapter::PIX2qImage(PIX* pixImage)
{
	int width = pixGetWidth(pixImage);
	int height = pixGetHeight(pixImage);
	int depth = pixGetDepth(pixImage);
	int bytesPerLine = pixGetWpl(pixImage) * 4;
	l_uint32 * datas = pixGetData(pixEndianByteSwapNew(pixImage));

	QImage::Format format;
	if (depth == 1)
		format = QImage::Format_Mono;
	else if (depth == 8)
		format = QImage::Format_Indexed8;
	else
		format = QImage::Format_RGB32;

	QImage result((uchar*)datas, width, height, bytesPerLine, format);

	// Set resolution
	l_int32 	xres, yres;
	pixGetResolution(pixImage, &xres, &yres);
	const qreal toDPM = 1.0 / 0.0254;
	result.setDotsPerMeterX(xres * toDPM);
	result.setDotsPerMeterY(yres * toDPM);

	// Handle pallete
	QVector<QRgb> _bwCT;
	_bwCT.append(qRgb(255,255,255));
	_bwCT.append(qRgb(0,0,0));

	QVector<QRgb> _grayscaleCT(256);
	for (int i = 0; i < 256; i++)  {
		_grayscaleCT.append(qRgb(i, i, i));
	}
	switch (depth) {
	case 1:
		result.setColorTable(_bwCT);
		break;
	case 8:
		result.setColorTable(_grayscaleCT);
		break;
	default:
		result.setColorTable(_grayscaleCT);
	}

	if (result.isNull()) {
		static QImage none(0,0,QImage::Format_Invalid);
		qDebug("Invalid format!!!\n");
		return none;
	}

	return result.rgbSwapped();
}