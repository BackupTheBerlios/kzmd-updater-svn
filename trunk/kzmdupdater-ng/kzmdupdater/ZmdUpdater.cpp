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
#include <stdio.h>
#include <string.h>

#include "ZmdUpdater.h"
#include "MainWindow.h"
#include "InstallWindow.h"
#include "ConfigWindow.h"

#include <iostream>
using namespace std;

ZmdUpdater::ZmdUpdater() : Updater() {

	core = new ZmdUpdaterCore(this);
	authorizeCore();
}

void ZmdUpdater::populateUpdateList(QListView *updateList) {

	tempList = updateList;

	connect(core, SIGNAL(catalogListing(QValueList<Catalog>)), this, SLOT(gotCatalogListing(QValueList<Catalog>)));
	connect(core, SIGNAL(updateListing(QValueList<Package>)), this, SLOT(gotUpdateListing(QValueList<Package>)));
	connect(core, SIGNAL(patchListing(QValueList<Patch>)), this, SLOT(gotPatchListing(QValueList<Patch>)));
	core->getCatalogs(); 

}

void ZmdUpdater::startInstall() {

	if (tempList != NULL) {
		InstallWindow *win = new InstallWindow(core);
		QValueList<Package> upList;
		QValueList<Package> instList;
		QCheckListItem *item = (QCheckListItem*)(tempList->firstChild());

		if (item == NULL) {
			delete win;
			return;
		}

		do {
			if (item->isOn()) {
				Package p;
				p.name = item->text(COLUMN_NAME); //gets the name
				p.id = item->text(COLUMN_ID); //gets the id
				p.version = item->text(COLUMN_NEW_VERSION);
				p.catalog = item->text(COLUMN_CATALOG);
				p.installed = (item->text(COLUMN_INSTALLED) == "Yes") ? true : false;
				if (p.installed == true)
					upList.append(p);
				else
					instList.append(p);
			}
		} while ((item = (QCheckListItem*)(item->nextSibling())) != 0);
		/* From reading the ZMD source, we only need name and ID for packages or patches. This may change in the future, was not in the API */

		win->setPackageList(instList, upList, QValueList<Package>());
		win->startUpdate();
		win->show();
	}
}

void ZmdUpdater::configureUpdater() {
	ConfigWindow *win = new ConfigWindow(core);
	win->show();
}

void ZmdUpdater::gotCatalogListing(QValueList<Catalog> catalogs) {
	QValueList<Catalog>::iterator iter;

	if (catalogs.size() <= 0)
		return;

	for (iter = catalogs.begin(); iter != catalogs.end(); iter++) {
		core->getUpdates(*iter);
	}
	disconnect(core, SIGNAL(catalogListing(QValueList<Catalog>)), this, SLOT(gotCatalogListing(QValueList<Catalog>)));
}

void ZmdUpdater::gotUpdateListing(QValueList<Package> packageList) {
	QValueList<Package>::iterator iter;
	QCheckListItem *newItem;

	if (packageList.size() > 0 || tempList->childCount() > 0) {
		emit(updateApplet(APPLET_UPDATES));
	} else {
		emit(updateApplet(APPLET_NO_UPDATES));
	}

	for (iter = packageList.begin(); iter != packageList.end(); iter++) {
		newItem = new QCheckListItem(tempList, (*iter).name, QCheckListItem::CheckBox);

		newItem->setText(COLUMN_OLD_VERSION,"Unknown");
		newItem->setText(COLUMN_NEW_VERSION,(*iter).version);
		newItem->setText(COLUMN_SIZE, "Unknown");
		newItem->setText(COLUMN_ID, (*iter).id);
		newItem->setText(COLUMN_DESC, (*iter).description);
		newItem->setText(COLUMN_INSTALLED, ((*iter).installed == true) ? "Yes" : "No");
		newItem->setText(COLUMN_CATALOG, (*iter).catalog);
	}
	tempList->setSelected(tempList->firstChild(), true);
	disconnect(core, SIGNAL(updateListing(QValueList<Package>)), this, SLOT(gotUpdateListing(QValueList<Package>)));

}

void ZmdUpdater::gotPatchListing(QValueList<Patch> patchList) {
	QValueList<Patch>::iterator iter;
	QCheckListItem *newItem;

	if (patchList.size() > 0 || tempList->childCount() > 0) {
		emit(updateApplet(APPLET_UPDATES));
	} else {
		emit(updateApplet(APPLET_NO_UPDATES));
	}

	for (iter = patchList.begin(); iter != patchList.end(); iter++) {
		newItem = new QCheckListItem(tempList, (*iter).name + " (Patch)", QCheckListItem::CheckBox);

		newItem->setText(COLUMN_OLD_VERSION,"Unknown");
		newItem->setText(COLUMN_NEW_VERSION,(*iter).version);
		newItem->setText(COLUMN_SIZE, "Unknown");
		newItem->setText(COLUMN_ID, (*iter).id);
		newItem->setText(COLUMN_DESC, (*iter).description);
		newItem->setText(COLUMN_INSTALLED, ((*iter).installed == true) ? "Yes" : "No");
		newItem->setText(COLUMN_CATALOG, (*iter).catalog);	
	}
	tempList->setSelected(tempList->firstChild(), true);
	disconnect(core, SIGNAL(patchListing(QValueList<Patch>)), this, SLOT(gotpatchListing(QValueList<Patch>)));
}

void ZmdUpdater::authorizeCore() {

	FILE *fd;
	char buffer[1024];
	KProcess proc;
	QCString pass;

	proc << "kdesu";
	proc << QString("kzmdauthutil ") + QString(ZMD_CONFIG_PATH);
	if (proc.start() == false)
		cout << "ERROR: Could not start the kzmdauthutil" << endl;

	if ( (fd = fopen("/var/tmp/kzmd-auth", "r")) != NULL) {
		
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

}
