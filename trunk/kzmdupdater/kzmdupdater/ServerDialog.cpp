#include "ServerDialog.h"


ServerDialog::ServerDialog(QWidget *parent) : QDialog(parent) {
	initGUI();
}

ServerDialog::~ServerDialog() {
}

QValueList<QString> ServerDialog::getServerInfo() {
	QValueList<QString> list;
	list.append(name);
	list.append(server);
	return list;
}

void ServerDialog::initGUI() {

	layout = new QVBoxLayout(this);
	nameLabel = new QLabel(i18n("Please enter server name below"), this);
	serverLabel = new QLabel(i18n("Please enter server URL below"), this);
	nameEdit = new KLineEdit(this);
	serverEdit = new KLineEdit(this);
	cancelButton = new KPushButton(i18n("Cancel"), this);
	addButton = new KPushButton(i18n("Add"), this);

	layout->addWidget(nameLabel,0,0);
	layout->addWidget(nameEdit,0,0);
	layout->addWidget(serverLabel,0,0);
	layout->addWidget(serverEdit,0,0);
	layout->setMargin(10);
	layout->setSpacing(5);

	buttonLayout = new QHBoxLayout(layout);
	buttonLayout->addWidget(cancelButton,0,Qt::AlignLeft);
	buttonLayout->addSpacing(100);
	buttonLayout->addWidget(addButton,0, Qt::AlignRight);
	cancelButton->setMinimumHeight(30);
	addButton->setMinimumHeight(30);

	connect(addButton, SIGNAL(clicked()), this, SLOT(addButtonClicked()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelButtonClicked()));

	resize(200,100);
	show();
}

void ServerDialog::addButtonClicked() {
	name = nameEdit->text();
	server = serverEdit->text();
	close();
}

void ServerDialog::cancelButtonClicked() {
	name = "";
	server = "";
	close();
}
