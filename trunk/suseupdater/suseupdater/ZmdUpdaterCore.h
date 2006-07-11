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
#include <qtimer.h>

#include "xmlrpciface.h"
#include "ZmdUpdaterCoreUtils.h"

#define DEBUG
//#define _ABORT_SUPPORTED_

/* Our error codes, there probably should be more of these, but not now */
enum {

	ERROR_NONE = 0,
	ERROR_DEP_FAIL,
	ERROR_INVALID,
	ERROR_INVALID_TYPE,
	ERROR_AUTH_REJECT,
	ERROR_TRANS_FAIL
};

/* The TCP address of the local zmd server */
#define SERVER_ADDY "http://127.0.0.1:2544/zmd/RPC2"

//We poll the server ever 1 second on transactions
#define CHECK_INTERVAL (1000)  //1 second

//Some small macros to ensure two operations do not happen at the same time
#define IS_ZMD_BUSY if (pollID.isEmpty() == false) return
#define ZMD_BLOCK(ID) (pollID = (ID))
#define ZMD_CLEAR (pollID = downloadID = "")

class ZmdUpdaterCore : public QObject {

	Q_OBJECT
	
	public:

		ZmdUpdaterCore(QObject *parent=0);
		~ZmdUpdaterCore();

		/**
			User/Pass functions for the temp authorization
		**/
		void setUser(QString);
		void setPass(QString);
   
   		/**
			Sends a request for the list of registered servers on ZMD
			Data arrives via serviceListing signal.
		**/
		void getServices();

		/**
			Sends a request for an addition of the service specified. Type, URI and Name
			required. Data arrives via serviceAdded.
		**/
		void addService(Service);

		/**
			Sends a request to delete the specified service. No data return, this is problematic.
			I would love there to be a return, but there isn't. A general fault maybe fired from 
			this.
		**/
		void removeService(Service);

		/**
			Send a request for the list of catalogs currently provided by the service on ZMD. 
			Note, these are all the catalogs, not all the subscribed catalogs. Returns data via
			catalogListing signal.
		**/
		void getCatalogs();

		/**
			Send a request to subscribe to the specified catalog. Catalog ID is required.
			Has no return data.
		**/
		void subscribeCatalog(Catalog);

		/**
			Send a request to unsubscribe to the specified catalog. Catalog ID is required.
			Has no return data.
		**/
		void unsubscribeCatalog(Catalog);

		/**
			Get the Patches available for a specified catalog. Catalog ID is required.
			Data returns via patchListing.
		**/
		void getPatches(Catalog);

		/**
			Get the updates available for a specified catalog. Catalog ID is required.
			Data returns via updateListing.
		**/
		void getUpdates(Catalog);

		/**
			Get the info for an installed package. Searches via package name.
			Data returns iva packageInfo.
		**/
		void getInfo(QString packageName);

		/**
			Get the details for an installed package.
			Returns via packageDetails.
		**/
		void getDetails(Package);

		/**
			Lock package (hold back from being upgraded)
		**/
		//void lockPackage(Package);

		/**
			Remove a package lock
		**/
		//void unlockPackage(Package);

		/**
			Get lock info - returns a list of locked packages
		**/
		//void getLocks();

		/**
			Start a package transaction, sends requests for dep tree verification and 
			dep resolution. Data returns via signal realPackages.
		**/
		void startTransaction(QValueList<Package> installList, 
						   	  QValueList<Package> updateList,
							  QValueList<Package> removeList);

		/**
			Actually run the transaction you have just started. startTransaction MUST
			be run before runTransaction. Data returns via transactionFinished and progress
			and downloadProgress.
		**/
		void runTransaction();

		/**
			Cancels the current transaction. Currently, only stops it in our backend, not in ZMD.
		**/
		void cancelTransaction();

	signals:

		void serviceListing(QValueList<Service>);
		void catalogListing(QValueList<Catalog>);
		void patchListing(QValueList<Patch>);
		void updateListing(QValueList<Package>);

		void packageInfo(Package);
		void packageDetails(PackageDetails);

		//Signal sent to the GUI
		//to inform of packages required to resolve
		//deps. The real install list.
		void realPackages(QValueList<Package> installs,
						  QValueList<Package> removals,
						  QValueList<Package> updates);

		//Signals that end transactions
		void transactionFinished(int flags, QString errorMessage);

		void serviceAdded(QString serviceName, int flags, QString errorMessage);

		//General Fault Signal - For non-specific faults
		void generalFault(QString faultMessage);

		/**
			General progress signal, reports progress on any on-going process.
		**/
		void progress(Progress);

		//Special progress signal for the download
		void downloadProgress(Progress);


	private slots:

		void catalogData(const QValueList<QVariant>&, const QVariant&);

		//temp
		void catalogSubData(const QValueList<QVariant>&, const QVariant&);

		void serviceData(const QValueList<QVariant>&, const QVariant&);

		void updateData(const QValueList<QVariant>&, const QVariant&);
		void patchData(const QValueList<QVariant>&, const QVariant&);

		void infoData(const QValueList<QVariant>&, const QVariant&);

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

		//Watch variables, temporary...we use these to get around a zmd bug with catalog subscription.
		QString catalogID;
		bool catalogStatus;

		//Obvious, we cannot save these from signal to signal, so we store them here
		QValueList<QVariant> packagesToInstall;
		QValueList<QVariant> packagesToUpdate;
		QValueList<QVariant> packagesToRemove;

		KXMLRPC::Server *server; 
		QTimer *timer;
};


#endif
