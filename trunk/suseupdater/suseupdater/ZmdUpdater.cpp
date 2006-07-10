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

#include <kprocess.h>
#include <kdebug.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kapp.h>

#include <stdio.h>
#include <string.h>

#include "ZmdUpdater.h"
#include "MainWindow.h"
#include "ZmdInstallWindow.h"
#include "ZmdConfigWindow.h"
#include "UpdateListItem.h"

ZmdUpdater::ZmdUpdater() : Updater() {

	//Init and auth the core
	core = new ZmdUpdaterCore(this);
	authorizeCore();

	//Connect core signals
	connect(core, SIGNAL(updateListing(QValueList<Package>)), this, SLOT(gotUpdateListing(QValueList<Package>)));
	connect(core, SIGNAL(patchListing(QValueList<Patch>)), this, SLOT(gotPatchListing(QValueList<Patch>)));
	connect(core, SIGNAL(packageInfo(Package)), this, SLOT(gotPackageInfo(Package)));
	connect(core, SIGNAL(generalFault(QString)), this, SLOT(error(QString)));
}

/*

	Slots recieving signals from the mainwindow 

*/

void ZmdUpdater::populateUpdateList(QListView *updateList) {

	tempList = updateList;
	emit(updateApplet(APPLET_NO_UPDATES));
	connect(core, SIGNAL(serviceListing(QValueList<Service>)), this, SLOT(gotServiceListing(QValueList<Service>)));
	core->getServices();

}

void ZmdUpdater::updateSelected(QListViewItem *item) {
	
	currentUpdate = item;
	//Ok, so...if MISC is empty, that means we have a package and use COLUMN_NAME
	//if MISC is not empty, we have a patch and we use COLUMN_MISC as the name
	core->getInfo(item->text( (COLUMN_MISC == "") ? COLUMN_NAME : COLUMN_MISC));
}

void ZmdUpdater::startInstall() {

	if (tempList != NULL) {
		QValueList<Package> upList;
		QValueList<Package> instList;
		UpdateListItem *item = (UpdateListItem*)(tempList->firstChild());

		if (item == NULL) {
			return;
		}

		do {
			if (item->state() == QCheckListItem::On) {
				Package p;

				if (COLUMN_MISC == "")
					p.name = item->text(COLUMN_NAME); //gets the name
				else 
					p.name = item->text(COLUMN_MISC); //gets the patch name

				p.id = item->text(COLUMN_ID); //gets the id
				p.version = item->text(COLUMN_NEW_VERSION);
				p.installed = (item->text(COLUMN_INSTALLED) == "Yes") ? true : false;

				if (p.installed == true)
					upList.append(p);
				else
					instList.append(p);
			}
		} while ((item = (UpdateListItem*)(item->nextSibling())) != 0);
		/* From reading the ZMD source, we only need name and ID for packages or patches. This may change in the future, was not in the API */

		if (instList.size() > 0 || upList.size() > 0) {
			ZmdInstallWindow *win = new ZmdInstallWindow(core);

			win->setPackageList(instList, upList, QValueList<Package>());
			win->startUpdate();
			connect(win, SIGNAL(refreshUpdates()), this, SLOT(startRefresh()));
			win->show();
		}
	}
}

void ZmdUpdater::startRefresh() {
	emit(refreshList());
}

void ZmdUpdater::configureUpdater() {
	ZmdConfigWindow *win = new ZmdConfigWindow(core);
//	connect(win, SIGNAL(refreshUpdates()), this, SLOT(startRefresh()));
	win->show();
}


/*

	Information Recieving Slots

		gotServiceListing
		gotCatalogListing
		gotUpdateListing
		gotPatchListing
		gotPackageDetails

*/

void ZmdUpdater::gotServiceListing(QValueList<Service> list) {

	disconnect(core, SIGNAL(serviceListing(QValueList<Service>)), this, SLOT(gotServiceListing(QValueList<Service>)));
	connect(core, SIGNAL(catalogListing(QValueList<Catalog>)), this, SLOT(gotCatalogListing(QValueList<Catalog>)));
	core->getCatalogs(); 
}

void ZmdUpdater::gotCatalogListing(QValueList<Catalog> catalogs) {
	QValueList<Catalog>::iterator iter;

	disconnect(core, SIGNAL(catalogListing(QValueList<Catalog>)), this, SLOT(gotCatalogListing(QValueList<Catalog>)));

	if (catalogs.size() <= 0)
		return;

	for (iter = catalogs.begin(); iter != catalogs.end(); iter++) {
		if ((*iter).subscribed) {
			catalogNames[(*iter).name] = (*iter).displayName;
			core->getUpdates(*iter);
			core->getPatches(*iter);
		}
	}
}

