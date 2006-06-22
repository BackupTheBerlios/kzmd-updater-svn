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

#ifndef _UPDATER_CORE_H_
#define _UPDATER_CORE_H_

#include <qstring.h>
#include <qvaluelist.h>
#include <qobject.h>

/**
	@libdoc A class to represent an updater backend
	@author Narayan Newton
	@version 0.0.1
**/


/* Error Codes */

/**
	Defines our error codes used throughout. 
	@li ERROR_NONE is obvious, no error
	@li ERROR_DEP_FAIL is also obvious
	@li ERROR_INVALID is less obvious and determined by context
	@li ERROR_INVALID_TYPE is an invalid service type
	@li ERROR_AUTH_REJECT is obvious
**/

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


/* Core Backend Class */

class UpdaterCore : public QObject {

	Q_OBJECT

	public:

		UpdaterCore(QObject *parent=0) : QObject(parent) {}

		/**
			Sets the user name used to authenticate the updater
		**/
		virtual void setUser(QString) = 0;

		/**
			Sets the password used to authenticate the updater
		**/
		virtual void setPass(QString) = 0;

		/**
			Get a list of update servers. Data arrives via signal serviceListing
		**/
		virtual void getServices() = 0;

		/**
			Add the specified update server to the updater. Service needs type, 		   name and uri specified. 
			Signal serviceAdded sent on success or failure.
		**/
		virtual void addService(Service) = 0;

		/**
			Remove the specified update server to the updater.
			Signal serviceRemoved sent on success or failure.
		**/
		virtual void removeService(Service) = 0;

		/**
			Get a list of catalogs. Data arrives via signal catalogListing.
		**/
		virtual void getCatalogs() = 0;

		/**
			Subscribe to the specified catalog. 
			Signal catalogAdded sent on success or failure.
		**/
		virtual void subscribeCatalog(Catalog) = 0;

		/**
			Unsubscribe to the specified catalog.
			Signal catalogRemoved sent on success or failure.
		**/
		virtual void unsubscribeCatalog(Catalog) = 0;

		/**
			Get patches available (and needed) for specified catalog.
			Data arrives via signal patchListing.
		**/
		virtual void getPatches(Catalog) = 0;

		/**
			Get updates available (and needed) for specified catalog.
			Data arrives via signal updateListing.
		**/
		virtual void getUpdates(Catalog) = 0;

		/**
			BROKEN -- This api is in flux.
		**/
		virtual void runTransaction(QValueList<Package> installList, QValueList<Package> updateList, QValueList<Package> removeList) = 0;

		/**
			Cancel current transaction.
		**/
		virtual void cancelTransaction() = 0;

	signals:

		void serviceListing(QValueList<Service>);
		void catalogListing(QValueList<Catalog>);
		void patchListing(QValueList<Patch>);
		void updateListing(QValueList<Package>);
	
		void transactionFinished(int flags);

		void serviceAdded(QString serviceName, int flags);
		void serviceRemoved(QString serviceName, int flags);

		void catalogAdded(QString catalogName, int flags);
		void catalogRemoved(QString catalogName, int flags);

		/**
			General progress signal, reports progress on any on-going process.
		**/
		void progress(Progress);
};



class Progress {

	public:

		Progress() : expectedTime(0), remainingTime(0), percent(0.0L), download(false) {}

		QString name;
		int expectedTime;
		int remainingTime;
		double percent;	
		QValueList<QString> messages;
		bool download; //true if this is a download
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

struct Catalog {

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
struct Package {
	
	public:

		Package() : installed(false) {}

		QString name;
		QString id;
		QString version;
		QString catalog;
		QString description;
		bool installed;	
};

struct Patch : public Package {

	public:

		Patch() : Package() {}

		QString category;
};

#endif

