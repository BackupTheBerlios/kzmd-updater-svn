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
#include <kconfig.h>
#include <kapp.h>

#include <qpopupmenu.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/poll.h>
#include <sys/types.h>
#include <fcntl.h>

#include "ZmdUpdater.h"
#include "MainWindow.h"
#include "ZmdInstallWindow.h"
#include "ZmdConfigWindow.h"
#include "UpdateListItem.h"

ZmdUpdater::ZmdUpdater() : Updater() {

	//Init and auth the core
	core = new ZmdUpdaterCore(this);
	showGeneralFaultError = true;
	readConfig();
	authorizeCore();

	//Connect core signals
	connect(core, SIGNAL(updateListing(QValueList<Package>)), 
					this, SLOT(gotUpdateListing(QValueList<Package>)));

	connect(core, SIGNAL(patchListing(QValueList<Patch>)), 
					this, SLOT(gotPatchListing(QValueList<Patch>)));

	connect(core, SIGNAL(packageInfo(Package)), 
					this, SLOT(gotPackageInfo(Package)));

	connect(core, SIGNAL(generalFault(QString, int)), 
					this, SLOT(error(QString, int)));

#ifndef NO_PACKAGE_LOCKS
	connect(core, SIGNAL(lockListing(QValueList<PackageLock>)), 
					this, SLOT(gotLockListing(QValueList<PackageLock>)));
#endif

}

/********************************************************************

	Slots recieving signals from the mainwindow 

*********************************************************************/

void ZmdUpdater::populateUpdateList(QListView *updateList) {

	tempList = updateList;
	emit(updateApplet(APPLET_NO_UPDATES));
	
	connect(core, SIGNAL(serviceListing(QValueList<Service>)), 
					this, SLOT(gotServiceListing(QValueList<Service>)));

	core->getServices();
}

void ZmdUpdater::updateSelected(QListViewItem *item) {
	
	currentUpdate = item;
	//Ok, so...if MISC is empty, that means we have a package and use COLUMN_NAME
	//if MISC is not empty, we have a patch and we use COLUMN_MISC as the name
	core->getInfo((item->text(COLUMN_MISC) == "" ) ? 
								(item->text(COLUMN_NAME)) : 
								(item->text(COLUMN_MISC)));
}

void ZmdUpdater::updateMenu(QListViewItem *item, const QPoint& point) {

#ifndef NO_PACKAGE_LOCKS
	if (item != NULL && tempList != NULL) {
		QPopupMenu *menu = new QPopupMenu(tempList);
		if (item->text(COLUMN_LOCK) == "") {
			menu->insertItem(i18n("Hold Back Package"), this, SLOT(holdPackage()));
		} else {
			menu->insertItem(i18n("Don't Hold Back Package"), this, SLOT(removeHold()));
		}
		menu->popup(point);
	}
#endif
}

/*****************************************************************************
									These are all noops because we do not support
									package holds yet
*****************************************************************************/
void ZmdUpdater::holdPackage() {
	QListViewItem *item;
	PackageLock lock;

	item = tempList->currentItem();
	if (item == NULL)
		return;

	lock.pack.id = item->text(COLUMN_ID);
	lock.pack.catalog = item->text(COLUMN_CATALOG);
	lock.pack.name = (item->text(COLUMN_MISC == "")) ? item->text(COLUMN_NAME) : item->text(COLUMN_MISC);
	core->lockPackage(lock);
}

