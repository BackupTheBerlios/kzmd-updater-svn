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

#include <iostream>
#include <fstream>
using namespace std;

#include "RepoFeed.h"
#include "FeedDialog.h"

#define ZMD_CONFIG_PATH "/etc/zmd"

enum {
	COLUMN_NAME,
	};


MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
	initGUI();

	core = new ZmdUpdaterCore();
	core->setServer("http://127.0.0.1:2544/zmd/RPC2");
	authorizeCore();

	connect(core, SIGNAL(serviceListing(QValueList<Service>)), 
					this, SLOT(services(QValueList<Service>)));
	connect(core, SIGNAL(generalFault(QString, int)), this, SLOT(fault(QString, int)));

	feed = new RepoFeed();
	feed->setUrl("http://www.osnews.com/files/recent.xml");
	feed->fetch();
	connect(feed, SIGNAL(fetchDone()), this, SLOT(fetchDone()));
}

void MainWindow::initGUI() {

	mainLayout = new QGridLayout(this);
	list = new QListView(this);
	addButton = new KPushButton(KStdGuiItem::add(), this);
	removeButton = new KPushButton(KStdGuiItem::remove(), this);
	feedButton = new KPushButton(i18n("Set RSS Feed"), this);
	closeButton = new KPushButton(KStdGuiItem::close(), this);

	list->addColumn(i18n("Repositories"));

	connect(addButton, SIGNAL(clicked()), this, SLOT(addButtonClicked()));
	connect(removeButton, SIGNAL(clicked()), this, SLOT(removeButtonClicked()));
	connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(feedButton, SIGNAL(clicked()), this, SLOT(feedButtonClicked()));

	mainLayout->addMultiCellWidget(list, 0, 0, 0, 2);
	mainLayout->addWidget(addButton, 1, 0);
	mainLayout->addWidget(removeButton, 1, 2);
	mainLayout->addWidget(feedButton, 2, 1);
	mainLayout->addWidget(closeButton, 2, 2);
	mainLayout->setSpacing(10);
	mainLayout->setMargin(10);
	setCaption(i18n("Add/Remove Repositories"));
	resize(500, 300);
	show();
}

void MainWindow::addButtonClicked() {

}

void MainWindow::removeButtonClicked() {

}

void MainWindow::feedButtonClicked() {
	FeedDialog *diag = new FeedDialog();
	if (diag->exec() == QDialog::Accepted) {
		if (diag->url().isEmpty() == false)
			feed->setUrl(diag->url());
			feed->fetch();
	}
	delete diag;
}

void MainWindow::fetchDone() {
	QValueList<Repo> rlist = feed->repos();
	QValueList<Repo>::iterator iter;

	for (iter = rlist.begin(); iter != rlist.end(); iter++) {
		QListViewItem *item = new QListViewItem(list, (*iter).title);
	}
}

void MainWindow::services(QValueList<Service>) {

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
