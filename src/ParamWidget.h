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

	QGroupBox* grayGroup;

	QGroupBox* threshold;
	QSlider* thresholdBlockSize;
	QSlider* thresholdMeanBias;

	QGroupBox* houghGroup;
	QSlider* houghRho;
	QSlider* houghTheta;
	QSlider* houghThreshold;
	QSlider* houghParam1;
	QSlider* houghParam2;

	QGroupBox* cornerGroup;
	QSlider* cornerDegree;
	QSlider* cornerGap;

	QGroupBox* backgroundGroup;

	QGroupBox* normalizeGroup;
	QSlider*  normalizeTopMargin;
	QSlider*  normalizeSideMargin;
	QSlider*  normalizeWidth;
	QSlider*  normalizeHeight;

	QGroupBox* blueTextGroup;
	QSlider*  blueTextMin;
	QSlider*  blueTextMax;


signals:
	void process();

private slots:
	void buttonPressed();

private:
	enum{
		SLIDER_WIDTH = 200,
		LABEL_WIDTH = 100,
		EDIT_WIDTH = 60,
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