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
#include <ksystemtray.h>
#include <ktextedit.h>
#include <kpushbutton.h>

#include <qlistview.h>
#include <qlabel.h>
#include <qtimer.h>
#include <qlayout.h>

#include "Constants.h"
#include "HeaderWidget.h"
#include "MainWindow.h"
#include "GeneralConfigWindow.h"
#include "Updater.h"
#include "UpdateListItem.h"
#include "TrayIcon.h"


MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {

	KIconLoader iconLoader(PROGRAM_NAME);

	applet = new TrayIcon(this);
	applet->show();
	connect(applet, SIGNAL(quitSelected()), this, SLOT(slotExit()));

	timer = new QTimer(this);
	readConfig();
	connect(timer, SIGNAL(timeout()), this, SLOT(checkUpdates()));
	//timerInterval is read in by readConfig
	timer->start(timerInterval,false);

	initGUI();
	initMenu();

	//Initially we have 0 selected updates of course. 
	updatesSelected = 0;
}

/*************************************************************

												INIT Methods

**************************************************************/

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
	configureButton = new KPushButton(i18n("Add/Remove Servers"), this);
	cancelButton = new KPushButton(KStdGuiItem::cancel(), this);
	installButton = new KPushButton(i18n("Install"), this);
	selectAllButton = new KPushButton(i18n("Select All"), this);
	clearSelectionButton = new KPushButton(i18n("Clear Selection"), this);
	
	mainBox->addWidget(header, 0, 0);
	mainBox->addWidget(updateList, 0, 0);
	
	selectionButtonsLayout = new QHBoxLayout(mainBox);
	selectionButtonsLayout->addWidget(selectAllButton, false, Qt::AlignLeft);
	selectionButtonsLayout->addWidget(clearSelectionButton, false, Qt::AlignRight);

	packageDescription->setReadOnly(true);
	packageDescription->setMinimumHeight(125);
	mainBox->addWidget(packageDescription, 0, 0);

	buttonsLayout = new QHBoxLayout(mainBox);
	buttonsLayout->addWidget(configureButton, false, Qt::AlignLeft);
	buttonsLayout->insertSpacing(1, 250);
	buttonsLayout->addWidget(installButton, false, Qt::AlignRight);
	buttonsLayout->addSpacing(10);
	buttonsLayout->addWidget(cancelButton, false, Qt::AlignRight);

	connect(configureButton, SIGNAL(clicked()), this, SLOT(serverButtonClicked()));
	connect(installButton, SIGNAL(clicked()), this, SLOT(installButtonClicked()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(hide()));
	connect(selectAllButton, SIGNAL(clicked()), this, SLOT(selectButtonClicked()));
	connect(clearSelectionButton, SIGNAL(clicked()), this, SLOT(clearButtonClicked()));

	header->setDescription(i18n("<b>Available Updates:</b><br> The following are software upgrades and patches to add features and fix bugs.<br> <u>Select those you would like and press install.</u>"));

	updateList->addColumn(i18n("Name"));
	updateList->addColumn(i18n("New Version"));
	updateList->addColumn(i18n("Catalog"), 10000); //cover the rest of the window
	updateList->setHScrollBarMode(QListView::AlwaysOff);

	/*
		Hidden Columns, we use these to store data about the packages/patches
	*/
	updateList->addColumn("ID", 0); // This is a hidden column to hold the ID of the patch/package
	updateList->setColumnWidthMode(COLUMN_ID, QListView::Manual);
	updateList->addColumn("Description", 0); // The Package/Patch description
	updateList->setColumnWidthMode(COLUMN_DESC, QListView::Manual);
	updateList->addColumn("Installed?", 0); // Is it installed? (Is it an update)
	updateList->setColumnWidthMode(COLUMN_INSTALLED, QListView::Manual);
	updateList->addColumn("Size", 0); //This is hidden for the moment, but it shouldn't be
	updateList->setColumnWidthMode(COLUMN_SIZE, QListView::Manual);
	updateList->addColumn("Misc", 0); //This is used to store misc info (patch name for zmd)
	updateList->setColumnWidthMode(COLUMN_MISC, QListView::Manual);
	updateList->addColumn("LockID", 0); //Stores our lock id
	updateList->setColumnWidthMode(COLUMN_LOCK, QListView::Manual);

	connect(updateList, SIGNAL(selectionChanged(QListViewItem*)), 
					this, SLOT(slotPackageSelected(QListViewItem*)));

	connect(updateList, SIGNAL(clicked(QListViewItem*)), 
					this, SLOT(slotPackageClicked(QListViewItem*)));

	connect(updateList, SIGNAL(contextMenuRequested(QListViewItem*, const QPoint&, int)), 
					this, SLOT(slotPackageRightClicked(QListViewItem*, const QPoint&, int)));

	mainBox->setSpacing(10);
	mainBox->setMargin(10);
	resize(400,500);
	setIcon(UserIcon(TRAY_ICON_GREEN));
	setCaption(i18n("Available Updates"));
	hide();
	return;
}

