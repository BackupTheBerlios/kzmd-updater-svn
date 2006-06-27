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

#include "ZmdUpdaterCore.h"

#include <iostream>
using namespace std;

/********************************************************************
 *
 *                     Init/User/Pass Methods
 *
 ********************************************************************/

ZmdUpdaterCore::ZmdUpdaterCore(QObject *parent) : QObject(parent) {
	KURL url(SERVER_ADDY);
	server = new KXMLRPC::Server(url, this);
	server->setUserAgent("kzmdupdater");

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(timerSlot()));
}

ZmdUpdaterCore::~ZmdUpdaterCore() {
}

/* User and Pass functions */
void ZmdUpdaterCore::setUser(QString user) {
	KURL url(server->url());
	username = user;

	url.setUser(user);
	server->setUrl(url);
}

void ZmdUpdaterCore::setPass(QString pass) {
	KURL url(server->url());
	password = pass;

	url.setPass(pass);
	server->setUrl(url);
}


/********************************************************************
 *
 *                     Add/Remove/Get Services 
 *
 ********************************************************************/

void ZmdUpdaterCore::getServices() {
	IS_ZMD_BUSY;

	server->call("zmd.system.service_list", QValueList<QVariant>(),
	this, SLOT(serviceData(const QValueList<QVariant>&, const QVariant&)), 
	this, SLOT(faultData(int, const QString&, const QVariant&)));
}

void ZmdUpdaterCore::addService(Service serv) {
	IS_ZMD_BUSY;

	QValueList<QVariant> data;
	QMap<QString,QVariant> map = serv.toMap();
	data.append(QVariant(map));
	temp = serv.name;

	server->call("zmd.system.service_add", data,
	this, SLOT(serviceData(const QValueList<QVariant>&, const QVariant&)), 
	this, SLOT(faultData(int, const QString&, const QVariant&)));
}

void ZmdUpdaterCore::removeService(Service serv) {
	IS_ZMD_BUSY;

	server->call("zmd.system.service_remove", serv.id,
	this, SLOT(serviceData(const QValueList<QVariant>&, const QVariant&)), 
	this, SLOT(faultData(int, const QString&, const QVariant&)));
}

//Data slot, returning information from xml-rpc
void ZmdUpdaterCore::serviceData(const QValueList<QVariant>& data, const QVariant& t) {

	if (data.front().canCast(QVariant::String) == true && data.front().toString().isEmpty() != true) {
		//We get a string back and its not empty, must have come from adding a service
		//So, we block and start the poll loop
		ZMD_BLOCK(data.front().toString());
		timer->start(CHECK_INTERVAL,false);
	} else if (data.front().canCast(QVariant::List) == true) {
		//We got a list back, is a service list
		QValueList<QVariant> list;
		list = (data.front().toList());
		QValueList<QVariant>::iterator iter;
		QValueList<Service> serviceList;

		for (iter = list.begin(); iter != list.end(); iter++) {
			QMap<QString, QVariant> map = (*iter).toMap();
			Service serv;
			serv.fromMap(map);
			serviceList.append(serv);
#ifdef DEBUG
			cout << "Service drop: " << serv.name << endl;
#endif
		}
		emit(serviceListing(serviceList));
	} 
}

/********************************************************************
 *
 *                     Sub/Unsub/Get Catalogs 
 *
 ********************************************************************/

void ZmdUpdaterCore::getCatalogs() {
	IS_ZMD_BUSY;

	server->call("zmd.system.catalog_list", QValueList<QVariant>(), 
	this, SLOT(catalogData(const QValueList<QVariant>&, const QVariant&)),
	this, SLOT(faultData(int, const QString&, const QVariant&)));
}

void ZmdUpdaterCore::subscribeCatalog(Catalog cat) {
	IS_ZMD_BUSY;

	QValueList<QVariant> argList;
	argList.append(cat.id);
	argList.append(true);

	server->call("zmd.system.catalog_subscribe", argList, 
	this, SLOT(catalogData(const QValueList<QVariant>&, const QVariant&)),
	this, SLOT(faultData(int, const QString&, const QVariant&)));


}

