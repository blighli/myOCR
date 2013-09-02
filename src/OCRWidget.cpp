#include "OCRWidget.h"
#include <QtGui>

OCRWidget::OCRWidget()
{
	mMasks = NULL;
	mainLayout = new QVBoxLayout();
	setLayout(mainLayout);
}

OCRWidget::~OCRWidget()
{
	if(mainLayout)
	{
		delete mainLayout;
	}
}

void OCRWidget::setMasks( QVector<OCRMask>* masks )
{
	mMasks = masks;
}

void OCRWidget::update()
{
	if(mMasks == NULL)
	{
		return;
	}

	while (QLayoutItem* item = mainLayout->takeAt(0))
    {
		if (QWidget* widget = item->widget())
		{
                delete widget;
        }
        delete item;
    }

	for(int i=0; i<mMasks->size(); i++)
	{
		QLineEdit* keyEdit = new QLineEdit();
		QTextEdit* valueEdit = new QTextEdit();
		valueEdit->setFixedHeight(100);

		keyEdit->setText(mMasks->at(i).key);
		valueEdit->setText(mMasks->at(i).value);

		QGroupBox* groupBox = new QGroupBox();
		QVBoxLayout* groupLayout = new QVBoxLayout();
		groupBox->setLayout(groupLayout);

		groupLayout->addWidget(keyEdit);
		groupLayout->addWidget(valueEdit);
		
		mainLayout->addWidget(groupBox);
	}
	mainLayout->addStretch();
}
