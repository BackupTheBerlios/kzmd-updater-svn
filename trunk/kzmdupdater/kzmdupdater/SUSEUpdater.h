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


#ifndef _SUSE_UPDATER_
#define _SUSE_UPDATER_

#include <kmainwindow.h>
#include <ksystemtray.h>
#include <dcopclient.h>
#include <dcopobject.h>
#include <ktextedit.h>

#include <qlistview.h>
#include <qlabel.h>
#include <qvbox.h>
#include <qpushbutton.h>

#include "UpdaterCore.h"

class HeaderWidget;


class SUSEUpdater : public KMainWindow {

	Q_OBJECT

	public:

		SUSEUpdater();
		~SUSEUpdater();

	public slots:

		void slotExit();
		void slotPackageSelected(QListViewItem *packageSelected);
		void configButtonClicked();
		void installButtonClicked();

	private slots:

		void gotList(QValueList<Package>*,QValueList<Patch>*);

	private:

		void initGUI();
//		void initList();
		void checkUpdates();

		KSystemTray *trayApplet;
		QListView *updateList;
		KTextEdit *packageDescription;
		HeaderWidget *header;

		QPushButton *installButton;
		QPushButton *cancelButton;
		QPushButton *configureButton;

		QVBox *mainBox;
		QWidget *buttons;
		QHBoxLayout *buttonsLayout;

		UpdaterCore *core;

};

#endif
