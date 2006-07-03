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

#include <kiconloader.h>
#include <klocale.h>
#include <kprocess.h>
#include <kpopupmenu.h>
#include <kapp.h>
#include <kconfig.h>
#include <kdebug.h>

#include "MainWindow.h"
#include "GeneralConfigWindow.h"
#include "Updater.h"
#include "UpdateListItem.h"


MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {

	KIconLoader iconLoader(PROGRAM_NAME);

	applet = new KSystemTray(this);
	applet->setPixmap(UserIcon(TRAY_ICON_GREEN));
	applet->setScaledContents(true);
	applet->show();
	connect(applet, SIGNAL(quitSelected()), this, SLOT(slotExit()));

	timer = new QTimer(this);
	readConfig();
	connect(timer, SIGNAL(timeout()), this, SLOT(checkUpdates()));
	//timerInterval is read in from readConfig
	timer->start(timerInterval,false);

	initGUI();
	initMenu();
}

// Read in the config, just the interval really as we cannot deal with the updater itself
void MainWindow::readConfig() {
	KConfig *config = kapp->config();
	int interval;

	config->setGroup("General");
	if ((interval = config->readEntry("Interval").toInt()) <= 0) {
		interval = 15; // set a reasonable default
	}
	interval = interval * 60 * 1000; // convert to ms
	timerInterval = interval;
	timer->changeInterval(timerInterval);
}

