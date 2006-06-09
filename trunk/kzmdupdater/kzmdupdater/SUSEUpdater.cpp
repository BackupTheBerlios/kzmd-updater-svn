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
#include "ConfigWindow.h"
#include "InstallWindow.h"
#include <iostream>
using namespace std;

#define TRAY_ICON_GREEN "trayicon-green.png"


SUSEUpdater::SUSEUpdater() : KMainWindow(0L, "kzmdupdater") {

	KIconLoader iconLoader("kzmdupdater");
	initGUI();
//	loadPreferences(); ?
	checkUpdates();
}

//Build GUI, setup system tray and hide GUI initially.
void SUSEUpdater::initGUI() {

	mainBox = new QVBox(this);
	trayApplet = new KSystemTray(this);
	header = new HeaderWidget(mainBox);
	updateList = new QListView(mainBox);
	packageDescription = new KTextEdit(mainBox);
	buttons = new QWidget(mainBox);
	buttonsLayout = new QHBoxLayout(buttons);
	configureButton = new QPushButton(i18n("Configure Updater"),buttons);
	cancelButton = new QPushButton(i18n("Cancel"),buttons);
	installButton = new QPushButton(i18n("Install"),buttons);

	trayApplet->setPixmap(UserIcon(TRAY_ICON_GREEN));
	trayApplet->setScaledContents(true);

	configureButton->setMinimumHeight(30);
	cancelButton->setMinimumHeight(30);
	installButton->setMinimumHeight(30);

	buttonsLayout->addWidget(configureButton,false, Qt::AlignLeft);
	buttonsLayout->insertSpacing(1, 250);
	buttonsLayout->addWidget(cancelButton,false, Qt::AlignRight);
	buttonsLayout->addWidget(installButton,false, Qt::AlignRight);

	connect(configureButton, SIGNAL(clicked()), this, SLOT(configClicked()));
	connect(installButton, SIGNAL(clicked()), this, SLOT(installClicked()));

	header->setDescription(i18n("<b>Available Updates:</b><br> The following are software upgrades and patches to add features and fix bugs.<br> <u>Select those you would like and press install.</u>"));

	packageDescription->setReadOnly(true);

	updateList->addColumn(i18n("Name"));
	updateList->addColumn(i18n("Old Version"));
	updateList->addColumn(i18n("New Version"));
	updateList->addColumn(i18n("Size"));
	connect(updateList, SIGNAL(selectionChanged(QListViewItem*)), this, SLOT(slotPackageSelected(QListViewItem*)));

	mainBox->setSpacing(10);
	mainBox->setMargin(10);
	setCentralWidget(mainBox);
	trayApplet->show();
	hide();
	return;
}

void SUSEUpdater::configClicked() {
	ConfigWindow *win = new ConfigWindow();
}

void SUSEUpdater::installClicked() {
	InstallWindow *win = new InstallWindow();
}

SUSEUpdater::~SUSEUpdater() {
	delete configureButton;
	delete cancelButton;
	delete installButton;
	delete buttons;
	delete packageDescription;
	delete updateList;
	delete header;
	delete trayApplet;
}

void SUSEUpdater::checkUpdates() {

	QCheckListItem *testItem = new QCheckListItem(updateList, QString("Testing"), QCheckListItem::CheckBox);

	testItem->setText(1,"0.3");

	testItem = new QCheckListItem(updateList, "Testing 2", QCheckListItem::CheckBox);
}

void SUSEUpdater::slotPackageSelected(QListViewItem *packageSelected) {
	packageDescription->setText(((QCheckListItem*)packageSelected)->text());
}

void SUSEUpdater::slotExit() {
	close();
}
