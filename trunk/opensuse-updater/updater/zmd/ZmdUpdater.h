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

/**
	@file

	This file defines our ZMD Backend and the path to the zmd configuration files.

	@author Narayan Newton <narayannewton@gmail.com>
*/

//This is where the secret and deviceid files are stored
//These hold the username and password for zmd
#define ZMD_CONFIG_PATH "/etc/zmd"

/** Enum to represent the two protocols for ZMD **/
enum {
  ZMD_TCP,	/** TCP **/
  ZMD_UDS,	/** Unix Domain Sockets **/
};

/* The TCP and UDS address of the local zmd server */
#define TCP_SERVER_ADDY "127.0.0.1"
#define TCP_SERVER_POSTFIX ":2544/zmd/RPC2"
#define UDS_SERVER_ADDY "udshttp:/var/run/zmd/zmd-web.socket"

//Program Options
#define BUGGY_ZMD //turns on bug workarounds for ZMD
#define NO_PACKAGE_LOCKS //turns off support for locking packages

/**
		Our ZMD Backend class. Is connected to the
		MainWindow class via UpdaterApp
	*/
class ZmdUpdater : public Updater
{

  Q_OBJECT

public:

  ZmdUpdater();

  virtual UpdaterCapabilities capabilities();

private slots:

  //Slots implemented from Updater abstract class

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


  //Our own slots
  void startRefresh();
  void gotLockListing(QValueList<PackageLock>);
  void gotCatalogListing(QValueList<Catalog>);
  void gotServiceListing(QValueList<Service>);
  void gotUpdateListing(QValueList<Package>);
  void gotPatchListing(QValueList<Patch>);
  void gotPackageInfo(Package);
  void gotPatchInfo(Patch);
  void gotPackageDetails(PackageDetails);
  void gotDepInfo(QString, QValueList<Package>,
                  QValueList<Package>,
                  QValueList<Package>,
                  QValueList<Package>);
  void holdPackage();
  void removeHold();

  //Error handling
  void error(QString, int); //Recieves error messages (generalFault) from the backend.


private:

  /**
  	Does the auth routine for the core ZMD functions. 
  	Basically, just gets the user/pass and gives it to 
  	the backend. This will change later when we get a real 
  	auth routine.
  */
  void authorizeCore();

  //Watch variable to control the generalFault message (ZMD TCP Error)
  //We should only show the error on first data drop, otherwise error is probably due to
  //zmd shutting down, sleeping or suspend.
  bool showGeneralFaultError;

  //Read in configuration for ZMD and tell the core backend what protocol to use
  void readConfig();

  //We hold the QListView passed in "populateUpdateList" here
  QListView *tempList;

  //This holds the currently selected update in the list
  QListViewItem *currentUpdate;

  //Holds the descript for the currently selected update
  QString currentDescription;

  //Holds a mapping of the catalog name to the catalog display name.
  QMap<QString, QString> catalogNames;

  //Experimental patch handling
  QMap<QString, QValueList<Package> > patchDeps;

  //Experimental package/patch handling
  QMap<QString, Package> currentPackages;
  QMap<QString, Patch> currentPatches;

  //The core updater functionality
  ZmdUpdaterCore *core;

};

#endif

