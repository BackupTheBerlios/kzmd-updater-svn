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

#ifndef _ZMD_UPDATER_CORE_H_
#define _ZMD_UPDATER_CORE_H_

#include <qvaluelist.h>
#include <qmap.h>
#include <qtimer.h>

#include "kxmlrpcserver.h"
#include "ZmdUpdaterCoreUtils.h"


/***************************************************************************
 *
 *			@file
 * 			This is our core class definition for the zmd backend.
 *			This file defines the ZMD functions that directly send xml data
 *
 *			@author Narayan Newton <narayannewton@gmail.com>
 *
 ***************************************************************************/


#define DEBUG
//#define _ABORT_SUPPORTED_

/** Our error codes. These are handled in the ZMD backend GUI **/
enum {
  ERROR_NONE = 0,			/** No Error **/
  ERROR_DEP_FAIL,			/** Dependency Resolve Failure **/
  ERROR_INVALID,			/** Invalid URI/Invalid Repo **/
  ERROR_INVALID_TYPE,	/** Invalid Repo Type **/
  ERROR_AUTH_REJECT,	/** Authorization Incorrect/Wrong Permissions **/
  ERROR_TRANS_FAIL,		/** Transaction Failure **/
};

//We poll the server ever 1 second on transactions
#define CHECK_INTERVAL (1000)  //1 second

//Some small macros to ensure two operations do not happen at the same time
#define IS_ZMD_BUSY if (pollID.isEmpty() == false) return
#define ZMD_BLOCK(ID) (pollID = (ID))
#define ZMD_CLEAR (pollID = downloadID = "")


/******************************************************************************
 *
 *		The ZmdUpdaterCore class. Contains all the functionality that directly
 *		relates to communication with ZMD. Every low-level function we support
 *		will appear here.
 *
 *
 ******************************************************************************/

class ZmdUpdaterCore : public QObject
{

  Q_OBJECT

public:

  ZmdUpdaterCore(QObject *parent=0);
  ~ZmdUpdaterCore();

  /**
  	This method sets the url for the server, this 
  	also decides whether it uses the http ioslave 
  	or the uds ioslave.

  	@param url the address of the server
  **/
  void setServer(KURL url);

  /**
  	User function for the temp authorization
  	
  	@param user the username, taken from /etc/zmd in ZmdUpdater.cpp
  **/
  void setUser(QString user);

  /**
  	Pass function for the temp authorization
  	
  	@param pass the password, taken from /etc/zmd in ZmdUpdater.cpp
  **/
  void setPass(QString pass);

  /**
  	Add user to ZMD

  	@param id the identity to register with the backend.
  **/
  void addUser(Identity id);

  /**
  	Remove user from ZMD

  	@param user the username to remove from the backend.
  **/
  void removeUser(QString user);

  /**
  	Modify a specified user

  	@param id the id with updated information. 
  **/
  void modifyUser(Identity id);

  /**
  	List all the users registered with the backend
  	Data arrives via userListing signal.
  **/
  void listUsers();

  /**
  Sends a request for the list of registered servers on ZMD
  Data arrives via serviceListing signal.
  **/
  void getServices();

  /**
  	Sends a request for an addition of the service specified. 
  	Type, URI and Name required. Data arrives via serviceAdded.

  	@param serv the server to add, really we only need the 
  	type and uri to be set.
  **/
  void addService(Service serv);

  /**
  	Sends a request to delete the specified service. No 
  	data return, this is problematic. I would love there 
  	to be a return, but there isn't. A general fault maybe 
  	fired from this.

  	@param serv the service to remove, we only need the id.
  **/
  void removeService(Service serv);

  /**
  	Send a request for the list of catalogs currently provided 
  	by the service on ZMD. Note, these are all the catalogs, 
  	not all the subscribed catalogs. Returns data via 
  	catalogListing signal.
  **/
  void getCatalogs();

  /**
  	Send a request to subscribe to the specified catalog. 
  	Catalog ID is required. Has no return data.

  	@param cat the catalog to which we will be subscribing. 
  	We only need the ID.
  **/
  void subscribeCatalog(Catalog cat);

  /**
  	Send a request to unsubscribe to the specified catalog. 
  	Catalog ID is required. Has no return data.

  	@param cat the catalog to which we will be unsubscribing. 
  	We only need the ID.
  **/
  void unsubscribeCatalog(Catalog cat);

  /**
  	Get the Patches available for a specified catalog. 
  	Catalog ID is required. Data returns via patchListing.

  	@param cat the catalog to check for patches. Only ID needs to be set.
  **/
  void getPatches(Catalog cat);

  /**
  	Get the updates available for a specified catalog. 
  	Catalog ID is required. Data returns via updateListing.

  	@param cat the catalog to check for updates. Only ID needs to bet set.
  **/
  void getUpdates(Catalog cat);

  /**
  	Get the info for an installed package. Searches via package name.
  	Data returns via packageInfo.

  	@param packageName the name we will be using to execute 
  	a search through the packageDB.
  **/
  void getPackageInfo(QString packageName);