void ZmdUpdater::removeHold() {
	QListViewItem *item;
	PackageLock lock;

	item = tempList->currentItem();
	if (item == NULL)
		return;

//This does nothing, since locking support is not available in ZMD as of yet
}
/******************************************************************************
******************************************************************************/

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

				if (item->text(COLUMN_MISC) == "") {
					p.name = item->text(COLUMN_NAME); //gets the name
					p.type = "package";
				} else {
					p.name = item->text(COLUMN_MISC); //gets the patch name
					p.type = "patch";
				}

				p.id = item->text(COLUMN_ID); //gets the id
				p.installed = (item->text(COLUMN_INSTALLED) == "Yes") ? true : false;

				if (p.installed == false)
					instList.append(p);
				else
					upList.append(p);
			}
		} while ((item = (UpdateListItem*)(item->nextSibling())) != 0);
		/* From reading the ZMD source, we only need name and ID for packages or patches. This may change in the future, was not in the API */

		if (instList.size() > 0 || upList.size() > 0) {
			ZmdInstallWindow *win = new ZmdInstallWindow(core); //deletes itself

			win->setPackageList(instList, upList, QValueList<Package>());
			win->startUpdate();

			//Allow the install window to signal an update refresh
			connect(win, SIGNAL(refreshUpdates()), 
							this, SLOT(startRefresh()));

			win->show();
		}
	}
}

void ZmdUpdater::startRefresh() {
	emit(refreshList());
}

void ZmdUpdater::configureUpdater() {
	ZmdConfigWindow *win = new ZmdConfigWindow(core); //deletes itself

	//Allow the configure window to signal an update refresh
	connect(win, SIGNAL(refreshUpdates()), 
					this, SLOT(startRefresh()));

	win->show();
}


/*

	Information Recieving Slots

		gotLockListing
		gotServiceListing
		gotCatalogListing
		gotUpdateListing
		gotPatchListing
		gotPackageDetails

*/

void ZmdUpdater::gotLockListing(QValueList<PackageLock> locks) {

	QListViewItem *item;
	QValueList<PackageLock>::iterator iter;

	if (tempList == NULL || tempList->childCount() == 0) {
		kdWarning() << "List empty or null" << endl;
		return;
	}

	for (iter = locks.begin(); iter != locks.end(); iter++) {
		if ((*iter).pack.id.isEmpty() == false) {
			if (tempList != NULL && (item = tempList->findItem((*iter).pack.id, COLUMN_ID)) != NULL) {
				item->setText(COLUMN_LOCK, (*iter).id);
				item->setText(COLUMN_NAME, item->text(COLUMN_NAME) + " (Package Held)");
			} else if (tempList != NULL) {
				//We have a lock without a package attached to it
				kdWarning() << "Lock without a package" << endl;
			}
		}
	}

}

void ZmdUpdater::gotServiceListing(QValueList<Service> list) {

	disconnect(core, SIGNAL(serviceListing(QValueList<Service>)), 
						this, SLOT(gotServiceListing(QValueList<Service>)));

	connect(core, SIGNAL(catalogListing(QValueList<Catalog>)), 
					this, SLOT(gotCatalogListing(QValueList<Catalog>)));

	//Ok, we communicated with zmd, don't show the error on failure now
	showGeneralFaultError = false;

	//get the catalogs
	core->getCatalogs(); 
}

void ZmdUpdater::gotCatalogListing(QValueList<Catalog> catalogs) {
	QValueList<Catalog>::iterator iter;

	disconnect(core, SIGNAL(catalogListing(QValueList<Catalog>)), 
						this, SLOT(gotCatalogListing(QValueList<Catalog>)));

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

	if (packageList.size() > 0) {
		tempList->setSelected(tempList->firstChild(), true);
		core->getLocks();
		emit(populateDone());
	}
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
		if ((*iter).status != 3)
			continue;

		newItem = new UpdateListItem(tempList, (*iter).description, QCheckListItem::CheckBox);

		newItem->setText(COLUMN_NEW_VERSION,(*iter).version);
		newItem->setText(COLUMN_SIZE, "Unknown");
		newItem->setText(COLUMN_ID, (*iter).id);
		newItem->setText(COLUMN_INSTALLED, ((*iter).installed == true) ? "Yes" : "No");
		newItem->setText(COLUMN_CATALOG, catalogNames[(*iter).catalog]);
		newItem->setText(COLUMN_MISC, (*iter).name);
		newItem->setText(COLUMN_CATEGORY, (*iter).category);
	}

	if (patchList.size() > 0) {
		tempList->setSelected(tempList->firstChild(), true);
		core->getLocks();
		emit(populateDone());
	}
}