void ZmdUpdaterCore::unsubscribeCatalog(Catalog cat) {
	IS_ZMD_BUSY;

	QValueList<QVariant> argList;
	argList.append(cat.id);
	argList.append(false);

	server->call("zmd.system.catalog_subscribe", argList, 
	this, SLOT(catalogData(const QValueList<QVariant>&, const QVariant&)),
	this, SLOT(faultData(int, const QString&, const QVariant&)));

}

void ZmdUpdaterCore::catalogData(const QValueList<QVariant>& data, const QVariant& t) {

	if (data.front().canCast(QVariant::String) == true) {
		/*
			If we get a string back, it is from a catalog sub/unsub
			No need to do anything else.
		*/
	} else if (data.front().canCast(QVariant::List) == true) {
		//If we get a list, it is because we got a catalog listing
		QValueList<QVariant> list;
		list = (data.front().toList());
		QValueList<QVariant>::iterator iter;
		QValueList<Catalog> catalogList;

		for (iter = list.begin(); iter != list.end(); iter++) {
			QMap<QString, QVariant> map = (*iter).toMap();
			Catalog cat;
			cat.fromMap(map);
			catalogList.append(cat);
#ifdef DEBUG
			cout << "Catalog Drop name: " << cat.name << endl;
			cout << "Catalog service: " << cat.service << endl;
#endif
		}
		emit(catalogListing(catalogList));
	}
}

/********************************************************************
 *
 *                     Get Patches/GetUpdates
 *
 ********************************************************************/

/* Package Handling (call and data slot) */
void ZmdUpdaterCore::getPatches(Catalog cat) {
	IS_ZMD_BUSY;
	server->call("zmd.packsys.get_patches", cat.id, 
	this, SLOT(patchData(const QValueList<QVariant>&, const QVariant&)),
	this, SLOT(faultData(int, const QString&, const QVariant&)));

}

void ZmdUpdaterCore::getUpdates(Catalog cat) {
	IS_ZMD_BUSY;
	server->call("zmd.packsys.get_updates", cat.id, 
	this, SLOT(updateData(const QValueList<QVariant>&, const QVariant&)),
	this, SLOT(faultData(int, const QString&, const QVariant&)));

}
void ZmdUpdaterCore::updateData(const QValueList<QVariant>& data, const QVariant&t) {
	QValueList<Package> packageList;
	
	packageList = mapListToPackageList(data.front().toList());
	emit(updateListing(packageList));
}

QValueList<Package> ZmdUpdaterCore::mapListToPackageList(QValueList<QVariant> data) {
	QValueList<QVariant>::iterator iter;
	QValueList<Package> packageList;

	for (iter = data.begin(); iter != data.end(); iter++) {
		QMap<QString, QVariant> map = (*iter).toMap();
		Package pack;

		if (map["id"].toString() == "") //bad package, try again
			continue;

		pack.fromMap(map);
		packageList.append(pack);
	}
	return packageList;
}

void ZmdUpdaterCore::patchData(const QValueList<QVariant>& data, const QVariant& t) {
	QValueList<QVariant> list;
	list = (data.front().toList());
	QValueList<QVariant>::iterator iter;
	QValueList<Patch> patchList;

	for (iter = list.begin(); iter != list.end(); iter++) {
		QMap<QString, QVariant> map = (*iter).toMap();
		Patch patch;
		patch.fromMap(map);
		patchList.append(patch);
#ifdef DEBUG
		cout << "Patch drop: " << patch.name << endl;
#endif
	}
	emit(patchListing(patchList));

}
/********************************************************************
 *
 *                     Get Package Info/Details
 *
 ********************************************************************/

void ZmdUpdaterCore::getInfo(QString packageName) {

	QValueList<QVariant> wrapper;
	QValueList<QVariant> args;

	args.append("Name");
	args.append("is");
	args.append(packageName);

	wrapper.append(args);

	server->call("zmd.packsys.query", wrapper, 
	this, SLOT(infoData(const QValueList<QVariant>&, const QVariant&)),
	this, SLOT(faultData(int, const QString&, const QVariant&)));
}

void ZmdUpdaterCore::getDetails(Package pack) {
	QMap<QString, QVariant> map;
	QValueList<QVariant> args;

	map = pack.toMap();
	args.append(map);

	//We need an id in packageDetails, but it does not return it
	temp = pack.id;


	server->call("zmd.packsys.package_details", args, 
	this, SLOT(infoData(const QValueList<QVariant>&, const QVariant&)),
	this, SLOT(faultData(int, const QString&, const QVariant&)));

}