  /**
  	Get the info for a patch. Searches via patch name.
  	Data returns via patchInfo.

  	@param patchName the name we will be using to execute 
  	a search through the patchDB.
  **/
  void getPatchInfo(QString patchName);

  /**
  	Get the details for an installed package.
  	Returns via packageDetails.

  	@param pack the package we will fetch details for. 
  **/
  void getPackageDetails(Package pack);

  /**
  	Get the details for a patch.
  	Returns via patchDetails.

  	@param patch the patch we will fetch details for. 
  **/
  void getPatchDetails(Patch patch);

  /**
  	Lock package (hold back from being upgraded)

  	@param lock the package lock we will be adding
  **/
  void lockPackage(PackageLock lock );

  /**
  	Remove a package lock

  	@param lock the lock we will be removing, we only really need the lockid
  **/
  void unlockPackage(PackageLock lock );

  /**
  	Get lock info - returns a list of locked packages
  **/
  void getLocks();

  /**
  	Get the dependency information for a package or patch

  	@param pack The package or patch to search for
  **/
  void getDepInfo(Package pack);

  /**
  	Start a package transaction, sends requests for dep 
  	tree verification and dep resolution. Data returns 
  	via signal realPackages.

  	@param installList packages we will be installing.
  	@param updateList packages we will be updating.
  	@param removeList packages we will be removing.
  **/
  void startTransaction(QValueList<Package> installList,
                        QValueList<Package> updateList,
                        QValueList<Package> removeList);

  /**
  	Actually run the transaction you have just started. 
  	startTransaction MUST be run before runTransaction. 
  	Data returns via transactionFinished and progress 
  	and downloadProgress.
  **/
  void runTransaction();

  /**
  	Cancels the current transaction. Currently, only 
  	stops it in our backend, not in ZMD.
  **/
  void cancelTransaction();

signals:

  void userListing(QValueList<Identity>);
  void serviceListing(QValueList<Service>);
  void catalogListing(QValueList<Catalog>);
  void patchListing(QValueList<Patch>);
  void updateListing(QValueList<Package>);
  void lockListing(QValueList<PackageLock>);

  void packageInfo(Package);
  void patchInfo(Patch);
  void packageDetails(PackageDetails);
  void patchDetails(PatchDetails);
  void depInfo(QString name, QValueList<Package> provides,
               QValueList<Package> requires,
               QValueList<Package> conflicts,
               QValueList<Package> obsoletes);

  //Signal sent to the GUI
  //to inform of packages required to resolve
  //deps. The real install list.
  void realPackages(QValueList<Package> installs,
                    QValueList<Package> removals,
                    QValueList<Package> updates);

  //Signals that end transactions
  void transactionFinished(int flags, QString errorMessage);

  //Service signals
  void serviceAdded(QString serviceName, int flags, QString errorMessage);
  void serviceRemoved();

  //Catalog signal -- sub/unsub by context
  void catalogSubscriptionChanged();

  //General Fault Signal - For non-specific faults
  void generalFault(QString faultMessage, int errorCode);

  /**
  	General progress signal, reports progress on any on-going process.
  **/
  void progress(Progress);

  //Special progress signal for the download
  void downloadProgress(Progress);


private slots:

  void catalogData(const QValueList<QVariant>&, const QVariant&);

  void identityData(const QValueList<QVariant>&, const QVariant&);

  void serviceData(const QValueList<QVariant>&, const QVariant&);

  void updateData(const QValueList<QVariant>&, const QVariant&);
  void patchData(const QValueList<QVariant>&, const QVariant&);
  void lockData(const QValueList<QVariant>&, const QVariant&);

  void infoPackageData(const QValueList<QVariant>&, const QVariant&);
  void infoPatchData(const QValueList<QVariant>&, const QVariant&);
  void depData(const QValueList<QVariant>&, const QVariant&);


  void faultData(int, const QString&, const QVariant&);

  void transactData(const QValueList<QVariant>&, const QVariant&);
  void timerSlot();
  void timerData(const QValueList<QVariant>&, const QVariant&);

  void abortData(const QValueList<QVariant>&, const QVariant&);

private:
  QValueList<Package> mapListToPackageList(QValueList<QVariant>);
  //These really need to be stored somewhere else. Kwallet?
  QString username;
  QString password;

  //Holds current pollID
  // Right now we can only watch one thing
  QString pollID;
  QString downloadID; //special pollID just for the download

  //Holds the name of the server we just added/removed
  //Also may hold package IDs...just a temp variable
  //I don't like this, but I am not sure at the moment
  QString temp;

  //How many times have we timed out? If this gets above 4, we need to throw a fault.
  int timeoutCount;

  //Obvious, we cannot save these from signal to signal, so we store them here
  QMap<QString, QVariant> packagesToInstall;
  QMap<QString, QVariant> packagesToUpdate;
  QMap<QString, QVariant> packagesToRemove;

  KXmlRpcServer *server;
  QTimer *timer;
};


#endif

