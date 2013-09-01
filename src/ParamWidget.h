#ifndef PARAMWIDGET_H
#define PARAMWIDGET_H

#include <QWidget>

class QSlider;
class QPushButton;
class QVBoxLayout;
class QGroupBox;

class ParamWidget : public QWidget
{
	Q_OBJECT
public:
	ParamWidget();

	QGroupBox* debugGroup;

	QGroupBox* grayGroup;

	QGroupBox* cannyGroup;
	QSlider* cannyThreshold1;
	QSlider* cannyThreshold2;
	
	QGroupBox* dilateGroup;
	QSlider* dilateIter;

	QGroupBox* erodeGroup;
	QSlider* erodeIter;

	QGroupBox* houghGroup;
	QSlider* houghRho;
	QSlider* houghTheta;
	QSlider* houghThreshold;
	QSlider* houghParam1;
	QSlider* houghParam2;

	QGroupBox* combineGroup;
	QSlider* combineTheta;
	QSlider* combineRho;
	QSlider* combineDistance;

	QGroupBox* rectangleGroup;
	QSlider* rectangleHMinLength;
	QSlider* rectangleVMinLength;
	QSlider* rectangleTop;
	QSlider* rectangleBottom;

	QGroupBox* backGroundGroup;

	QGroupBox* normalizeGroup;
	QSlider* normalizeTop;
	QSlider* normalizeWidth;
	QSlider* normalizeHeight;

signals:
	void process();

private slots:
	void buttonPressed();

private:
	enum{
		SLIDER_WIDTH = 200,
		LABEL_WIDTH = 80,
		EDIT_WIDTH = 40,
		BUTTON_WIDTH = 100
	};
	QGroupBox* addGroup(QString groupName);
	QSlider* addParam(QString paramName, int min, int max, int value);
	QPushButton* addButton(QString buttonName);

	QVBoxLayout* mainLayout;
	QGroupBox* group;
	QVBoxLayout* groupLayout;

	QPushButton* buttonProcess;

};

#endif //PARAMWIDGET_H