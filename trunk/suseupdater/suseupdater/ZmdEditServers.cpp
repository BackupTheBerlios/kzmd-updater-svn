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
#include <kpushbutton.h>
#include <kdebug.h>

#include <qlayout.h>

#include "ZmdConfigWindow.h"
#include "ZmdEditServers.h"
#include "ZmdServerDialog.h"
#include "ZmdProgressDialog.h"
#include "ZmdCatalogListItem.h"

ZmdEditServers::ZmdEditServers(ZmdUpdaterCore *_core, QWidget *parent) : QWidget(parent, "EditTab", 0) {
	core = _core;

	initGUI();
	initList();
	connect(core, SIGNAL(generalFault(QString, int)), this, SLOT(serverFault(QString, int)));
}

void ZmdEditServers::initGUI() {

	mainLayout = new QVBoxLayout(this);
	serverList = new QListView(this);
	addButton = new KPushButton(i18n("Add Server"), this);
	removeButton = new KPushButton(i18n("Remove Server"), this);

	serverList->addColumn(i18n("Services/Catalogs"), 10000); //make it cover the window
	serverList->setColumnWidthMode(0, QListView::Manual);
	serverList->setTreeStepSize(30);
	serverList->setRootIsDecorated(true);
	serverList->setHScrollBarMode(QScrollView::AlwaysOff);

	/*
		Hidden Columns - We use these to store info about the services/catalogs
	*/
	serverList->addColumn("URI", 0); // Holds catalog and service URI
	serverList->setColumnWidthMode(1, QListView::Manual);
	serverList->addColumn("ID", 0); //Holds catalog and service ID
	serverList->setColumnWidthMode(2, QListView::Manual);
	serverList->addColumn("Subscribed", 0); //Holds subscription status for catalogs
	serverList->setColumnWidthMode(3, QListView::Manual);


	mainLayout->addWidget(serverList);
	mainLayout->setSpacing(10);

	buttonLayout = new QHBoxLayout(mainLayout);
	buttonLayout->addWidget(addButton, false, 0);
	buttonLayout->addSpacing(300);
	buttonLayout->addWidget(removeButton, false, 0);

	connect(addButton, SIGNAL(clicked()), this, SLOT(addButtonClicked()));
	connect(removeButton, SIGNAL(clicked()), this, SLOT(removeButtonClicked()));

	mainLayout->setMargin(10);	
}

void ZmdEditServers::initList() {

	QListViewItem *item;

	//Clear the list and re-populate it
	serverList->clear();
	removeButton->setDisabled(true); //We don't try to remove things from an empty list
	addButton->setDisabled(true); //Don't allow the user to add a server until we get the data drop

	item = new QListViewItem(serverList, i18n("Fetching service list..."));

	//Connect the signals and call the backend
	connect(core, SIGNAL(serviceListing(QValueList<Service>)), this, 
	SLOT(gotServiceList(QValueList<Service>)));
	core->getServices();

}

void ZmdEditServers::gotServiceList(QValueList<Service> servers) {
	QValueList<Service>::iterator iter;
	QListViewItem *item;

	//Disconnect this signal. If this doesn't happen we will connect it again on each iteration and end up adding many copies of each service to the list
	disconnect(core, SIGNAL(serviceListing(QValueList<Service>)), this, SLOT(gotServiceList(QValueList<Service>)));

	serverList->clear(); //get rid of the preparing item and anything else that has lingered (multiple signals)
	//Got data, re-enable the add button
	addButton->setDisabled(false);

	for (iter = servers.begin(); iter != servers.end(); iter++) {
		item = new QListViewItem(serverList, (*iter).name);
		item->setText(CONFW_URI,(*iter).uri);
		item->setText(CONFW_ID,(*iter).id);

		//Inform the user that we have not got the catalogs yet
		item->setOpen(true);
		item = new QListViewItem(item, i18n("Fetching Catalogs For Service..."));
	}

	if (servers.size() > 0) {
		removeButton->setDisabled(false); //re-enable the remove button
		connect(core, SIGNAL(catalogListing(QValueList<Catalog>)), this, 
		SLOT(gotCatalogList(QValueList<Catalog>)));
		core->getCatalogs();
	}
}