void ZmdUpdater::gotUpdateListing(QValueList<Package> packageList) {
	QValueList<Package>::iterator iter;
	UpdateListItem *newItem;

	if (packageList.size() > 0 || tempList->childCount() > 0) {
		emit(updateApplet(APPLET_UPDATES));
	} else {
		emit(updateApplet(APPLET_NO_UPDATES));
	}

	for (iter = packageList.begin(); iter != packageList.end(); iter++) {
		newItem = new UpdateListItem(tempList, (*iter).name, QCheckListItem::CheckBox); 

		newItem->setText(COLUMN_NEW_VERSION,(*iter).version);
		newItem->setText(COLUMN_SIZE, "Unknown");
		newItem->setText(COLUMN_ID, (*iter).id);
		newItem->setText(COLUMN_INSTALLED, ((*iter).installed == true) ? "Yes" : "No");
		newItem->setText(COLUMN_CATALOG, catalogNames[(*iter).catalog]);
		newItem->setText(COLUMN_MISC, "");

	}
	tempList->setSelected(tempList->firstChild(), true);
	emit(populateDone());
}

void ZmdUpdater::gotPatchListing(QValueList<Patch> patchList) {
	QValueList<Patch>::iterator iter;
	UpdateListItem *newItem;

	if (patchList.size() > 0 || tempList->childCount() > 0) {
		emit(updateApplet(APPLET_UPDATES));
	} else {
		emit(updateApplet(APPLET_NO_UPDATES));
	}

	for (iter = patchList.begin(); iter != patchList.end(); iter++) {
		newItem = new UpdateListItem(tempList, (*iter).description, QCheckListItem::CheckBox);

		newItem->setText(COLUMN_NEW_VERSION,(*iter).version);
		newItem->setText(COLUMN_SIZE, "Unknown");
		newItem->setText(COLUMN_ID, (*iter).id);
		newItem->setText(COLUMN_INSTALLED, ((*iter).installed == true) ? "Yes" : "No");
		newItem->setText(COLUMN_CATALOG, catalogNames[(*iter).catalog]);
		newItem->setText(COLUMN_MISC, (*iter).name);
	}
	tempList->setSelected(tempList->firstChild(), true);
	emit(populateDone());
}

void ZmdUpdater::gotPackageInfo(Package pack) {
	QListViewItem *item;

	if (pack.installed == false)
		return;
	item = tempList->findItem(pack.name, COLUMN_NAME);
	if (item != NULL) {
		currentDescription = pack.version;
		connect(core, SIGNAL(packageDetails(PackageDetails)), this, SLOT(gotPackageDetails(PackageDetails)));
		core->getDetails(pack);
	}
}		

void ZmdUpdater::gotPackageDetails(PackageDetails details) {

	QString version = currentDescription;
	disconnect(core, SIGNAL(packageDetails(PackageDetails)), this, SLOT(gotPackageDetails(PackageDetails)));

	currentDescription = "<b>" + i18n("Description: ") + "</b><br>";
	currentDescription += details.description + "<br>";
	currentDescription += i18n("<b>Upgrading from old version:</b> ");
	currentDescription += version;
	emit(returnDescription(currentDescription));
}


/*

	ERROR Handling

*/
void ZmdUpdater::error(QString message) {
	if (message.contains("Could not connect")) {
		KMessageBox::error(NULL, "We could not connect to ZMD, you may need to go into 'Add/Remove Servers'"
			" and the 'Advanced Options' tab to enable TCP support for ZMD. You will then have to restart ZMD."); 
	}
}

/*

	Auths the core of the updater. This is a workaround.

*/

void ZmdUpdater::authorizeCore() {

	FILE *fd;
	char buffer[1024];
	KProcess proc;
	QCString pass;

	proc << "kdesu";
	proc << "--noignorebutton";
	proc << QString("kzmdauthutil ") + QString(ZMD_CONFIG_PATH);
	if (proc.start() == false) {
		kdError() << "Could not start authutil" << endl;
		authorizeCore();
	}

	while ((fd = fopen("/var/tmp/kzmd-auth", "r")) == NULL) {
		sleep(10);
	}

	fgets(buffer, 1023, fd);
	buffer[strlen(buffer)-1] = '\0'; //get rid of newline
	core->setUser(buffer);
	printf("User: %s\n", buffer);
	memset(buffer, '\0', 1024);
	fgets(buffer, 1023, fd);
	buffer[strlen(buffer)-1] = '\0'; // get rid of newline
	printf("Pass: %s\n", buffer);
	core->setPass(buffer);
	memset(buffer, '\0', 1024);
	fclose(fd);
}

