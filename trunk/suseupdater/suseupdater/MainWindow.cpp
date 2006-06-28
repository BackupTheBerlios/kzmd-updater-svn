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

#include "MainWindow.h"
#include "GeneralConfigWindow.h"
#include "Updater.h"


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
	configureButton = new QPushButton(i18n("Add/Remove Servers"),this);
	cancelButton = new QPushButton(i18n("Cancel"),this);
	installButton = new QPushButton(i18n("Install"),this);
	
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

	connect(configureButton, SIGNAL(clicked()), this, SLOT(serverButtonClicked()));
	connect(installButton, SIGNAL(clicked()), this, SLOT(installButtonClicked()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(hide()));

	header->setDescription(i18n("<b>Available Updates:</b><br> The following are software upgrades and patches to add features and fix bugs.<br> <u>Select those you would like and press install.</u>"));

	packageDescription->setReadOnly(true);
	updateList->addColumn(i18n("Name"));
	updateList->addColumn(i18n("Old Version"));
	updateList->addColumn(i18n("New Version"));

	/*
		Hidden Columns, we use these to store data about the packages/patches
	*/
	updateList->addColumn("ID", 0); // This is a hidden column to hold the ID of the patch/package
	updateList->addColumn("Description", 0); // The Package/Patch description
	updateList->addColumn("Installed?", 0); // Is it installed? (Is it an update)
	updateList->addColumn(i18n("Size"), 0); //This is hidden for the moment, but it shouldn't be
	updateList->addColumn("Catalog", 0); //Obvious

	connect(updateList, SIGNAL(selectionChanged(QListViewItem*)), this, SLOT(slotPackageSelected(QListViewItem*)));

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

void MainWindow::checkUpdates() {
	//Clear the list and fire the update signal to the backend
	updateList->clear();
	packageDescription->setText("");
	emit(populateUpdateList(updateList)); 
}

void MainWindow::slotPackageSelected(QListViewItem *packageSelected) {
	packageDescription->setText(((QCheckListItem*)packageSelected)->text(COLUMN_DESC));
}

//This is where we actually close, called from the system tray
void MainWindow::slotExit() {
	kapp->quit();
}

