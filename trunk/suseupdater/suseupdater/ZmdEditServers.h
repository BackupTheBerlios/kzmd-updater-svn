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

#ifndef _ZMD_EDIT_SERVERS_H_
#define _ZMD_EDIT_SERVERS_H_

#include <kpushbutton.h>

#include <qlistview.h>
#include <qvbox.h>
#include <qlayout.h>

#include "ZmdUpdaterCore.h"

class ZmdEditServers : public QWidget {

	Q_OBJECT

	public:

		ZmdEditServers(ZmdUpdaterCore *_core, QWidget *parent=0);

	signals:

		//We are going to fire this off after an add/remove to update the list
		void refreshUpdates();

	private slots:

		void addButtonClicked();
		void removeButtonClicked();

		//Core Signals

		void gotServiceList(QValueList<Service>);
		void gotCatalogList(QValueList<Catalog>);
		void addedServer(QString, int, QString);
		void serverFault(QString);

	private:

		void initGUI();
		void initList();

		QVBoxLayout	*mainLayout;
		QHBoxLayout *buttonLayout;
		QListView *serverList;
		KPushButton *addButton;
		KPushButton *removeButton;
		KPushButton *closeButton;

		ZmdUpdaterCore *core;
};

#endif

