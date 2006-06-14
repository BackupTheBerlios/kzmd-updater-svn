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