void ZmdUpdaterCore::infoData(const QValueList<QVariant>& data, const QVariant& t) {

	if (data.front().canCast(QVariant::List) == true) {

		QValueList<QVariant>::const_iterator iter;

		for (iter = (data.front().toList().begin()); iter != (data.front().toList().end()); iter++) {
			Package pack;
			pack.fromMap((*iter).toMap());

			emit(packageInfo(pack));
		}
	} else {
		PackageDetails packDet;

		packDet.fromMap(data.front().toMap());
		packDet.id = temp;
		temp = "";
		emit(packageDetails(packDet));
	}
}



/********************************************************************
 *
 *                     Install Packages/Patches
 *
 ********************************************************************/

void ZmdUpdaterCore::startTransaction(QValueList<Package> installList, 
									QValueList<Package> updateList,
									QValueList<Package> removeList) {

	IS_ZMD_BUSY;
	
	for (QValueList<Package>::iterator iter = installList.begin();
		 iter != installList.end(); iter++) {
		 QMap<QString, QVariant> map;
		 map = (*iter).toMap();
		 packagesToInstall.append(map);
	}
	for (QValueList<Package>::iterator iter = updateList.begin();
		 iter != updateList.end(); iter++) {
		 QMap<QString, QVariant> map;
		 map = (*iter).toMap();
		 packagesToUpdate.append(map);
	}
	for (QValueList<Package>::iterator iter = removeList.begin();
		 iter != removeList.end(); iter++) {
		 QMap<QString, QVariant> map;
		 map = (*iter).toMap();
		 packagesToRemove.append(map);
	}

#ifdef DEBUG
	cout << "Asking for dep verification" << endl;
#endif
	server->call("zmd.packsys.verify", QValueList<QVariant>(), 
	this, SLOT(transactData(const QValueList<QVariant>&, const QVariant&)),
	this, SLOT(faultData(int, const QString&, const QVariant&)));
}

void ZmdUpdaterCore::runTransaction() {
	
	QValueList<QVariant> argList;


	cout << "Running transaction" << endl;
	
	argList.append(packagesToInstall);
	argList.append(packagesToUpdate);
	argList.append(packagesToRemove);
	argList.append(0); //Run it, no dry run

	server->call("zmd.packsys.transact", argList, 
	this, SLOT(transactData(const QValueList<QVariant>&, const QVariant&)),
	this, SLOT(faultData(int, const QString&, const QVariant&)));
	packagesToInstall.clear();
	packagesToRemove.clear();
	packagesToUpdate.clear();
}

void ZmdUpdaterCore::transactData(const QValueList<QVariant>& data, const QVariant &t) {
	static bool verification = true;
	QValueList<QVariant> argList;

	// Is the first member of the arg list a map? If so, we just got verification/dep info
	if ((data.front()).canCast(QVariant::Map) == true) {

		QMap<QString,QVariant> map;
		QValueList<QVariant> list;

		map = data.front().toMap();
		list = map["installs"].toList();
		for (QValueList<QVariant>::iterator listIter = list.begin();
			 listIter != list.end(); listIter++) {
			//In all these cases, we get a struct (QMAP) with an array explaining
			//the reason for including a package and then the package itself
			//we only want the package
			packagesToInstall.append(((*listIter).toMap())["resolveable"]);
		}
		list = map["upgrades"].toList();
		for (QValueList<QVariant>::iterator listIter = list.begin();
			 listIter != list.end(); listIter++) {

			packagesToUpdate.append(((*listIter).toMap())["resolveable"]);
			
		}
		list = map["removals"].toList();
		for (QValueList<QVariant>::iterator listIter = list.begin();
			 listIter != list.end(); listIter++) {

			packagesToRemove.append(((*listIter).toMap())["resolveable"]);
		}

		argList.append(packagesToInstall);
		argList.append(packagesToUpdate);
		argList.append(packagesToRemove);

		if (verification) { //If this is true, the info we just got is verification info 
			cout << "Asking for dep resolve" << endl;

			server->call("zmd.packsys.resolve_dependencies", argList, 
			this, SLOT(transactData(const QValueList<QVariant>&, const QVariant&)),
			this, SLOT(faultData(int, const QString&, const QVariant&))); 
			verification = false; //next time true we do the transact
		} else {

			QValueList<Package> installs;
			QValueList<Package> removals;
			QValueList<Package> updates;

			installs = mapListToPackageList(packagesToInstall);
			removals = mapListToPackageList(packagesToRemove);
			updates = mapListToPackageList(packagesToUpdate);
			emit(realPackages(installs, updates, removals));
			verification = true; //next time through we do the resolving again

		}


	} else { //or else we got two IDs for transact
		//right now are are ignoring the downloading part
		ZMD_BLOCK(data.front().toList().last().toString());
		downloadID = data.front().toList().front().toString();
		timer->start(CHECK_INTERVAL,false);
	}
}

