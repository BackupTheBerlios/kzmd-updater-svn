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

#ifndef _UPDATER_H_
#define _UPDATER_H_

#include <qobject.h>
#include <qlistview.h>

#include "Constants.h"

/**
	Constants that define the various states our applet can be in. 
	Currently APPLET_CHECKING is a useless constant.
*/

enum { 	APPLET_NO_UPDATES,
		APPLET_UPDATES,
		APPLET_CHECKING };

/**
	@file

	This is the base class for all updater backends. 
	If a backend class conforms to this, it will work

	@author Narayan Newton

**/


/**
	Parent class for all updater backends
*/
class Updater : public QObject {

	Q_OBJECT

	public:

		Updater() : QObject() {}

	signals:

		/** This is a signal sent to the MainWindow class. It updates the applet
			to the specified state.
			@param state this is the state the updater applet will be refreshed to reflect
		**/
		void updateApplet(int state);

		/** 
			Send this signal to force an update list refresh. You should do this after an installation or
			after a service is added to the updater
		**/
		void refreshList();

		/**
			Send this signal to disable the two selection buttons on the GUI. You do this if you
			are not going to load the list with UpdateListItems. If you don't disable these buttons,
			we assume you are using UpdateListItems.
		**/
		void disableSelectButtons();

		/**
			Fire this signal to deliver the description of the currently selected update back
			to the main window.

			@param desc the description text which will be displayed in the Main Window
		**/
		void returnDescription(QString desc);

		/**
			Fire this signal to inform the MainWindow that the updates are all added to the list.
			This can be sent multiple times, it just causes the MainWindow to handle the new updates
		**/
		void populateDone();

	protected slots:

		/**
			When signaled, this slot should run whatever configuration utility this updater 
			uses. However, this is marked as Add/Remove Servers in the GUI, so it should at least
			be able to do that.
		**/
		virtual void configureUpdater() = 0;

		/**
			When signaled, this slot should run the installation routine for the updater. It
			is important to note that you get the package list from the QListView you will _ALWAYS_
			be given in the "populateUpdateList" slot. This pointer should  be saved for later use in 
			this slot (so you can fetch the package list).
		**/
		virtual void startInstall() = 0;

		/**
			When signaled, this slot should fetch a list of current updates available and add them
			to the listview specified. 

			@param updateList the QListView we will be loading with updates.
		**/
		virtual void populateUpdateList(QListView* updateList) = 0;

		/**
			This slot is signaled when an update is selected and we need to put together
			a description for it.

			@param selectedItem the currently selected update. 
		**/
		virtual void updateSelected(QListViewItem* selectedItem) = 0;

		/**
			This slot is signaled when an update is right clicked. What menu comes up is entirely
			the decision of the backend, but it is expected that this will control holding packages.

			@param item the QListViewItem currently selected.
			@param point where to show the menu.
		**/
		virtual void updateMenu(QListViewItem *item, const QPoint &point) = 0;
};

#endif

