/* 
   Copyright (C) 2006 Novell Inc

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

#ifndef _ZYPP_UPDATER_H_
#define _ZYPP_UPDATER_H_

#include "Updater.h"

/**
	@file

	This file defines ZYpp Backend

  @author Martin Vidner <mvidner@novell.com>
	@author Duncan Mac-Vicar <dmacvicar@novell.com>
*/

class KProcess;

class ZYppUpdater : public Updater
{

	Q_OBJECT

	public:

		ZYppUpdater();

	private slots:

		/**
			Slot which gets called when we need to throw up the add/remove server dialog.
		*/
		void configureUpdater();

		/**
			Slot which gets called when we need to start an update. 
			Collect our update info from the list we got passed 
			in populateList and go. 
		*/
		void startInstall();

		/**
			Slot which gets called when we need to check for updates 
			and load up the list. 

			@param updateList the list on the main window
		*/
		void populateUpdateList(QListView* updateList);

		/**
			Slot which gets called when the user selects an update. 
			We are expected to write a description of the update and pass it back.

			@param item the update itself

		*/
		void updateSelected(QListViewItem* item);

		/**
			Slot which gets called when the user right clicks an update.

			@param updateItem the update itself.
			@param point where to show the menu
		*/
		void updateMenu(QListViewItem* item, const QPoint& point);

  protected slots:
    
    void processExited( KProcess * );
    void showLog();
    void slotProcessExited( KProcess *proc );
    void slotReceivedStdout(KProcess *proc, char *buffer, int buflen);
    void slotReceivedStderr(KProcess *proc, char *buffer, int buflen);
    
	private:
    
    void doCheckForUpdates();

		//We hold the QListView passed in "populateUpdateList" here
		QListView *tempList;

		//This holds the currently selected update in the list
		QListViewItem *currentUpdate;

		//Holds the descript for the currently selected update
		QString currentDescription;

		//Holds a mapping of the catalog name to the catalog display name.
		//QMap<QString, QString> catalogNames;

		//Experimental patch handling
		//QMap<QString, QValueList<Package> > patchDeps;

		//Experimental package/patch handling
		//QMap<QString, Package> currentPackages;
		//QMap<QString, Patch> currentPatches;
    
    KProcess *_process;
    QString _buffer;
};

#endif