void MainWindow::initMenu() {
	KPopupMenu *menu = applet->contextMenu();
	menu->insertItem(i18n("Configure Applet"), this, SLOT(configButtonClicked()),0,-1,1);
	menu->insertItem(i18n("Add/Remove Servers"), this, SLOT(serverButtonClicked()),0,-1,1);
}

/*
	Button logic for mainWindow: 
	If disable is true, we disable select/install buttons. 
	If false, we make decisions based on whether we have 
	updates and if they are selected
*/
void MainWindow::disableButtons(bool disable) {

	if (disable) { //If true, we just disable the buttons
		installButton->setDisabled(true);
		if (selectAllButton != NULL) { //if not null, we need to disable these buttons too
			selectAllButton->setDisabled(true);
			clearSelectionButton->setDisabled(true);
		}
	} else if (selectAllButton != NULL) { //if this is true, we have to deal with the CheckListItems
		if (updateList->childCount() > 0) { //if there are updates in the list, enable these
			selectAllButton->setDisabled(false);
			clearSelectionButton->setDisabled(false);
		}
		if (updatesSelected > 0) //if there are updates selected for installation, enable this button
			installButton->setDisabled(false);
		else
			installButton->setDisabled(true); //else disable it
	} else if (updateList->childCount() > 0) { //if we don't have to deal with selections,
		installButton->setDisabled(false);	   //just enable the install button if there are updates
	}
}

/*********************************************************************

	Slots recieving signals from the updater 

*********************************************************************/

void MainWindow::appletState(int state) {
	applet->setUpdates(updateList->childCount());
	applet->setState(state);
}

void MainWindow::populateDone() {

	//if selectAllButton is not NULL, then we have check boxes
	if (selectAllButton != NULL) {
		//UpdateListItems we have (yoda am I in this comment)
		QListViewItem *item = updateList->firstChild();

		/*
			We loop through the list here, giving each UpdateListItem a reference to count.
			This count variable is basically just a semephore type thing that controls which 
			buttons are enabled. Only useful for those backends that use checkboxes.
		*/

		while (item != NULL) {
			((UpdateListItem*)item)->setCount(&updatesSelected);
			item = item->nextSibling();
		}
	}
	applet->setUpdates(updateList->childCount());
}

void MainWindow::disableSelectButtons() {

	//if selectAllButton is not NULL, we have not yet disabled it
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

void MainWindow::gotDescription(QString description) {
	packageDescription->setText("");
	packageDescription->setTextFormat(Qt::RichText);
	packageDescription->setText(description);
}

/***************************************************************
	
	Internal slots, recieving signals from GUI events and timers

***************************************************************/

void MainWindow::serverButtonClicked() {
	//Fire the configure signal to the backend
	emit(configureUpdater());
}

void MainWindow::configButtonClicked() {
	GeneralConfigWindow *win = new GeneralConfigWindow(); //deletes itself
	connect(win, SIGNAL(configChanged()), this, SLOT(readConfig()));
	win->show();
}

void MainWindow::installButtonClicked() {
	//Fire the  install signal to the backend and hide ourselves
	emit(startInstall());
	hide();
}

//Make the main window hide, not quit
void MainWindow::closeEvent(QCloseEvent *e) {
	hide();
}

/*************************************************************************
	NOTE: These two functions assume we pack the list with QCheckListItems.
	If this is not the case, the Updater class needs to disable these buttons. 
	You do this by emitting the "disableSelectButtons" signal. 
*************************************************************************/

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


void MainWindow::checkUpdates() {
	//Clear the list and fire the update signal to the backend
	updateList->clear();
	packageDescription->setText("");
	disableButtons(true);
	updatesSelected = 0;

	emit(populateUpdateList(updateList)); 
}

//Selection/Fetch Description slots
void MainWindow::slotPackageSelected(QListViewItem *packageSelected) {
	if (selectAllButton != NULL) {
		//We have to enable the select buttons here, if they exist
		disableButtons(false); 	
	} 
	emit(updateSelected(packageSelected));
}

void MainWindow::slotPackageClicked(QListViewItem *pack) {
	disableButtons(false);
}

//Menu/Lock Menu Slot
void MainWindow::slotPackageRightClicked(QListViewItem *update, const QPoint &point, int col) {
	emit(updateMenu(update, point));
}

//This is where we actually close, called from the system tray
void MainWindow::slotExit() {
	kapp->quit();
}

