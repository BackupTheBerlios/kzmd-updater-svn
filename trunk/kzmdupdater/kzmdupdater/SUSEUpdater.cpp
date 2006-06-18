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
#include <kdesu/client.h>
#include <kpassdlg.h>

#include "HeaderWidget.h"
#include "ConfigWindow.h"
#include "InstallWindow.h"
#include "ZmdUpdaterCore.h"

#define TRAY_ICON_GREEN "suse_green.png"

#include <iostream>
using namespace std;

SUSEUpdater::SUSEUpdater() : KMainWindow(0L, "kzmdupdater") {

	KIconLoader iconLoader("kzmdupdater");
	initGUI();
	core = new ZmdUpdaterCore();
	authorizeCore();
	checkUpdates();
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
	connect(updateList, SIGNAL(selectionChanged(QListViewItem*)), this, SLOT(slotPackageSelected(QListViewItem*)));

	mainBox->setSpacing(10);
	mainBox->setMargin(10);
	trayApplet->show();
	resize(400,500);
	hide();
	return;
}

void SUSEUpdater::configButtonClicked() {
	ConfigWindow *win = new ConfigWindow(core);
}

void SUSEUpdater::installButtonClicked() {
	InstallWindow *win = new InstallWindow(core);
}

SUSEUpdater::~SUSEUpdater() {
}

void SUSEUpdater::checkUpdates() {
	connect(core, SIGNAL(catalogListing(QValueList<Catalog>)), this, SLOT(gotCatalogs(QValueList<Catalog>)));
	connect(core, SIGNAL(updateListing(QValueList<Package>)), this, SLOT(gotUpdateListing(QValueList<Package>)));
	connect(core, SIGNAL(patchListing(QValueList<Patch>)), this, SLOT(gotPatchListing(QValueList<Patch>)));
	core->getCatalogs(); //this will return to gotCatalogs
}


void SUSEUpdater::slotPackageSelected(QListViewItem *packageSelected) {
	packageDescription->setText(((QCheckListItem*)packageSelected)->text());
}

void SUSEUpdater::slotExit() {
	close();
}

void SUSEUpdater::gotCatalogs(QValueList<Catalog> catalogs) {
	QValueList<Catalog>::iterator iter;

	if (catalogs.size() <= 0)
		return;

	for (iter = catalogs.begin(); iter != catalogs.end(); iter++) {
		core->getUpdates(*iter);
	}
}

void SUSEUpdater::gotUpdateListing(QValueList<Package> packageList) {
	QValueList<Package>::iterator iter;
	QCheckListItem *newItem;

	for (iter = packageList.begin(); iter != packageList.end(); iter++) {
		newItem = new QCheckListItem(updateList, (*iter).name, QCheckListItem::CheckBox);

		newItem->setText(1,"Unknown");
		newItem->setText(2,(*iter).version);
		newItem->setText(3, "Unknown");
	}
}

void SUSEUpdater::gotPatchListing(QValueList<Patch> patchList) {
	QValueList<Patch>::iterator iter;
	QCheckListItem *newItem;

	for (iter = patchList.begin(); iter != patchList.end(); iter++) {
		newItem = new QCheckListItem(updateList, (*iter).name, QCheckListItem::CheckBox);

		newItem->setText(1,"Unknown");
		newItem->setText(2,(*iter).version);
		newItem->setText(3, "Unknown");
	}
}

void SUSEUpdater::authorizeCore() {

	FILE *fd;
	char buffer[1024];
	KDEsuClient client;
	QCString pass;

	int status = KPasswordDialog::getPassword(pass, i18n("Please enter the root password for this machine:"));

	if(status != KPasswordDialog::Accepted) {
		exit(1);
	}	
	client.setPass(pass,60);
	client.exec("kzmdauthutil /etc/zmd", "root");	

	if ( (fd = fopen("/var/tmp/kzmd-auth", "r")) != NULL) {

		
		fgets(buffer, 1023, fd);
		core->setUser(buffer);
		puts(buffer);
		memset(buffer, '\0', 1024);
		fgets(buffer, 1023, fd);
		puts(buffer);		
		core->setPass(buffer);
		memset(buffer, '\0', 1024);
		fclose(fd);
	} else {
		//Jury-rig for devs comp
		core->setUser("8489a518dd084e2d83cdd00c3beaed8b");
		core->setPass("12acc5618a004a26863c034a24a6f31f");
	}
}
