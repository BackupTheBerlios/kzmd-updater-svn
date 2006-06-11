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

#include "ConfigWindow.h"
#include <klocale.h>

ConfigWindow::ConfigWindow(QWidget *parent) : QWidget(parent) {

	initGUI();

}


ConfigWindow::~ConfigWindow() {
}

void ConfigWindow::initGUI() {

	mainLayout = new QVBoxLayout(this);
	header = new HeaderWidget(this);
	serverList = new QListView(this);
	addButton = new KPushButton(i18n("Add Server"), this);
	removeButton = new KPushButton(i18n("Remove Server"), this);
	closeButton = new KPushButton(i18n("Close"), this);


	header->setDescription("<b>Add/Remove Package Servers:</b><br> You may add or remove update servers below or change your software catalog subscriptions.<br> <u>Make whatever changes you wish and press accept.</u>");

	mainLayout->addWidget(header);
	mainLayout->addWidget(serverList);

	buttonLayout = new QHBoxLayout(mainLayout);
	buttonLayout->addWidget(addButton, false, 0);
	buttonLayout->addSpacing(100);
	buttonLayout->addWidget(removeButton, false, 0);

	mainLayout->addWidget(closeButton, false, Qt::AlignRight);

	mainLayout->setMargin(10);	
	setMinimumWidth(400);
	setMinimumHeight(500);
	setCaption(i18n("Configure Updater"));
	show();
}
