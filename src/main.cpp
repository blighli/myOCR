#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

int main(int argc, char** argv)
{
	QCoreApplication app(argc, argv);
	qDebug()<<"hello qt!";
	app.exec();
	
	char *outText;
	tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
	if (api->Init(NULL, "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
    }
}