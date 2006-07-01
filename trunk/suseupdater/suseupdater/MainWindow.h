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


#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_

#include <ksystemtray.h>
#include <ktextedit.h>
#include <kpushbutton.h>

#include <qlistview.h>
#include <qlabel.h>
#include <qtimer.h>

#include "Updater.h"
#include "Constants.h"
#include "HeaderWidget.h"

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
		void readConfig();
		void disableSelectButtons();

	private slots:

		void slotExit();
		void slotPackageSelected(QListViewItem *packageSelected);

		void configButtonClicked();
		void serverButtonClicked();
		void installButtonClicked();
		void selectButtonClicked();
		void clearButtonClicked();

		void closeEvent(QCloseEvent *e);

	private:
		
		void initGUI();
		void initMenu();

		KSystemTray *applet;
		QListView *updateList;
		KTextEdit *packageDescription;
		HeaderWidget *header;

		KPushButton *installButton;
		KPushButton *cancelButton;
		KPushButton *configureButton;
		KPushButton *selectAllButton;
		KPushButton *clearSelectionButton;

		QVBoxLayout *mainBox;
		QHBoxLayout *selectionButtonsLayout;
		QHBoxLayout *buttonsLayout;
		QTimer *timer;
		int timerInterval;

};

#endif