void ZmdUpdater::gotPackageInfo(Package pack) {
	QListViewItem *item;

	item = tempList->findItem(pack.name, COLUMN_NAME);
	if (item != NULL) {
		if (pack.installed == false)
			return;
	} else {
		item = tempList->findItem(pack.name, COLUMN_MISC);
		if (item == NULL)
			return;
	}

	currentDescription = pack.version;

	connect(core, SIGNAL(packageDetails(PackageDetails)), 
					this, SLOT(gotPackageDetails(PackageDetails)));

	core->getDetails(pack);
}		

void ZmdUpdater::gotPackageDetails(PackageDetails details) {

	QString version = currentDescription;
	disconnect(core, SIGNAL(packageDetails(PackageDetails)), 
						this, SLOT(gotPackageDetails(PackageDetails)));

	currentDescription = "<b>" + i18n("Description: ") + "</b><br>";
	currentDescription += details.description + "<br>";
	currentDescription += i18n("<b>Upgrading from old version:</b> ");
	currentDescription += version;

	emit(returnDescription(currentDescription));
}


/*

	ERROR Handling

*/
void ZmdUpdater::error(QString message, int errorCode) {
	if (showGeneralFaultError == true && message.contains("Could not connect")) {
		showGeneralFaultError = false;
		KMessageBox::error(NULL, "We could not connect to ZMD, you may need to go into 'Add/Remove Servers'"
			" and the 'Advanced Options' tab to enable TCP support for ZMD. You will then have to restart ZMD."); 
	} 
}

/*

	Read configuration

*/

void ZmdUpdater::readConfig() {

	QString serverIP;
	KConfig *config = kapp->config();
	config->setGroup("General");

	switch (config->readEntry("ZmdProto").toInt()) {

		case ZMD_TCP:
			serverIP = config->readEntry("ZmdServer", TCP_SERVER_ADDY);
			core->setServer(QString("http://") + serverIP + QString(TCP_SERVER_POSTFIX));
			break;
		case ZMD_UDS:
			core->setServer(UDS_SERVER_ADDY);
			break;
		default:
			core->setServer(QString("http://") + TCP_SERVER_ADDY + QString(TCP_SERVER_POSTFIX));
			config->writeEntry("ZmdProto", ZMD_TCP);
			break;
	}

}

/*

	Auths the core of the updater. This is a workaround.

*/

void ZmdUpdater::authorizeCore() {

	int fd;
	char buffer[1024];
	KProcess proc;
	QCString pass;
	struct pollfd pfd;
	int count;
	QString data;
	QStringList list;


	proc << "kdesu";
	proc << QString("kzmdauthutil ") + QString(ZMD_CONFIG_PATH);
	if (proc.start() == false) {
		kdError() << "Could not start authutil" << endl;
		authorizeCore();
	}

	//This is just  to make sure we don't proceed faster than our child proc
	mkfifo("/var/tmp/kzmd-auth", 0666);

	if ((fd = open("/var/tmp/kzmd-auth", O_NONBLOCK | O_RDONLY)) == -1) {
		perror("There has been a problem opening the fifo");
		exit(1);
	}

	pfd.fd = fd;
	pfd.events = POLLIN;

	if (poll(&pfd, 1, 2*(1000*60)) < 0) {
		kdError() << "We timed out waiting for the root password" << endl;
		exit(1);
	}

	while ((count = read(fd, buffer, 1024)) > 0) {
		buffer[count] = '\0';
		data += buffer;
	}
	list = QStringList::split("\n", data);
	if (list.count() != 2) {
		kdError() << "Could not read the pass file" << endl;
		exit(1);
	}
	for (QStringList::iterator iter = list.begin(); iter != list.end(); iter++) {
		if (iter == list.begin()) {
			core->setUser((*iter).ascii());
			kdWarning() << (*iter).ascii() << endl;
		} else {
			core->setPass((*iter).ascii());			
			kdWarning() << (*iter).ascii() << endl;
		}
	}
	close(fd);
}

