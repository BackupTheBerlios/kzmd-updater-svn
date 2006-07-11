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

#ifndef _ZMD_UPDATER_H_
#define _ZMD_UPDATER_H_

#include <qtimer.h>

#include "Updater.h"
#include "ZmdUpdaterCore.h"

//This is where the secret and deviceid files are stored
//These hold the username and password for zmd
#define ZMD_CONFIG_PATH "/etc/zmd"

class ZmdUpdater : public Updater {

	Q_OBJECT

	public:

		ZmdUpdater();

	private slots:

		//Slots implemented from Updater abstract class
		void configureUpdater();
		void startInstall();
		void populateUpdateList(QListView*);
		void updateSelected(QListViewItem*);


		//Our own slots
		void startRefresh();
		void gotCatalogListing(QValueList<Catalog>);
		void gotServiceListing(QValueList<Service>);
		void gotUpdateListing(QValueList<Package>);
		void gotPatchListing(QValueList<Patch>);
		void gotPackageInfo(Package);
		void gotPackageDetails(PackageDetails);

		//Error handling 
		void error(QString); //Recieves error messages (generalFault) from the backend.


	private:

		void authorizeCore();

		//Watch variable to control the generalFault message (ZMD TCP Error)
		bool errorShowed;

		//We hold the QListView passed in "populateUpdateList" here
		QListView *tempList;

		//This holds the currently selected update in the list
		QListViewItem *currentUpdate;

		//Holds the descript for the currently selected update
		QString currentDescription;

		//Holds a mapping of the catalog name to the catalog display name.
		QMap<QString, QString> catalogNames;

		//The core updater functionality
		ZmdUpdaterCore *core;

};

#endif

