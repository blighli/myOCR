#include "ParamWidget.h"
#include <QtGui>

ParamWidget::ParamWidget()
{
	mainLayout = new QVBoxLayout();
	setLayout(mainLayout);

	debugGroup = addGroup(tr("Debug"));

	grayGroup = addGroup(tr("Gray"));

	cannyGroup = addGroup(tr("Canny"));
	cannyThreshold1 = addParam(tr("Threshold1"),1, 400, 50);
	cannyThreshold2 = addParam(tr("Threshold2"),1, 400, 150);
	
	dilateGroup = addGroup(tr("Dilate"));
	dilateIter = addParam(tr("Iter"),0,10, 1);
	erodeGroup = addGroup(tr("Erode"));
	erodeIter = addParam(tr("Iter"),0,10, 0);

	houghGroup = addGroup(tr("Hough"));
	houghRho = addParam(tr("Rho"),1,100, 1);
	houghTheta = addParam(tr("Theta"),1,90, 1);
	houghThreshold = addParam(tr("Threshold"),1,1000, 500);
	houghParam1 = addParam(tr("Param1"),1, 1000, 100);
	houghParam2 = addParam(tr("Param2"),1, 100, 5);

	combineGroup = addGroup(tr("Combine"));
	combineTheta = addParam(tr("Theta"),0, 180, 1);
	combineRho = addParam(tr("Rho"),0, 500, 10);
	combineDistance = addParam(tr("Distance"),0,1000, 100);

	rectangleGroup = addGroup(tr("Rectangle"));
	rectangleHorizontalLength = addParam(tr("HLen"),0, 3000, 800);
	rectangleVerticalLength = addParam(tr("VLen"),0, 2000, 800);

	backGroundGroup =  addGroup(tr("Background"));

	normalizeGroup =  addGroup(tr("Normalize"));
	normalizeTop = addParam(tr("Top"),0, 4000, 200);
	normalizeWidth = addParam(tr("Width"),0, 4000, 2000);
	normalizeHeight = addParam(tr("Height"),0, 4000, 940);

	buttonProcess = addButton(tr("Process"));
	connect(buttonProcess, SIGNAL(pressed()), this, SLOT(buttonPressed()));

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