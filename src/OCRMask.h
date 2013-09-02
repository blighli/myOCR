#ifndef OCRMASK_H
#define OCRMASK_H

#include <QtCore/QtCore>

class OCRMask
{
public:
	QRect rect;
	QString key;

	OCRMask();
};

#endif //OCRMASK_H