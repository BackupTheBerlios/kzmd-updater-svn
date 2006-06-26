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

#define DEBUG
enum {

	ERROR_NONE = 0,
	ERROR_DEP_FAIL,
	ERROR_INVALID,
	ERROR_INVALID_TYPE,
	ERROR_AUTH_REJECT
};

/* Data Types */
class Progress;
class Package;
class Patch;
class Service;
class Catalog;

#define SERVER_ADDY "http://127.0.0.1:2544/zmd/RPC2"
#define CHECK_INTERVAL (1000)  //1 second

#define IS_ZMD_BUSY if (!pollID.isEmpty()) return
#define ZMD_BLOCK(ID) (pollID = (ID))
#define ZMD_CLEAR (pollID = "")

class ZmdUpdaterCore : public QObject {

	Q_OBJECT
	
	public:

		ZmdUpdaterCore(QObject *parent=0);
		~ZmdUpdaterCore();

		void setUser(QString);
		void setPass(QString);
    
		void getServices();
		void addService(Service);
		void removeService(Service);

		void getCatalogs();
		void subscribeCatalog(Catalog);
		void unsubscribeCatalog(Catalog);

		void getPatches(Catalog);
		void getUpdates(Catalog);

		void startTransaction(QValueList<Package> installList, 
							QValueList<Package> updateList,
							QValueList<Package> removeList);
		void runTransaction();

		void cancelTransaction();

	signals:

		void serviceListing(QValueList<Service>);
		void catalogListing(QValueList<Catalog>);
		void patchListing(QValueList<Patch>);
		void updateListing(QValueList<Package>);

		//Signal sent to the GUI
		//to inform of packages required to resolve
		//deps. The real install list.
		void realPackages(QValueList<Package> installs,
						  QValueList<Package> removals,
						  QValueList<Package> updates);
	
		void transactionFinished(int flags);

		void serviceAdded(QString serviceName, int flags);
		void serviceRemoved(QString serviceName, int flags);

		/**
			General progress signal, reports progress on any on-going process.
		**/
		void progress(Progress);

		//Special progress signal for the download
		void downloadProgress(Progress);

	private slots:

		void catalogData(const QValueList<QVariant>&, const QVariant&);
		void serviceData(const QValueList<QVariant>&, const QVariant&);

		void updateData(const QValueList<QVariant>&, const QVariant&);
		void patchData(const QValueList<QVariant>&, const QVariant&);

		void faultData(int, const QString&, const QVariant&);

		void transactData(const QValueList<QVariant>&, const QVariant&);
		void timerSlot();
		void timerData(const QValueList<QVariant>&, const QVariant&);

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
		//I don't like this, but I am not sure at the moment
		QString tempServiceName;
		QValueList<QVariant> packagesToInstall;
		QValueList<QVariant> packagesToUpdate;
		QValueList<QVariant> packagesToRemove;

		KXMLRPC::Server *server; 
		QTimer *timer;
};

class Progress {

	public:

		Progress() : expectedTime(0), remainingTime(0), status(0), percent(0.0L) {}

		QString name;
		int status;
		int expectedTime;
		int remainingTime;
		double percent;	
		QValueList<QString> messages;
};

class Service {

	public:

		Service() : activated(false) {}

		QString name;
		QString id;
		QString uri;
		QString type;
		bool activated;
	
};

class Catalog {

	public:

		Catalog() : subscribed(false) {}

		QString name;
		QString id;
		QString displayName;
		QString service;
		bool subscribed;
};

/*

	This is the "base" class. It can represent either a package 
	or a patch for the backend
*/
class Package {
	
	public:

		Package() : installed(false) {}

		QString name;
		QString id;
		QString version;
		QString catalog;
		QString description;
		bool installed;	
};

class Patch : public Package {

	public:

		Patch() : Package() {}

		QString category;
};

#endif
