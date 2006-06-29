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

#include "ZmdConfigWindow.h"
#include "ZmdServerDialog.h"
#include "ZmdProgressDialog.h"
#include "ZmdCatalogListItem.h"

ZmdConfigWindow::ZmdConfigWindow(ZmdUpdaterCore *_core, QWidget *parent) : QWidget(parent,0,Qt::WDestructiveClose) {
	core = _core;

	initGUI();
	initList();
	connect(core, SIGNAL(generalFault(QString)), this, SLOT(serverFault(QString)));
}

ZmdConfigWindow::~ZmdConfigWindow() {
}

void ZmdConfigWindow::initGUI() {

	mainLayout = new QVBoxLayout(this);
	header = new HeaderWidget(this);
	serverList = new QListView(this);
	addButton = new KPushButton(i18n("Add Server"), this);
	removeButton = new KPushButton(i18n("Remove Server"), this);
	closeButton = new KPushButton(i18n("Close"), this);

	serverList->addColumn(i18n("Services/Catalogs"), 400);
	serverList->setTreeStepSize(30);
	serverList->setRootIsDecorated(true);

	/*
		Hidden Columns - We use these to store info about the services/catalogs
	*/
	serverList->addColumn("URI", 0); // Holds catalog and service URI
	serverList->addColumn("ID", 0); //Holds catalog and service ID
	serverList->addColumn("Subscribed", 0); //Holds subscription status for catalogs

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

void ZmdConfigWindow::initList() {
	//Clear the list and re-populate it
	serverList->clear();

	//Connect the signals and call the backend
	connect(core, SIGNAL(serviceListing(QValueList<Service>)), this, 
	SLOT(gotServiceList(QValueList<Service>)));
	connect(core, SIGNAL(catalogListing(QValueList<Catalog>)), this, 
	SLOT(gotCatalogList(QValueList<Catalog>)));
	core->getServices();

}

void ZmdConfigWindow::gotServiceList(QValueList<Service> servers) {
	QValueList<Service>::iterator iter;
	QListViewItem *item;

	for (iter = servers.begin(); iter != servers.end(); iter++) {
		item = new QListViewItem(serverList, (*iter).name);
		item->setText(CONFW_URI,(*iter).uri);
		item->setText(CONFW_ID,(*iter).id);
	}
	//Disconnect this signal. If this doesn't happen we will connect it again on each iteration and end up adding many copies of each service to the list
	disconnect(core, SIGNAL(serviceListing(QValueList<Service>)), this, SLOT(gotServiceList(QValueList<Service>)));

	if (servers.size() > 0)
		core->getCatalogs();
}

void ZmdConfigWindow::gotCatalogList(QValueList<Catalog> catalogs) {
	QValueList<Catalog>::iterator iter;
	ZmdCatalogListItem *item;
	QListViewItem *parentItem;

	for (iter = catalogs.begin(); iter != catalogs.end(); iter++) {
		parentItem = serverList->findItem((*iter).service,CONFW_URI);
		if (parentItem == NULL) {
			KMessageBox::error(this, "Catalog " + (*iter).displayName + i18n("has no service attached to it. This is is strange and you may want to look into it"));
			continue;
		} else {
			item = new ZmdCatalogListItem(parentItem, (*iter).name, core);
			item->setOn((*iter).subscribed);
			item->setText(CONFW_ID, (*iter).id);
			parentItem->setOpen(true);
		}
	}
	//Disconnect this signal. If this doesn't happen, the same thing as with services happens
	disconnect(core, SIGNAL(catalogListing(QValueList<Catalog>)), this, 
				SLOT(gotCatalogList(QValueList<Catalog>)));
}

void ZmdConfigWindow::addButtonClicked() {

	QValueList<QString> list;
	ZmdServerDialog diag;
	ZmdProgressDialog prog(true,this);

	if (diag.exec() != QDialog::Accepted)
		return;

	list = diag.getServerInfo();
	//make sure the name, type and uri are not blank
	if (list[0] != "" && list[1] != "" && list[2] != "") { 		
		Service newServ;
		newServ.name = list[0];
		newServ.uri = list[1];
		newServ.type = list[2];

		//Connect the signal and start the adding of a service
		connect(core, SIGNAL(serviceAdded(QString,int)), this, SLOT(addedServer(QString,int)));
		core->addService(newServ);
	
		//Tell the user what is going on, this takes a long long time
		prog.setTitle(i18n("Adding server.."));
		prog.setDescription(i18n("We are adding a server to the updater, this may take some time. \nPlease be patient"));

		//Connect the progress dialog signals
		connect(core, SIGNAL(progress(Progress)), &prog, SLOT(progress(Progress)));
		connect(core, SIGNAL(serviceAdded(QString,int)), &prog, SLOT(finished(QString,int)));
		prog.exec();
	} else {
		//We don't say you need to have a type, because the groupbox takes care of that
		KMessageBox::error(this, i18n("You need to specify a name and URL to add a server"));
	}

}

void ZmdConfigWindow::addedServer(QString server, int status) {
	//Got a server added, we disconnect and re-init the list or show error
	disconnect(core, SIGNAL(serviceAdded(QString,int)), this, SLOT(addedServer(QString,int)));
	if (status != ERROR_INVALID) {
		initList();
		emit(refreshUpdates());
	} else {
		KMessageBox::error(this, i18n("Sorry, the server information you entered was invalid."));
	}
}

void ZmdConfigWindow::removeButtonClicked() {

	if (serverList->currentItem()->parent() == NULL) {
		Service serv;

		serv.name = serverList->currentItem()->text(CONFW_NAME);
		serv.id = serverList->currentItem()->text(CONFW_ID);
		serv.uri = serverList->currentItem()->text(CONFW_URI);

		core->removeService(serv);
		//Re-init the list after removal, we get no return from the removal
		KMessageBox::information(this, i18n("Service Removed")); 
		emit(refreshUpdates());
		initList();
	}
}

void ZmdConfigWindow::serverFault(QString message) {
	KMessageBox::error(this, message);
}
