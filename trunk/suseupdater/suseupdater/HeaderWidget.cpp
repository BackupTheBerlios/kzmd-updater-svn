#include "HeaderWidget.h"

#include <kiconloader.h>

#include <qlabel.h>
#include <qlayout.h>

HeaderWidget::HeaderWidget(QWidget *parent) : QWidget(parent) {

	layout = new QHBoxLayout(this);
	suseLogo = new QLabel(this);
	text = new QLabel(this);

	suseLogo->setPixmap(UserIcon("suse_logo.png"));

	layout->addWidget(suseLogo,false,Qt::AlignLeft);
	layout->addWidget(text,true,Qt::AlignRight);
	layout->insertSpacing(1,30);

	text->setText("<b>Default</b><br>");
	text->setMinimumWidth(400);
	show();
}

void HeaderWidget::setDescription(QString description) {
	text->setText(description);
}

HeaderWidget::~HeaderWidget() {
}