void ZmdEditServers::gotCatalogList(QValueList<Catalog> catalogs) {
	QValueList<Catalog>::iterator iter;
	ZmdCatalogListItem *item;
	QListViewItem *parentItem;

	//Disconnect this signal. If this doesn't happen, the same thing as with services happens
	disconnect(core, SIGNAL(catalogListing(QValueList<Catalog>)), this, 
				SLOT(gotCatalogList(QValueList<Catalog>)));

	if (serverList->firstChild()->childCount() != 1) {
		kdWarning() << "ERROR: We are trying to add catalogs to a list that already has them or has not had a service drop yet" << endl;
		return;
	}

	for (iter = catalogs.begin(); iter != catalogs.end(); iter++) {
		parentItem = serverList->findItem((*iter).service,CONFW_URI);
		if (parentItem == NULL) {
			continue;
		} else {
			if (parentItem->firstChild() != NULL)
				delete parentItem->firstChild(); //get rid of "Fetching" item

			item = new ZmdCatalogListItem(parentItem, (*iter).name, core);
			item->setOn((*iter).subscribed);
			item->setText(CONFW_ID, (*iter).id);
			parentItem->setOpen(true);
		}
	}
}

void ZmdEditServers::addButtonClicked() {

	QValueList<QString> list;
	ZmdServerDialog diag;
	ZmdProgressDialog prog(true, this);

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
		connect(core, SIGNAL(serviceAdded(QString,int,QString)), this, SLOT(addedServer(QString,int,QString)));
		core->addService(newServ);
	
		//Tell the user what is going on, this takes a long long time
		prog.setTitle(i18n("Adding server.."));
		prog.setDescription(i18n("We are adding a server to the updater, this may take a long time. \nPlease be patient"));

		//Connect the progress dialog signals
		connect(core, SIGNAL(progress(Progress)), &prog, SLOT(progress(Progress)));
		connect(core, SIGNAL(serviceAdded(QString,int,QString)), &prog, SLOT(finished(QString,int,QString)));
		prog.exec();
	} else {
		//We don't say you need to have a type, because the groupbox takes care of that
		KMessageBox::error(this, i18n("You need to specify a name and URL to add a server"));
	}

}

void ZmdEditServers::addedServer(QString server, int status, QString error) {
	//Got a server added, we disconnect and re-init the list or show error
	disconnect(core, SIGNAL(serviceAdded(QString,int,QString)), this, SLOT(addedServer(QString,int,QString)));
	if (status != ERROR_INVALID) {
		initList();
	} else if (status != ERROR_INVALID_TYPE) {
		KMessageBox::error(this, i18n("The type you specified for the server is invalid:") + error);	
	} else {
		KMessageBox::error(this, i18n("Sorry, the server could not be added: ") + error);
	}
}

void ZmdEditServers::removeButtonClicked() {

	if (serverList->currentItem() == NULL)
		return;

	if (serverList->currentItem()->parent() == NULL) {
		Service serv;

		serv.name = serverList->currentItem()->text(CONFW_NAME);
		serv.id = serverList->currentItem()->text(CONFW_ID);
		serv.uri = serverList->currentItem()->text(CONFW_URI);

		core->removeService(serv);
		QTimer::singleShot(1000, this, SLOT(removedServer())); //We wait 1 second to let zmd really delete the service 
		KMessageBox::information(this, i18n("Service Removed")); 
	}
}

void ZmdEditServers::removedServer() {
	initList();
}

void ZmdEditServers::serverFault(QString message, int errorCode) {
	KMessageBox::error(this, message);
}

