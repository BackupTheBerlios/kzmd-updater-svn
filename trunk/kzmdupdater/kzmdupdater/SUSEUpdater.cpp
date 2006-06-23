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


#include "SUSEUpdater.h"
#include <kiconloader.h>
#include <klocale.h>
#include <kprocess.h>
#include <kpopupmenu.h>

#include "ConfigWindow.h"
#include "InstallWindow.h"

//Cores
#include "ZmdUpdaterCore.h"

#define TRAY_ICON_GREEN "suse_green.png"
#define TRAY_ICON_RED	"suse_red.png"
#define TIMER_INTERVAL	(100*60*15) //1000 ms * 60 s * 5 min = 15 min

//Enum for updateListing
enum { COLUMN_NAME, COLUMN_OLD_VERSION, COLUMN_NEW_VERSION,
		COLUMN_SIZE, COLUMN_ID, COLUMN_DESC, COLUMN_INSTALLED,
		COLUMN_CATALOG };

#include <iostream>
using namespace std;

SUSEUpdater::SUSEUpdater(QWidget *parent) : DCOPObject("kzmdupdater"), QWidget(parent) {

	KIconLoader iconLoader("kzmdupdater");
	core = new ZmdUpdaterCore();
	timer = new QTimer(this);

	authorizeCore();
	initGUI();
	initMenu();
	connect(timer, SIGNAL(timeout()), this, SLOT(checkUpdates()));
	timer->start(TIMER_INTERVAL, false);
	checkUpdates();

}

void SUSEUpdater::fakeCall() {

}

