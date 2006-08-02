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

#include "EditWindow.h"

#include <klocale.h>
#include <klineedit.h>
#include <kpushbutton.h>

#include <qlabel.h>
#include <qlayout.h>

#include "ZmdUpdaterCore.h"

EditWindow::EditWindow(QWidget *parent) : QDialog(parent) {

	initGUI();
}

EditWindow::~EditWindow() {
	if (user == NULL)
		return;
	user->name = nameEdit->text();
	user->privs = privEdit->text();
}

void EditWindow::initGUI() {

	layout = new QGridLayout(this);
	nameLabel = new QLabel(i18n("User Name"), this);
	nameEdit = new KLineEdit(this);
	passLabel = new QLabel(i18n("Password"), this);
	passEdit = new KLineEdit(i18n("No Password Required"), this);
	privLabel = new QLabel(i18n("Privileges"), this);
	privEdit = new KLineEdit(this);
	closeButton = new KPushButton(i18n("Close"), this);

	connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

	layout->addWidget(nameLabel, 0, 0);
	layout->addWidget(nameEdit, 0, 1);
	layout->addWidget(passLabel, 1, 0);
	layout->addWidget(passEdit, 1, 1);
	layout->addWidget(privLabel, 2, 0);
	layout->addWidget(privEdit, 2, 1);
	layout->addWidget(closeButton, 3, 1);
	layout->setMargin(10);
	layout->setSpacing(10);
	resize(300,100);
	show();
}

void EditWindow::setIdentity(Identity *id) {
	user = id;
	if (user != NULL && user->name.isEmpty() == false) {
		nameEdit->setText(user->name);
		privEdit->setText(user->privs);
	}
}

