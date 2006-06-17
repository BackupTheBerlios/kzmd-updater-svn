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

/* Error Codes */

#define SUCCEEDED 0
#define ERROR_DEPENDENCY_FAILURE 1
#define ERROR_INVALID_URI 2
#define ERROR_AUTH_REJECT 3

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

		UpdaterCore(QObject *parent=0) : QObject(parent) {}

		virtual void setUser(QString) = 0;
		virtual void setPass(QString) = 0;

		virtual void getServices() = 0;
		virtual void addService(Service) = 0;
		virtual void removeService(Service) = 0;

		virtual void getCatalogs() = 0;
		virtual void subscribeCatalog(Catalog) = 0;
		virtual void unsubscribeCatalog(Catalog) = 0;

		virtual void getPatches(Catalog) = 0;
		virtual void getUpdates(Catalog) = 0;

		virtual void runTransaction(QValueList<Patch>*, QValueList<Package>*) = 0;

	signals:

		void serviceListing(QValueList<Service>);
		void catalogListing(QValueList<Catalog>);
		void patchListing(QValueList<Patch>);
		void updateListing(QValueList<Package>);

		void transactionProgress(int progress);
		void transactionFinished(int flags);
		void serviceChange(int flags);
		void catalogChange(int flags);

};

#endif

