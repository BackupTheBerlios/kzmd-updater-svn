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

#include <ksystemtray.h>
#include <ktextedit.h>

#include <qlistview.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qtimer.h>

#include "Updater.h"
#include "HeaderWidget.h"

#define TIMER_INTERVAL	(1000*60*15) //1000 ms * 60 s * 5 min = 15 min

#define TRAY_ICON_GREEN "suse_green.png"
#define TRAY_ICON_RED	"suse_red.png"


//Define the columns of the update list. These must be set

enum { 	COLUMN_NAME,
		COLUMN_OLD_VERSION,
		COLUMN_NEW_VERSION,
		COLUMN_SIZE,
		COLUMN_ID,
		COLUMN_DESC,
		COLUMN_INSTALLED,
		COLUMN_CATALOG };


class MainWindow : public QWidget {

	Q_OBJECT

	public:

		MainWindow(QWidget *parent=0);

	signals:

		void populateUpdateList(QListView*);
		void startInstall();
		void configureUpdater();

	public slots:

		void appletState(int);
		void checkUpdates();

	private slots:

		void slotExit();
		void slotPackageSelected(QListViewItem *packageSelected);
		void configButtonClicked();
		void installButtonClicked();

	private:

		void initGUI();
		void initMenu();

		KSystemTray *applet;
		QListView *updateList;
		KTextEdit *packageDescription;
		HeaderWidget *header;

		QPushButton *installButton;
		QPushButton *cancelButton;
		QPushButton *configureButton;

		QVBoxLayout *mainBox;
		QHBoxLayout *buttonsLayout;
		QTimer *timer;

};

#endif
