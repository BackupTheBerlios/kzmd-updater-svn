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

#include "MainWindow.h"
#include "Updater.h"


MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {

	KIconLoader iconLoader(PROGRAM_NAME);
	applet = new KSystemTray(this);
	applet->setPixmap(UserIcon(TRAY_ICON_GREEN));
	applet->setScaledContents(true);
	applet->show();
	timer = new QTimer(this);

	connect(timer, SIGNAL(timeout()), this, SLOT(checkUpdates()));
	timer->start(TIMER_INTERVAL,false);
	initGUI();
	initMenu();
}

//Build GUI, setup system tray and hide GUI initially.
void MainWindow::initGUI() {
	
	mainBox = new QVBoxLayout(this);
	header = new HeaderWidget(this);
	updateList = new QListView(this);
	packageDescription = new KTextEdit(this);
	configureButton = new QPushButton(i18n("Configure Updater"),this);
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
	resize(400,500);
	setIcon(UserIcon(TRAY_ICON_GREEN));
	hide();
	return;
}

void MainWindow::initMenu() {
	KPopupMenu *menu = applet->contextMenu();
//	menu->insertItem(i18n("About"), this, SLOT(configButtonClicked()),0,-1,1);
	menu->insertItem(i18n("Configure"), this, SLOT(configButtonClicked()),0,-1,1);
}

void MainWindow::appletState(int state) {

	switch (state) {

		case APPLET_CHECKING:
		case APPLET_NO_UPDATES:
			applet->setPixmap(UserIcon(TRAY_ICON_GREEN));
			break;
		case APPLET_UPDATES:
			applet->setPixmap(UserIcon(TRAY_ICON_RED));
			break;
	}

}

void MainWindow::configButtonClicked() {
	emit(configureUpdater());
}

void MainWindow::installButtonClicked() {
	emit(startInstall());
	hide();
}

void MainWindow::checkUpdates() {
	emit(populateUpdateList(updateList)); 
}

void MainWindow::slotPackageSelected(QListViewItem *packageSelected) {
	packageDescription->setText(((QCheckListItem*)packageSelected)->text(COLUMN_DESC));
}

void MainWindow::slotExit() {
	close();
}

