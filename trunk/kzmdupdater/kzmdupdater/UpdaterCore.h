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


/* Error Codes */

#define SUCCEEDED 0
#define ERROR_DEPENDENCY_FAILURE 1
#define ERROR_INVALID_URI 2

//More will obviously be added later

/* Data Types */

struct Service {

	QString name;
	QString id;
	QString uri;
	QString type;
	bool activated;
	
};

struct Catalog {

	QString name;
	QString id;
	QString displayName;
	QString service;
	bool subscribed;
};

struct Package {

	QString name;
	QString id;
	QString version;
	QString catalog;
	QString description;
	bool installed;	
};

struct Patch {

	QString name;
	QString id;
	QString version;
	QString catalog;
	QString description;
	QString category;
	bool installed;
};

/* Core Backend Class */

class UpdaterCore : public QObject {

	Q_OBJECT

	public:

		virtual QValueList<Service> *getServices() = 0;
		virtual void addService(Service) = 0;
		virtual void removeService(Service) = 0;

		virtual QValueList<Catalog> *getCatalogs() = 0;
		virtual void subscribeCatalog(Catalog) = 0;
		virtual void unsubscribeCatalog(Catalog) = 0;

		virtual QValueList<Patch> *getPatches(Catalog) = 0;
		virtual QValueList<Package> *getUpdates(Catalog) = 0;

		virtual void runTransaction(QValueList<Patch>*, QValueList<Package>*) = 0;

	signals:

		virtual void serviceListing(QValueList<Service>) = 0;
		virtual void catalogListing(QValueList<Catalog>) = 0;
		virtual void patchListing(QValueList<Patch>) = 0;
		virtual void updateListing(QValueList<Package>) = 0;

		virtual void transactionProgress(int progress) = 0;
		virtual void transactionFinished(int flags) = 0;
		virtual void serviceChange(int flags) = 0;
		virtual void catalogChange(int flags) = 0;

};

#endif
