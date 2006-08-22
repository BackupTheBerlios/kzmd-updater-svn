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
	COLUMN_SUB,
	COLUMN_URI, 
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
	connect(feed, SIGNAL(fetchDone()), this, SLOT(fetchDone()));
}

void MainWindow::initGUI() {

	mainLayout = new QGridLayout(this);
	list = new QListView(this);
	addButton = new KPushButton(KStdGuiItem::add(), this);
	removeButton = new KPushButton(KStdGuiItem::remove(), this);
	feedButton = new KPushButton(i18n("Set RSS Feed"), this);
	closeButton = new KPushButton(KStdGuiItem::close(), this);

	list->addColumn(i18n("Repository Name"));
	list->addColumn(i18n("Subscribed"));
	list->addColumn(i18n("Uri"));
	list->setRootIsDecorated(true);

	connect(addButton, SIGNAL(clicked()), 
					this, SLOT(addButtonClicked()));
	connect(removeButton, SIGNAL(clicked()), 
					this, SLOT(removeButtonClicked()));
	connect(closeButton, SIGNAL(clicked()), 
					this, SLOT(close()));
	connect(feedButton, SIGNAL(clicked()), 
					this, SLOT(feedButtonClicked()));

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
	Service serv;
	QListViewItem *item;

	kdWarning() << "Add Button Clicked" << endl;

	if ((item = list->currentItem()) == NULL)
		return;

	if (list->currentItem()->parent() == NULL)
		item = list->currentItem();
	else
		item = list->currentItem()->parent();

	serv.name = item->text(COLUMN_NAME);
	serv.uri = item->text(COLUMN_URI);
	serv.type = "zypp";
	core->addService(serv);

	connect(core, SIGNAL(serviceAdded(QString,int,QString)),
					this, SLOT(serviceAdded(QString,int,QString)));
}

void MainWindow::serviceAdded(QString service, int flags, QString error) {
	if (flags == ERROR_NONE) {
		core->getServices();
	}
}

void MainWindow::removeButtonClicked() {
	QListViewItem *item;
	QValueList<Service>::iterator iter;
	QString uri;

	if ((item = list->currentItem()) == NULL)
		return;

	if (list->currentItem()->parent() == NULL)
		item = list->currentItem();
	else
		item = list->currentItem()->parent();

	uri = item->text(COLUMN_URI);
	for (iter = currentServices.begin(); iter != currentServices.end(); iter++) {
			if ((*iter).uri == uri) {
				core->removeService((*iter));
				connect(core, SIGNAL(serviceRemoved()), 
								this, SLOT(serviceRemoved()));
				break;
			}
		}
}

void MainWindow::serviceRemoved() {
	core->getServices();
}

void MainWindow::refreshList() {
	currentServices.clear();
	list->clear();
	core->getServices();
}

void MainWindow::feedButtonClicked() {
	FeedDialog *diag = new FeedDialog();
	if (diag->exec() == QDialog::Accepted) {
		if (diag->url().isEmpty() == false)
			feed->setUrl(QString(diag->url()));
			refreshList();
	}
	delete diag;
}

void MainWindow::fetchDone() {
	QValueList<Repo> rlist = feed->repos();
	QValueList<Repo>::iterator iter;
	QValueList<Service>::iterator sIter;

	list->clear();

	for (iter = rlist.begin(); iter != rlist.end(); iter++) {
		QListViewItem *item = new QListViewItem(list);
		item->setText(COLUMN_NAME, (*iter).title);
		item->setText(COLUMN_URI, (*iter).url);
		QListViewItem *itemDescription = new QListViewItem(item);
		itemDescription->setText(0, (*iter).description);

		cout << "About to service service list of: " << endl
								<< currentServices.count() << endl;

		for (sIter = currentServices.begin(); sIter != currentServices.end(); sIter++) {
			cout << (*sIter).uri << " " << (*iter).url << endl;
			if ( (*sIter).uri.stripWhiteSpace() == (*iter).url.stripWhiteSpace() ) {
				item->setText(COLUMN_SUB, "Yes");
				break;
			}
		}
	}
}

void MainWindow::services(QValueList<Service> services) {
	cout << "Got Service Drop" << endl;
	currentServices = services;
	feed->fetch();
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
