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

#include "ConfigWindow.h"
#include <klocale.h>

ConfigWindow::ConfigWindow(UpdaterCore *_core, QWidget *parent) : 
	QWidget(parent,0,Qt::WDestructiveClose) {

	initGUI();
	core = _core;
}


ConfigWindow::~ConfigWindow() {
}

void ConfigWindow::initGUI() {

	mainLayout = new QVBoxLayout(this);
	header = new HeaderWidget(this);
	serverList = new QListView(this);
	addButton = new KPushButton(i18n("Add Server"), this);
	removeButton = new KPushButton(i18n("Remove Server"), this);
	closeButton = new KPushButton(i18n("Close"), this);


	header->setDescription("<b>Add/Remove Package Servers:</b><br> You may add or remove update servers below or change your software catalog subscriptions.<br> <u>Make whatever changes you wish and press accept.</u>");

	mainLayout->addWidget(header);
	mainLayout->addWidget(serverList);
	mainLayout->setSpacing(10);

	buttonLayout = new QHBoxLayout(mainLayout);
	buttonLayout->addWidget(addButton, false, 0);
	buttonLayout->addSpacing(300);
	buttonLayout->addWidget(removeButton, false, 0);

	addButton->setMinimumHeight(30);
	removeButton->setMinimumHeight(30);
	closeButton->setMinimumHeight(30);

	connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(addButton, SIGNAL(clicked()), this, SLOT(addServer()));
	connect(removeButton, SIGNAL(clicked()), this, SLOT(removeServer()));
	connect(serverList, SIGNAL(selectionChanged(QListViewItem*)), this, SLOT(selection(QListViewItem*)));

	mainLayout->addWidget(closeButton, false, Qt::AlignRight);

	mainLayout->setMargin(10);	
	setCaption(i18n("Configure Updater"));
	resize(250,400);
	show();
}

void ConfigWindow::initList() {
}

void ConfigWindow::gotList(QValueList<Service> *servers) {
}
void ConfigWindow::addedServer(int status) {
}
void ConfigWindow::removedServer(int status) {
}
void ConfigWindow::addButtonClicked() {
}
void ConfigWindow::removeButtonClicked() {
}
void ConfigWindow::selection(QListViewItem *item) {
}
