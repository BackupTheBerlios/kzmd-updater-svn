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

#ifndef _USER_EDIT_MAIN_WINDOW_H_
#define _USER_EDIT_MAIN_WINDOW_H_

#include <qwidget.h>
#include "ZmdUpdaterCore.h"

class QListView;
class KPushButton;
class QGridLayout;
class RepoFeed;

class MainWindow : public QWidget {

	Q_OBJECT

	public:

		MainWindow(QWidget *parent=0);


	private slots:

		void addButtonClicked();
		void serviceAdded(QString, int, QString);
		void removeButtonClicked();
		void serviceRemoved();
		void feedButtonClicked();
		void fetchDone();
		void services(QValueList<Service>);
		void fault(QString, int);

	private:

		void initGUI();
		void authorizeCore();
		void refreshList();

		QGridLayout *mainLayout;
		QListView *list;
		KPushButton *addButton;
		KPushButton *removeButton;
		KPushButton *closeButton;
		KPushButton *feedButton;

		QValueList<Service> currentServices;

		ZmdUpdaterCore *core;
		RepoFeed *feed;

};

#endif