//Build GUI, setup system tray and hide GUI initially.
void SUSEUpdater::initGUI() {

	mainBox = new QVBoxLayout(this);
	trayApplet = new KSystemTray(this);
	header = new HeaderWidget(this);
	updateList = new QListView(this);
	packageDescription = new KTextEdit(this);
	configureButton = new QPushButton(i18n("Configure Updater"),this);
	cancelButton = new QPushButton(i18n("Cancel"),this);
	installButton = new QPushButton(i18n("Install"),this);
	
	trayApplet->setPixmap(UserIcon(TRAY_ICON_GREEN));
	trayApplet->setScaledContents(true);

	mainBox->addWidget(header,0,0);
	mainBox->addWidget(updateList,0,0);
	mainBox->addWidget(packageDescription,0,0);

	configureButton->setMinimumHeight(30);
	cancelButton->setMinimumHeight(30);
	installButton->setMinimumHeight(30);

	buttonsLayout = new QHBoxLayout(mainBox);
	buttonsLayout->addWidget(configureButton,false, Qt::AlignLeft);
	buttonsLayout->insertSpacing(1, 250);
	buttonsLayout->addWidget(cancelButton,false, Qt::AlignRight);
	buttonsLayout->addWidget(installButton,false, Qt::AlignRight);

	connect(configureButton, SIGNAL(clicked()), this, SLOT(configButtonClicked()));
	connect(installButton, SIGNAL(clicked()), this, SLOT(installButtonClicked()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(hide()));

	header->setDescription(i18n("<b>Available Updates:</b><br> The following are software upgrades and patches to add features and fix bugs.<br> <u>Select those you would like and press install.</u>"));

	packageDescription->setReadOnly(true);
	updateList->addColumn(i18n("Name"));
	updateList->addColumn(i18n("Old Version"));
	updateList->addColumn(i18n("New Version"));
	updateList->addColumn(i18n("Size"));
	updateList->addColumn("ID", 0); // This is a hidden column to hold the ID of the patch/package
	updateList->addColumn("Description", 0); //Another Hidden Column
	updateList->addColumn("Installed?", 0);
	updateList->addColumn("Catalog", 0);

	connect(updateList, SIGNAL(selectionChanged(QListViewItem*)), this, SLOT(slotPackageSelected(QListViewItem*)));

	mainBox->setSpacing(10);
	mainBox->setMargin(10);
	trayApplet->show();
	resize(400,500);
	setIcon(UserIcon(TRAY_ICON_GREEN));
	hide();
	return;
}

void SUSEUpdater::initMenu() {
	KPopupMenu *menu = trayApplet->contextMenu();
//	menu->insertItem(i18n("About"), this, SLOT(configButtonClicked()),0,-1,1);
	menu->insertItem(i18n("Configure"), this, SLOT(configButtonClicked()),0,-1,1);
}

void SUSEUpdater::configButtonClicked() {
	ConfigWindow *win = new ConfigWindow(core);
	win->show();
}

void SUSEUpdater::installButtonClicked() {
	InstallWindow *win = new InstallWindow(core);
	QValueList<Package> upList;
	QValueList<Package> instList;
	QCheckListItem *item = (QCheckListItem*)(updateList->firstChild());

	if (item == NULL) {
		delete win;
		return; //Don't install 0 updates...
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
	hide();
}

SUSEUpdater::~SUSEUpdater() {
}

void SUSEUpdater::checkUpdates() {
	connect(core, SIGNAL(catalogListing(QValueList<Catalog>)), this, SLOT(gotCatalogListing(QValueList<Catalog>)));
	connect(core, SIGNAL(updateListing(QValueList<Package>)), this, SLOT(gotUpdateListing(QValueList<Package>)));
	connect(core, SIGNAL(patchListing(QValueList<Patch>)), this, SLOT(gotPatchListing(QValueList<Patch>)));

	core->getCatalogs(); //this will return to gotCatalogs
}


void SUSEUpdater::slotPackageSelected(QListViewItem *packageSelected) {
	packageDescription->setText(((QCheckListItem*)packageSelected)->text(COLUMN_DESC));
}

void SUSEUpdater::slotExit() {
	close();
}

void SUSEUpdater::gotCatalogListing(QValueList<Catalog> catalogs) {
	QValueList<Catalog>::iterator iter;

	if (catalogs.size() <= 0)
		return;

	for (iter = catalogs.begin(); iter != catalogs.end(); iter++) {
		core->getUpdates(*iter);
	}
	disconnect(core, SIGNAL(catalogListing(QValueList<Catalog>)), this, SLOT(gotCatalogListing(QValueList<Catalog>)));
}

void SUSEUpdater::gotUpdateListing(QValueList<Package> packageList) {
	QValueList<Package>::iterator iter;
	QCheckListItem *newItem;

	if (packageList.size() > 0 || updateList->childCount() > 0) {
		trayApplet->setPixmap(UserIcon(TRAY_ICON_RED));
	} else {
		trayApplet->setPixmap(UserIcon(TRAY_ICON_GREEN));
	}
	updateList->clear();	
	for (iter = packageList.begin(); iter != packageList.end(); iter++) {
		newItem = new QCheckListItem(updateList, (*iter).name, QCheckListItem::CheckBox);

		newItem->setText(COLUMN_OLD_VERSION,"Unknown");
		newItem->setText(COLUMN_NEW_VERSION,(*iter).version);
		newItem->setText(COLUMN_SIZE, "Unknown");
		newItem->setText(COLUMN_ID, (*iter).id);
		newItem->setText(COLUMN_DESC, (*iter).description);
		newItem->setText(COLUMN_INSTALLED, ((*iter).installed == true) ? "Yes" : "No");
		newItem->setText(COLUMN_CATALOG, (*iter).catalog);
	}
	updateList->setSelected(updateList->firstChild(), true);

}

void SUSEUpdater::gotPatchListing(QValueList<Patch> patchList) {
	QValueList<Patch>::iterator iter;
	QCheckListItem *newItem;

	if (patchList.size() > 0 || updateList->childCount() > 0) {
		trayApplet->setPixmap(UserIcon(TRAY_ICON_RED));
	} else {
		trayApplet->setPixmap(UserIcon(TRAY_ICON_GREEN));
	}

	for (iter = patchList.begin(); iter != patchList.end(); iter++) {
		newItem = new QCheckListItem(updateList, (*iter).name + " (Patch)", QCheckListItem::CheckBox);

		newItem->setText(COLUMN_OLD_VERSION,"Unknown");
		newItem->setText(COLUMN_NEW_VERSION,(*iter).version);
		newItem->setText(COLUMN_SIZE, "Unknown");
		newItem->setText(COLUMN_ID, (*iter).id);
		newItem->setText(COLUMN_DESC, (*iter).description);
		newItem->setText(COLUMN_INSTALLED, ((*iter).installed == true) ? "Yes" : "No");
		newItem->setText(COLUMN_CATALOG, (*iter).catalog);	
	}
	updateList->setSelected(updateList->firstChild(), true);

}

/*

	This is a sticky point. Seems to work at the moment, but it is "flaky"

*/

void SUSEUpdater::authorizeCore() {

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