//Build GUI, setup system tray and hide GUI initially.
void MainWindow::initGUI() {
	
	mainBox = new QVBoxLayout(this);
	header = new HeaderWidget(this);
	updateList = new QListView(this);
	packageDescription = new KTextEdit(this);
	configureButton = new KPushButton(i18n("Add/Remove Servers"),this);
	cancelButton = new KPushButton(i18n("Cancel"),this);
	installButton = new KPushButton(i18n("Install"),this);
	selectAllButton = new KPushButton(i18n("Select All"),this);
	clearSelectionButton = new KPushButton(i18n("Clear Selection"), this);
	
	mainBox->addWidget(header,0,0);
	mainBox->addWidget(updateList,0,0);
	
	selectionButtonsLayout = new QHBoxLayout(mainBox);
	selectionButtonsLayout->addWidget(selectAllButton, false, Qt::AlignLeft);
	selectionButtonsLayout->addWidget(clearSelectionButton, false, Qt::AlignRight);

	mainBox->addWidget(packageDescription,0,0);

	buttonsLayout = new QHBoxLayout(mainBox);
	buttonsLayout->addWidget(configureButton,false, Qt::AlignLeft);
	buttonsLayout->insertSpacing(1, 250);
	buttonsLayout->addWidget(cancelButton,false, Qt::AlignRight);
	buttonsLayout->addSpacing(10);
	buttonsLayout->addWidget(installButton,false, Qt::AlignRight);

	connect(configureButton, SIGNAL(clicked()), this, SLOT(serverButtonClicked()));
	connect(installButton, SIGNAL(clicked()), this, SLOT(installButtonClicked()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(hide()));
	connect(selectAllButton, SIGNAL(clicked()), this, SLOT(selectButtonClicked()));
	connect(clearSelectionButton, SIGNAL(clicked()), this, SLOT(clearButtonClicked()));

	header->setDescription(i18n("<b>Available Updates:</b><br> The following are software upgrades and patches to add features and fix bugs.<br> <u>Select those you would like and press install.</u>"));

	packageDescription->setReadOnly(true);
	updateList->addColumn(i18n("Name"));
	updateList->addColumn(i18n("New Version"));

	/*
		Hidden Columns, we use these to store data about the packages/patches
	*/
	updateList->addColumn("ID", 0); // This is a hidden column to hold the ID of the patch/package
	updateList->addColumn("Description", 0); // The Package/Patch description
	updateList->addColumn("Installed?", 0); // Is it installed? (Is it an update)
	updateList->addColumn("Size", 0); //This is hidden for the moment, but it shouldn't be
	updateList->addColumn("Catalog", 0); //Obvious

	connect(updateList, SIGNAL(selectionChanged(QListViewItem*)), this, SLOT(slotPackageSelected(QListViewItem*)));
	connect(updateList, SIGNAL(clicked(QListViewItem*)), this, SLOT(slotPackageClicked(QListViewItem*)));

	mainBox->setSpacing(10);
	mainBox->setMargin(10);
	resize(400,500);
	setIcon(UserIcon(TRAY_ICON_GREEN));
	setCaption(i18n("Available Updates"));
	hide();
	return;
}

//Make the main window hide, not quit
void MainWindow::closeEvent(QCloseEvent *e) {
	hide();
}

void MainWindow::initMenu() {
	KPopupMenu *menu = applet->contextMenu();
	menu->insertItem(i18n("Configure Updater"), this, SLOT(configButtonClicked()),0,-1,1);
	menu->insertItem(i18n("Add/Remove Servers"), this, SLOT(serverButtonClicked()),0,-1,1);
}

void MainWindow::disableButtons(bool disable) {

	if (disable) {
		installButton->setDisabled(true);
		if (selectAllButton != NULL) {
			selectAllButton->setDisabled(true);
			clearSelectionButton->setDisabled(true);
		}
	} else if (selectAllButton != NULL) {
		if (updateList->childCount() > 0) {
			selectAllButton->setDisabled(false);
			clearSelectionButton->setDisabled(false);
		}
		if (updatesSelected > 0)
			installButton->setDisabled(false);
		else
			installButton->setDisabled(true);
	} else if (updateList->childCount() > 0) {
		installButton->setDisabled(false);
	}
}

void MainWindow::appletState(int state) {

	switch (state) {
		case APPLET_CHECKING: //We do not have a special icon for checking updates
		case APPLET_NO_UPDATES:
			applet->setPixmap(UserIcon(TRAY_ICON_GREEN));
			break;
		case APPLET_UPDATES:
			applet->setPixmap(UserIcon(TRAY_ICON_RED));
			break;
	}
}

void MainWindow::populateDone() {
	
	if (selectAllButton != NULL) {
		//ZmdUpdateListItems we have (yoda am I in this comment)
		QListViewItem *item = updateList->firstChild();

		while (item != NULL) {
			((UpdateListItem*)item)->setCount(&updatesSelected);
			kdWarning() << "Handing out count" << endl;
			item = item->nextSibling();
		}
	}
}

void MainWindow::serverButtonClicked() {
	//Fire the configure signal to the backend
	emit(configureUpdater());
}

void MainWindow::configButtonClicked() {
	GeneralConfigWindow *win = new GeneralConfigWindow();
	connect(win, SIGNAL(configChanged()), this, SLOT(readConfig()));
	win->show();
}

void MainWindow::installButtonClicked() {
	//Fire the  install signal to the backend and hide ourselves
	emit(startInstall());
	hide();
}

/*
	NOTE: These two functions assume we pack the list with QCheckListItems.
	If this is not the case, the Updater class needs to disable these buttons. 
	You do this by emitting the "disableSelectButtons" signal. 
*/
void MainWindow::selectButtonClicked() {
	QCheckListItem *item = (QCheckListItem*)updateList->firstChild();
	
	while (item != NULL) {
		if (!item->isOn()) {
			item->setState(QCheckListItem::On);
		}
		item = (QCheckListItem*)item->nextSibling();
	}
	updatesSelected = updateList->childCount();
	disableButtons(false);
}

void MainWindow::clearButtonClicked() {
	QCheckListItem *item = (QCheckListItem*)updateList->firstChild();
	
	while (item != NULL) {
		if (item->isOn()) {
			item->setState(QCheckListItem::Off);
		}
		item = (QCheckListItem*)item->nextSibling();
	}
	updatesSelected = 0;
	disableButtons(false);
}

void MainWindow::disableSelectButtons() {
	if (selectAllButton != NULL) {
		selectionButtonsLayout->remove(selectAllButton);
		selectionButtonsLayout->remove(clearSelectionButton);

		delete selectAllButton;
		delete clearSelectionButton;
		delete selectionButtonsLayout;
		selectAllButton = clearSelectionButton = NULL;
		update();
	}
}

void MainWindow::checkUpdates() {
	//Clear the list and fire the update signal to the backend
	updateList->clear();
	packageDescription->setText("");
	emit(populateUpdateList(updateList)); 
	disableButtons(true);
	updatesSelected = false;
}

//Selection/Fetch Description slots
void MainWindow::slotPackageSelected(QListViewItem *packageSelected) {
	
	if (selectAllButton != NULL) {
		disableButtons(false);
	}
	emit(updateSelected(packageSelected));
}

void MainWindow::slotPackageClicked(QListViewItem *pack) {
	disableButtons(false);
}

void MainWindow::gotDescription(QString description) {
	packageDescription->setText(description);
}

//This is where we actually close, called from the system tray
void MainWindow::slotExit() {
	kapp->quit();
}

