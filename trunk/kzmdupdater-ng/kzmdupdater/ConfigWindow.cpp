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

#include <klocale.h>
#include <kmessagebox.h>
#include <iostream>

#include "ConfigWindow.h"
#include "ServerDialog.h"
#include "ProgressDialog.h"
#include "CatalogListItem.h"

using namespace std;

ConfigWindow::ConfigWindow(ZmdUpdaterCore *_core, QWidget *parent) : 
	QWidget(parent,0,Qt::WDestructiveClose) {
	core = _core;

	initGUI();
	initList();
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

	serverList->addColumn(i18n("Services/Catalogs"), 400);
	serverList->setTreeStepSize(30);
	serverList->setRootIsDecorated(true);
	serverList->addColumn("URI", 0); // Hidden column to hold uri for searching
	serverList->addColumn("ID", 0);
	serverList->addColumn("Subscribed", 0); //Yet another hidden column, for catalogs this time

	header->setDescription(i18n("<b>Add/Remove Package Servers:</b><br> You may add or remove update servers below or change your software catalog subscriptions.<br> <u>Make whatever changes you wish and press accept.</u>"));

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
	connect(addButton, SIGNAL(clicked()), this, SLOT(addButtonClicked()));
	connect(removeButton, SIGNAL(clicked()), this, SLOT(removeButtonClicked()));

	mainLayout->addWidget(closeButton, false, Qt::AlignRight);

	mainLayout->setMargin(10);	
	setCaption(i18n("Configure Updater"));
	resize(250,400);
}

void ConfigWindow::initList() {
	serverList->clear();
	core->getServices();
	connect(core, SIGNAL(serviceListing(QValueList<Service>)), this, SLOT(gotServiceList(QValueList<Service>)));
	connect(core, SIGNAL(catalogListing(QValueList<Catalog>)), this, SLOT(gotCatalogList(QValueList<Catalog>)));

}

void ConfigWindow::gotServiceList(QValueList<Service> servers) {
	QValueList<Service>::iterator iter;
	QListViewItem *item;

	for (iter = servers.begin(); iter != servers.end(); iter++) {
		item = new QListViewItem(serverList, (*iter).name);
		item->setText(CONFW_URI,(*iter).uri);
		item->setText(CONFW_ID,(*iter).id);
	}
	disconnect(core, SIGNAL(serviceListing(QValueList<Service>)), this, SLOT(gotServiceList(QValueList<Service>)));
	if (servers.size() > 0)
		core->getCatalogs();
}

void ConfigWindow::gotCatalogList(QValueList<Catalog> catalogs) {
	QValueList<Catalog>::iterator iter;
	CatalogListItem *item;
	QListViewItem *parentItem;

	for (iter = catalogs.begin(); iter != catalogs.end(); iter++) {
		parentItem = serverList->findItem((*iter).service,CONFW_URI);
		if (parentItem == NULL) {
			KMessageBox::error(this, "You have a catalog that has no service attached to it. This is is strange and you may want to look into it");
			continue;
		} else {
			item = new CatalogListItem(parentItem, (*iter).name, core);
			item->setOn((*iter).subscribed);
			item->setText(CONFW_ID, (*iter).id);
			parentItem->setOpen(true);
		}
	}
	disconnect(core, SIGNAL(catalogListing(QValueList<Catalog>)), this, 
				SLOT(gotCatalogList(QValueList<Catalog>)));
}

void ConfigWindow::addButtonClicked() {

	QValueList<QString> list;
	ServerDialog diag;
	ProgressDialog prog(true,this);

	diag.exec();
	list = diag.getServerInfo();
	if (list.front() != "" && list.back() != "") {
		Service newServ;
		newServ.name = list[0];
		newServ.uri = list[1];
		newServ.type = list[2];
		core->addService(newServ);
		connect(core, SIGNAL(serviceAdded(QString,int)), this, SLOT(addedServer(QString,int)));

		prog.setTitle(i18n("Adding server.."));
		prog.setDescription(i18n("We are adding a server to the updater, this may take some time. \nPlease be patient"));
		prog.connectProgress(core, SIGNAL(progress(Progress)));
		prog.connectFinished(core, SIGNAL(serviceAdded(QString,int)));
		prog.exec();

	}

}

void ConfigWindow::addedServer(QString server, int status) {
	if (status != ERROR_INVALID) {
		initList();
	} else {
		KMessageBox::error(this, i18n("Sorry, the server information you entered was invalid."));
	}
	disconnect(core, SIGNAL(serviceAdded(QString,int)), this, SLOT(addedServer(QString,int)));

}

void ConfigWindow::removeButtonClicked() {
	Service serv;

	serv.name = serverList->currentItem()->text(CONFW_NAME);
	serv.id = serverList->currentItem()->text(CONFW_ID);
	serv.uri = serverList->currentItem()->text(CONFW_URI);
	core->removeService(serv);
	connect(core, SIGNAL(serviceRemoved(QString,int)), this, SLOT(removedServer(QString,int)));
}

void ConfigWindow::removedServer(QString server, int status) {
	QListViewItem *item;

	if (status == ERROR_NONE)
		if ((item = serverList->findItem(server,CONFW_NAME)) != NULL)
			delete item;
}
