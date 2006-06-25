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

#ifndef _CONFIG_WINDOW_
#define _CONFIG_WINDOW_

#include <kpushbutton.h>

#include <qlistview.h>
#include <qvbox.h>
#include <qlayout.h>

#include "HeaderWidget.h"
#include "ZmdUpdaterCore.h"


//Column IDs for the configure window
enum { CONFW_NAME=0, CONFW_URI, CONFW_ID };

class ConfigWindow : public QWidget {

	Q_OBJECT

	public:

		ConfigWindow(ZmdUpdaterCore *_core, QWidget *parent=0);
		~ConfigWindow();


	private slots:

		void addButtonClicked();
		void removeButtonClicked();


		//Core Signals

		void gotServiceList(QValueList<Service>);
		void gotCatalogList(QValueList<Catalog>);
		void addedServer(QString server, int);
		void removedServer(QString server, int);


	private:

		void initGUI();
		void initList();

		QVBoxLayout	*mainLayout;

		HeaderWidget *header;
		QListView *serverList;
		KPushButton *addButton;
		KPushButton *removeButton;
		KPushButton *closeButton;

		QHBoxLayout *buttonLayout;

		ZmdUpdaterCore *core;
};

#endif
