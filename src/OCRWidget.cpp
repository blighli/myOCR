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

void OCRWidget::setMasks( std::vector<OCRMask>* masks )
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

		keyEdit->setText(QString::fromLocal8Bit(mMasks->at(i).key.c_str()));
		valueEdit->setText(QString::fromLocal8Bit(mMasks->at(i).value.c_str()));

		QGroupBox* groupBox = new QGroupBox();
		QVBoxLayout* groupLayout = new QVBoxLayout();
		groupBox->setLayout(groupLayout);

		groupLayout->addWidget(keyEdit);
		groupLayout->addWidget(valueEdit);
		
		mainLayout->addWidget(groupBox);

		keyEdit->setProperty("index", i);
		connect(keyEdit, SIGNAL(textChanged(const QString &)), this, SLOT(keyChanged()));
	}
	mainLayout->addStretch();
}

void OCRWidget::keyChanged()
{
	QLineEdit* keyEdit = (QLineEdit*)sender();
	int index = keyEdit->property("index").toInt();
	QString key = keyEdit->text();
	(*mMasks)[index].key = key.toLocal8Bit();
}
