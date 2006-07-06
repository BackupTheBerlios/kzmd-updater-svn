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

#include <klocale.h>

#include "ZmdConfigWindow.h"

#include <iostream>
using namespace std;

ZmdConfigWindow::ZmdConfigWindow(ZmdUpdaterCore *_core, QWidget *parent) : QWidget(parent, 0, Qt::WDestructiveClose | Qt::WShowModal) {
	core = _core;
	initGUI();
}

void ZmdConfigWindow::initGUI() {

	mainLayout = new QVBoxLayout(this);
	header = new HeaderWidget(this);
	tabs = new QTabWidget(this);
	closeButton = new KPushButton(i18n("Close"), this);
	editServers = new ZmdEditServers(core, tabs);
	advancedConfig = new ZmdAdvancedConfig(tabs);

	mainLayout->addWidget(header, 0, 0);
	mainLayout->addWidget(tabs, 0, 0);
	mainLayout->addWidget(closeButton, 0, Qt::AlignRight);
	tabs->addTab(editServers, i18n("Add/Remove Servers"));
	tabs->addTab(advancedConfig, i18n("Advanced Options"));

	header->setDescription(i18n("<b>Add/Remove Package Servers:</b><br> You may add or remove update servers below or change your software catalog subscriptions.<br> <u>Make whatever changes you wish and press accept.</u>"));

	connect(editServers, SIGNAL(refreshUpdates()), this, SLOT(serverChange()));
	connect(tabs, SIGNAL(currentChanged(QWidget *)), this, SLOT(tabChanged(QWidget *)));
	connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
	
	mainLayout->setSpacing(10);
	mainLayout->setMargin(10);
	resize(250,500);
}

void ZmdConfigWindow::serverChange() {
	emit(refreshUpdates());
}

void ZmdConfigWindow::tabChanged(QWidget *tab) {
	if (tab == NULL)
		return;
	if (QString(tab->name()) == QString("EditTab")) {
		header->setDescription(i18n("<b>Add/Remove Package Servers:</b><br> You may add or remove update servers below or change your software catalog subscriptions.<br> <u>Make whatever changes you wish and press accept.</u>"));
	} else if (QString(tab->name()) == QString("AdvancedTab")) {
		header->setDescription(i18n("<b>Configure ZMD:</b><br>The following are advanced options for ZMD. Only change these if you know what you are doine. <br><u>Be aware, disabling TCP support will disable this updater</u>"));
	}
}

