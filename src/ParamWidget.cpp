#include "ParamWidget.h"
#include <QtGui>

ParamWidget::ParamWidget()
{
	mainLayout = new QVBoxLayout();
	setLayout(mainLayout);

	grayGroup = addGroup(tr("Gray"));

	threshold = addGroup(tr("Threshold"));
	thresholdBlockSize = addParam(tr("BlockSize"),1, 1000, 100);
	thresholdMeanBias = addParam(tr("MeanBias"),1, 1000, 1);
	
	houghGroup = addGroup(tr("Hough"));
	houghRho = addParam(tr("Rho"),1,100, 1);
	houghTheta = addParam(tr("Theta"),1,90, 1);
	houghThreshold = addParam(tr("Threshold"),1,1000, 50);
	houghParam1 = addParam(tr("Param1"),1, 1000, 50);
	houghParam2 = addParam(tr("Param2"),1, 100, 1);

	cornerGroup = addGroup(tr("Corner"));
	cornerDegree = addParam(tr("Degree"),0, 90, 80);
	cornerGap = addParam(tr("Gap"),0, 100, 10);

	backgroundGroup =  addGroup(tr("Background"));

	normalizeGroup =  addGroup(tr("Normalize"));
	normalizeTopMargin = addParam(tr("Top"),0, 1000, 200);
	normalizeSideMargin = addParam(tr("Width"),0, 1000, 100);
	normalizeWidth = addParam(tr("Width"),0, 4000, 2000);
	normalizeHeight = addParam(tr("Height"),0, 4000, 940);

	blueTextGroup =  addGroup(tr("BlueText"));
	blueTextMin = addParam(tr("Min"),0, 240, 130);
	blueTextMax = addParam(tr("Max"),0, 240, 170);

	setFixedSize(sizeHint());
}

void ParamWidget::buttonPressed()
{
	emit process();
}

QGroupBox* ParamWidget::addGroup(QString groupName)
{
	group = new QGroupBox(groupName);
	group->setCheckable(true);
	mainLayout->addWidget(group);
	groupLayout = new QVBoxLayout();
	group->setLayout(groupLayout);

	return group;
}

QSlider* ParamWidget::addParam(QString paramName, int min, int max, int value)
{
	QHBoxLayout* lineLayout = new QHBoxLayout();
	QLabel* label = new QLabel(paramName);
	label->setFixedWidth(LABEL_WIDTH);
	lineLayout->addWidget(label);

	QSlider* slider = new QSlider(Qt::Horizontal);
	slider->setTickPosition(QSlider::TicksAbove);
	slider->setRange(min, max);
	slider->setValue(value);
	slider->setFixedWidth(SLIDER_WIDTH);
	lineLayout->addWidget(slider);
	label->setBuddy(slider);

	QSpinBox* spinBox = new QSpinBox();
	spinBox->setFixedWidth(EDIT_WIDTH);
	spinBox->setRange(min, max);
	spinBox->setValue(value);
	lineLayout->addWidget(spinBox);
	groupLayout->addLayout(lineLayout);
	
	connect(slider, SIGNAL(sliderMoved(int)), spinBox, SLOT(setValue(int)));
	connect(spinBox, SIGNAL(valueChanged(int)), slider, SLOT(setValue(int)));

	return slider;
}


QPushButton* ParamWidget::addButton(QString buttonName)
{
	QHBoxLayout* lineLayout = new QHBoxLayout();
	QPushButton* button = new QPushButton(buttonName);
	button->setFixedWidth(BUTTON_WIDTH);
	lineLayout->addStretch(1);
	lineLayout->addWidget(button);
	lineLayout->addStretch(1);
	mainLayout->addLayout(lineLayout);

	return button;
}