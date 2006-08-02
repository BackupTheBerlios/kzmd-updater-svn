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

#include "MainWindow.h"

#include <kprocess.h>
#include <kdebug.h>
#include <kpushbutton.h>
#include <klocale.h>
#include <kmessagebox.h>

#include <qlayout.h>
#include <qlistview.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/poll.h>
#include <fcntl.h>

#include "EditWindow.h"

#include <iostream>
#include <fstream>
using namespace std;

#define ZMD_CONFIG_PATH "/etc/zmd"

enum {
	COLUMN_NAME,
	COLUMN_PRIV, };


MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
	initGUI();

	core = new ZmdUpdaterCore();
	core->setServer("http://127.0.0.1:2544/zmd/RPC2");
	authorizeCore();

	connect(core, SIGNAL(userListing(QValueList<Identity>)), this, SLOT(userData(QValueList<Identity>)));
	connect(core, SIGNAL(generalFault(QString, int)), this, SLOT(fault(QString, int)));

	core->listUsers();
}

void MainWindow::initGUI() {

	mainLayout = new QGridLayout(this);
	list = new QListView(this);
	addButton = new KPushButton(i18n("Add User"), this);
	removeButton = new KPushButton(i18n("Remove User"), this);
	modifyButton = new KPushButton(i18n("Modify User"), this);
	closeButton = new KPushButton(i18n("Close"), this);

	list->addColumn("User Name");
	list->addColumn("User Privileges");

	connect(addButton, SIGNAL(clicked()), this, SLOT(addButtonClicked()));
	connect(removeButton, SIGNAL(clicked()), this, SLOT(removeButtonClicked()));
	connect(modifyButton, SIGNAL(clicked()), this, SLOT(modifyButtonClicked()));
	connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

	mainLayout->addMultiCellWidget(list, 0, 0, 0, 2);
	mainLayout->addWidget(addButton, 1, 0);
	mainLayout->addWidget(removeButton, 1, 1);
	mainLayout->addWidget(modifyButton, 1, 2);
	mainLayout->addWidget(closeButton, 2, 3);
	mainLayout->setSpacing(10);
	mainLayout->setMargin(10);

	show();
}

void MainWindow::addButtonClicked() {
	Identity id;
	EditWindow *win = new EditWindow();
	win->setIdentity(&id);
	win->exec();
	delete win; //must delete the window to trigger the setting of ID
	if (id.name != "") {
		core->addUser(id);
		core->listUsers();
	}
}
void MainWindow::removeButtonClicked() {
	QListViewItem *item = list->currentItem();

	if (item != NULL) {
		core->removeUser(item->text(COLUMN_NAME));
		core->listUsers();
	}
}
void MainWindow::modifyButtonClicked() {
	Identity id;
	EditWindow *win = new EditWindow();
	QListViewItem *item = list->currentItem();

	if (item != NULL) {
		id.name = item->text(COLUMN_NAME);
		id.privs = item->text(COLUMN_PRIV);
		win->setIdentity(&id);
		win->exec();
		delete win; //must delete the window to trigger the setting of ID
		if (id.name != "") {
			core->modifyUser(id);
			core->listUsers();
		}
	}
}

void MainWindow::userData(QValueList<Identity> data) {
	QValueList<Identity>::iterator iter;

	list->clear();
	for (iter = data.begin(); iter != data.end(); iter++) {
		QListViewItem *item = new QListViewItem(list, (*iter).name, (*iter).privs);
	}
}

void MainWindow::fault(QString message, int code) {
	KMessageBox::error(this, message);
}

void MainWindow::authorizeCore() {

	char buffer[1024];

	ifstream file;

	file.open(ZMD_CONFIG_PATH + QString("/") + QString("deviceid"));
	if (file.is_open() == true) {
		file >> buffer;
		cout << buffer << endl;
		core->setUser(buffer);
	}
	memset(buffer, '\0', 1024);
	file.close();
	file.open(ZMD_CONFIG_PATH + QString("/") + QString("secret"));
	if (file.is_open() == true) {
		file >> buffer;
		cout << buffer << endl;
		core->setPass(buffer);
	} else {
		KMessageBox::error(this, i18n("We must be run as root"));
		exit(-1);
	}
	memset(buffer, '\0', 1024);
	file.close();
}
