#ifndef OCRWIDGET_H
#define OCRWIDGET_H

#include <QWidget>
#include "Share/OCRMask.h"
#include <vector>

class QVBoxLayout;

class OCRWidget : public QWidget
{
	Q_OBJECT
public:
	OCRWidget();
	~OCRWidget();
	void setMasks(std::vector<OCRMask>* masks);
public slots:
	void update();
private slots:
	void keyChanged();
private:
	std::vector<OCRMask>* mMasks;
	QVBoxLayout* mainLayout;
};

#endif //OCRWIDGET_H