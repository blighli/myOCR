#include "ParamWidget.h"
#include <QtGui>

ParamWidget::ParamWidget()
{
	mainLayout = new QVBoxLayout();
	setLayout(mainLayout);

	cannyGroup = addGroup(tr("Canny"));
	cannyThreshold1 = addParam(tr("Threshold1"),1, 400, 50);
	cannyThreshold2 = addParam(tr("Threshold2"),1, 400, 150);
	
	houghGroup = addGroup(tr("Hough"));
	houghThreshold = addParam(tr("Threshold"),1,400, 100);
	houghParam1 = addParam(tr("Param1"),1, 400, 50);
	houghParam2 = addParam(tr("Param2"),1, 400, 5);

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