void ZmdUpdaterCore::cancelTransaction() {
	//Currently just clear the package lists and stop the poll
	packagesToInstall.clear();
	packagesToUpdate.clear();
	packagesToRemove.clear();
	timer->stop();
	ZMD_CLEAR;
}

/********************************************************************
 *
 *                     Progress Tracking Methods/QTimer
 *
 ********************************************************************/

void ZmdUpdaterCore::timerSlot() {

	if (downloadID != "") {
		server->call("zmd.system.poll", downloadID, 
		this, SLOT(timerData(const QValueList<QVariant>&, const QVariant&)),
		this, SLOT(faultData(int, const QString&, const QVariant&))); 

	} else {
		server->call("zmd.system.poll", pollID, 
		this, SLOT(timerData(const QValueList<QVariant>&, const QVariant&)),
		this, SLOT(faultData(int, const QString&, const QVariant&)));
	}
}

void ZmdUpdaterCore::timerData(const QValueList<QVariant>& data, const QVariant &t) {

	if (data.front().canCast(QVariant::Map) == true) {
		QMap<QString, QVariant> map = data.front().toMap();
		Progress status;
		status.fromMap(map);
#ifdef DEBUG
		cout << "Status: " << status.status << endl;
		cout << "Name: " << status.name << endl;
		cout << "Percent: " << status.percent << endl;
#endif
		//test if download here NNN
		if (status.name == "Downloading Packages") {
			emit(downloadProgress(status)); 
			if (map["percent"].toDouble() > 99) {
				downloadID = "";
			}
		} else {
			emit(progress(status));
			if (downloadID.isEmpty() && map["status"].toInt() > 1) {
				ZMD_CLEAR;
				timer->stop();
				
				if (temp != "") {
					if (map["status"].toInt() == 4)
						emit(serviceAdded(temp, ERROR_INVALID));
					else 
						emit(serviceAdded(temp, ERROR_NONE));

					temp = "";
				} else {
					emit(transactionFinished(ERROR_NONE));
				}

			}
		}
	}

}

/********************************************************************
 *
 *                     Fault Handling
 *
 ********************************************************************/

void ZmdUpdaterCore::faultData(int code, const QString& message, const QVariant&t) {
	switch (code) {

		case 0:
			//Service already exists -- this was experimentally found, might change
			//Thread fault
			if (pollID != "") {
				emit(serviceAdded(temp, ERROR_INVALID));
			} else {
				//this usually happens during a service removal
				emit(generalFault(message));
			}
			temp = "";
			break;
		case -603:
			//Dep Failure
			emit(transactionFinished(ERROR_DEP_FAIL));
			break;
		case -605:
			//Invalid package specified
			emit(transactionFinished(ERROR_INVALID));
			break;
		case -606:
			//Invalid catalog specified
			break;
		case -607:
			//Invalid progress ID
			emit(transactionFinished(ERROR_INVALID));
			break;
		case -617:
			//Invalid service specified
			emit(serviceAdded(temp, ERROR_INVALID));
			temp = "";
			break;
		case -619: 
			//Invalid service type specified
			emit(serviceAdded(temp, ERROR_INVALID_TYPE));
			temp = "";
			break;
		default:
			//Things we do not handle
			cout << "ERROR: Something really strange just happened -- " << message << endl;
			emit(generalFault(message));
			break;
	}
	ZMD_CLEAR;
	timer->stop();
#ifdef DEBUG
	cout << "Fault: " << message << endl;
	cout << "Fault Code: " << code << endl;
#endif
}
