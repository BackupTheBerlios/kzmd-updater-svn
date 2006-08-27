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

#include <qwidget.h>

class TrayIcon;
class KTextEdit;
class KPushButton;
class QListView;
class QListViewItem;
class QTimer;
class QVBoxLayout;
class QHBoxLayout;
class Updater;
class HeaderWidget;


/**
	@file

	This file defines our MainWindow class, which also controls the 
	applet itself. This is where much of the GUI logic that is 
	non-backend specific lives.

	@author Narayan Newton <narayannewton@gmail.com>
*/


/**
	Represnts the various columns, some hidden and some not, which hold 
	our package info
*/
enum { 	
		COLUMN_NAME, 						/** The Package Name or Patch Description **/
		COLUMN_TYPE,						/** Patch or Package **/
		COLUMN_NEW_VERSION,			/** The Version We Are Upgrading To **/
		COLUMN_CATALOG,					/** The Catalog **/ 
		COLUMN_ID,							/** The ID of the Package/Patch **/
		COLUMN_DESC,						/** The Description **/
		COLUMN_INSTALLED,				/** Is it installed? **/
		COLUMN_SIZE,						/** The size, that we cannot fetch at the moment **/
		COLUMN_MISC, 						/** Only used for patches, the real name **/
		COLUMN_LOCK,						/** The unused magic lock variable... **/
		COLUMN_CATEGORY,				/** The category of a patch, only used for patches **/
};  

/**
	Class to represent our MainWindow, init the applet itself and hold much of
	the GUI logic.
*/

class MainWindow : public QWidget {

	Q_OBJECT

	public:

		/**
			Init function
			@param parent the parent window, always null in our case.
		*/
		MainWindow(QWidget *parent=0);

	signals:

		/**
			Signal we send to our current backend requesting updates for the list.

			@param updateList our QListView we want the backend to pack.
		*/
		void populateUpdateList(QListView* updateList);

		/**
			Signal we send to our current backend requesting the start of 
			an installation.  Note we don't pass a package list, the backend 
			must take the info from the QListView we passed it.
		*/
		void startInstall();

		/**
			Signal we send to our current backend when the user has requested
			to config the server list. Implementation is entirely backend 
			specific.
		*/
		void configureUpdater();

		/**
			Signal we send to our current backend when the user has selected
			a particular update. We fully expect to backend to build a package
			description for us, but that is not technically required.

			@param updateSelected the recently selected update.
		*/
		void updateSelected(QListViewItem* updateSelected);

		/**
			Signal we send to our current backend when the user has right
			clicked an update. We expect a menu to be shown allowing the
			user to configure package locking, but that is entirely up
			to the backend author.

			@param updateClicked clicked the update the user right clicked.
			@param point where to display the menu
		*/
		void updateMenu(QListViewItem *updateClicked, const QPoint &point);

	public slots:

		/**
			Updates the applet to the specified state.

			@param state the state, taken from Updater.h definition.
		*/
		void appletState(int state);

		/**
			Tells the backend to repopulate our list.
		*/
		void checkUpdates();

		/**
			Reads in the config file
		*/
		void readConfig();

		/**
			Hides and deletes the "Select All" and "Clear Selection" 
			buttons. This is for backends which do not QCheckListItems.
		*/
		void disableSelectButtons();

		/**
			Gets the description from the backend. We probably just sent
			an "updateSelected" signal to signal for the description.

			@param desc the description itself.
		*/
		void gotDescription(QString desc);

		/**
			Informs us that the backend has finished populating the update
			list. We do some house keeping at this point.
		*/
		void populateDone();

	private slots:

		void slotExit();
		void slotPackageSelected(QListViewItem *packageSelected);
		void slotPackageRightClicked(QListViewItem*, const QPoint&, int);
		void slotPackageClicked(QListViewItem *);

		void configButtonClicked();
		void serverButtonClicked();
		void installButtonClicked();
		void selectButtonClicked();
		void clearButtonClicked();

		void closeEvent(QCloseEvent *e);

	private:
		
		void initGUI();
		void initMenu();

		//Decides which buttons to disable, if arg is true, 
		//we disable all the buttons. 

		//If false the logic runs
		void disableButtons(bool);

		TrayIcon *applet;
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

		//This is only for CheckListItems, holds the number of items selected
		int updatesSelected;
};

#endif

