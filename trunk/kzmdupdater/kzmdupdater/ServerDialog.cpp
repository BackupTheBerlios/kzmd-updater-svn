/* 
   Copyright (C) 2006 Narayan Newton <narayannewton@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/


#include "ServerDialog.h"
#include <qradiobutton.h>

enum { TYPE_ZYPP=1, TYPE_YUM, TYPE_APT };

ServerDialog::ServerDialog(QWidget *parent) : QDialog(parent) {
	initGUI();
}

ServerDialog::~ServerDialog() {
}

QValueList<QString> ServerDialog::getServerInfo() {
	QValueList<QString> list;
	list.append(name);
	list.append(server);
	switch (typeGroup->selectedId()) {
		case TYPE_ZYPP:
			list.append("zypp");
			break;
		case TYPE_YUM:
			list.append("yum");
			break;
		case TYPE_APT:
			list.append("apt-rpm"); //FIX
			break;
	}
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
	typeGroup = new QButtonGroup(this);

	layout->addWidget(nameLabel,0,0);
	layout->addWidget(nameEdit,0,0);
	layout->addWidget(serverLabel,0,0);
	layout->addWidget(serverEdit,0,0);
	layout->addWidget(typeGroup,0,0);
	layout->setMargin(10);
	layout->setSpacing(5);

	typeGroup->setExclusive(true);
	typeGroup->insert(new QRadioButton("ZYPP",this), TYPE_ZYPP);
	typeGroup->insert(new QRadioButton("YUM",this), TYPE_YUM);
	typeGroup->insert(new QRadioButton("APT-RPM",this), TYPE_APT);
	typeGroup->setButton(TYPE_ZYPP);
	typeGroup->setTitle(i18n("Server Type"));
	typeGroup->setOrientation(Qt::Horizontal);

	buttonLayout = new QHBoxLayout(layout);
	buttonLayout->addWidget(cancelButton,0,Qt::AlignLeft);
	buttonLayout->addSpacing(100);
	buttonLayout->addWidget(addButton,0, Qt::AlignRight);
	addButton->setDefault(true);
	cancelButton->setMinimumHeight(30);
	addButton->setMinimumHeight(30);

	connect(addButton, SIGNAL(clicked()), this, SLOT(addButtonClicked()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelButtonClicked()));

	layout->setMargin(20);
	resize(400,100);
	setCaption(i18n("Add Server"));
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
