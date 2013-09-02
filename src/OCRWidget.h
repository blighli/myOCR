#ifndef OCRWIDGET_H
#define OCRWIDGET_H

#include <QWidget>
#include "OCRMask.h"

class QVBoxLayout;

class OCRWidget : public QWidget
{
	Q_OBJECT
public:
	OCRWidget();
	~OCRWidget();
	void setMasks(QVector<OCRMask>* masks);
public slots:
	void update();
private:
	QVector<OCRMask>* mMasks;
	QVBoxLayout* mainLayout;
};

#endif //OCRWIDGET_H