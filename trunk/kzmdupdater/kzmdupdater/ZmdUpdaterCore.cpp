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

ZmdUpdaterCore::ZmdUpdaterCore(QObject *parent) : UpdaterCore(parent) {
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

/* Service Handling (call and data slot) */

void ZmdUpdaterCore::getServices() {
	IS_ZMD_BUSY;

	server->call("zmd.system.service_list", QValueList<QVariant>(),
	this, SLOT(serviceData(const QValueList<QVariant>&, const QVariant&)), 
	this, SLOT(faultData(int, const QString&, const QVariant&)));
}

void ZmdUpdaterCore::addService(Service serv) {
	IS_ZMD_BUSY;

	QValueList<QVariant> data;
	QMap<QString, QVariant> map;
	map["name"] = QVariant(serv.name);
	map["type"] = QVariant(serv.type);
	map["uri"] = QVariant(serv.uri);
	data.append(QVariant(map));

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

void ZmdUpdaterCore::serviceData(const QValueList<QVariant>& data, const QVariant& t) {
	if (data.front().canCast(QVariant::String) == true) {
		ZMD_BLOCK(data.front().toString());
		timer->start(CHECK_INTERVAL,false);
	} else if (data.front().canCast(QVariant::List) == true) {
		QValueList<QVariant> list;
		list = (data.front().toList());
		QValueList<QVariant>::iterator iter;
		QValueList<Service> serviceList;

		for (iter = list.begin(); iter != list.end(); iter++) {
			QMap<QString, QVariant> map = (*iter).toMap();
			Service serv;
			serv.name = map["name"].toString();
			serv.id = map["id"].toString();
			serv.uri = map["uri"].toString();
			serv.type = map["type"].toString();
			serv.activated = map["active"].toString();
			serviceList.append(serv);
#ifdef DEBUG
			cout << "Service drop: " << serv.name << endl;
#endif
		}
		emit(serviceListing(serviceList));
	}
}

/* Catalog Handling (call and data slot) */

void ZmdUpdaterCore::getCatalogs() {
	IS_ZMD_BUSY;

	server->call("zmd.system.catalog_list", QValueList<QVariant>(), 
	this, SLOT(catalogData(const QValueList<QVariant>&, const QVariant&)),
	this, SLOT(faultData(int, const QString&, const QVariant&)));
}

void ZmdUpdaterCore::catalogData(const QValueList<QVariant>& data, const QVariant& t) {

	if (data.front().canCast(QVariant::String) == true) {
		ZMD_BLOCK(data.front().toString());
		timer->start(CHECK_INTERVAL, false);
	} else if (data.front().canCast(QVariant::List) == true) {

		QValueList<QVariant> list;
		list = (data.front().toList());
		QValueList<QVariant>::iterator iter;
		QValueList<Catalog> catalogList;

		for (iter = list.begin(); iter != list.end(); iter++) {
			QMap<QString, QVariant> map = (*iter).toMap();
			Catalog cat;
			cat.id = map["id"].toString();
			cat.name = map["name"].toString();
			cat.displayName = map["display_name"].toString();
			cat.service = map["service"].toString();
			cat.subscribed = map["subscribed"].toBool();
			catalogList.append(cat);
#ifdef DEBUG
			cout << "Catalog name: " << cat.name << endl;
			cout << "Catalot service: " << cat.service << endl;
#endif
		}
		emit(catalogListing(catalogList));
	}
}

void ZmdUpdaterCore::subscribeCatalog(Catalog cat) {
	IS_ZMD_BUSY;

	QValueList<QVariant> argList;
	argList.append(cat.id);
	argList.append(true);

	server->call("zmd.packsys.catalog_subscribe", argList, 
	this, SLOT(catalogData(const QValueList<QVariant>&, const QVariant&)),
	this, SLOT(faultData(int, const QString&, const QVariant&)));


}

void ZmdUpdaterCore::unsubscribeCatalog(Catalog cat) {
	IS_ZMD_BUSY;
	QValueList<QVariant> argList;
	argList.append(cat.id);
	argList.append(false);

	server->call("zmd.packsys.catalog_subscribe", argList, 
	this, SLOT(catalogData(const QValueList<QVariant>&, const QVariant&)),
	this, SLOT(faultData(int, const QString&, const QVariant&)));

}

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
	QValueList<QVariant> list;
	list = (data.front().toList());
	QValueList<QVariant>::iterator iter;
	QValueList<Package> packageList;

	for (iter = list.begin(); iter != list.end(); iter++) {
		QMap<QString, QVariant> map = (*iter).toMap();
		Package pack;
		pack.id = map["id"].toString();
		pack.name = map["name"].toString();
		pack.version = map["version"].toString();
		pack.catalog = map["catalog"].toString();
		pack.description = map["summary"].toString();
		pack.installed = map["installed"].toBool();
		packageList.append(pack);
#ifdef DEBUG
		cout << "Package drop: " << pack.name << endl;
#endif
	}
	emit(updateListing(packageList));
}

void ZmdUpdaterCore::patchData(const QValueList<QVariant>& data, const QVariant& t) {
	QValueList<QVariant> list;
	list = (data.front().toList());
	QValueList<QVariant>::iterator iter;
	QValueList<Patch> patchList;

	for (iter = list.begin(); iter != list.end(); iter++) {
		QMap<QString, QVariant> map = (*iter).toMap();
		Patch patch;
		patch.id = map["id"].toString();
		patch.name = map["name"].toString();
		patch.version = map["version"].toString();
		patch.catalog = map["catalog"].toString();
		patch.description = map["summary"].toString();
		patch.installed = map["installed"].toBool();
		patchList.append(patch);
#ifdef DEBUG
		cout << "Patch drop: " << patch.name << endl;
#endif
	}
	emit(patchListing(patchList));

}

/* Transaction call, data slot, timer */

void ZmdUpdaterCore::runTransaction(QValueList<Package> installList, 
									QValueList<Package> updateList,
									QValueList<Package> removeList) {

	IS_ZMD_BUSY;
	
	QValueList<QVariant> argList;
	QValueList<QVariant> installs;	
	QValueList<QVariant> updates;	
	QValueList<QVariant> removals;

	for (QValueList<Package>::iterator iter = installList.begin();
		 iter != updateList.end(); iter++) {
		 QMap<QString, QVariant> map;
		 map["id"] = (*iter).id;
		 map["name"] = (*iter).name;
		 installs.append(map);
	}
	for (QValueList<Package>::iterator iter = updateList.begin();
		 iter != updateList.end(); iter++) {
		 QMap<QString, QVariant> map;
		 map["id"] = (*iter).id;
		 map["name"] = (*iter).name;
		 updates.append(map);
	}
	for (QValueList<Package>::iterator iter = removeList.begin();
		 iter != updateList.end(); iter++) {
		 QMap<QString, QVariant> map;
		 map["id"] = (*iter).id;
		 map["name"] = (*iter).name;
		 removals.append(map);
	}
	argList.append(installs);
	argList.append(updates);	
	argList.append(removals);

#ifdef DEBUG
	cout << "Asking for dep resolve" << endl;
#endif
	server->call("zmd.packsys.resolve_dependencies", argList, 
	this, SLOT(transactData(const QValueList<QVariant>&, const QVariant&)),
	this, SLOT(faultData(int, const QString&, const QVariant&)));
}

void ZmdUpdaterCore::transactData(const QValueList<QVariant>& data, const QVariant &t) {

	// Is the first member of the arg list a list? If so, we just got dep info
	if ((data.front()).canCast(QVariant::List) == true) {
		QValueList<QVariant> installList;
		QValueList<QVariant> updateList;
		QValueList<QVariant> removeList;
		QMap<QString, QVariant> map;

		QValueList<QVariant>::const_iterator iter = data.begin();
		installList = (*iter).toList().front().toList();
		iter++;
		updateList = (*iter).toList().front().toList();
		iter++;
		removeList = (*iter).toList().front().toList();

#ifdef DEBUG
		for (iter = updateList.begin(); iter != updateList.end(); iter++) {
			map = (*iter).toMap();
			cout << "Updated package: " << map["name"].toString() << endl;
		}
		cout << "Starting transaction" << endl;
#endif

		server->call("zmd.packsys.transact", data, 
		this, SLOT(transactData(const QValueList<QVariant>&, const QVariant&)),
		this, SLOT(faultData(int, const QString&, const QVariant&)));


	} else { //or else we got two IDs for transact
		//right now are are ignoring the downloading part
		ZMD_BLOCK(data.last().toString());
		timer->start(CHECK_INTERVAL,false);
	}
}

void ZmdUpdaterCore::cancelTransaction() {

}


/* Global timer slot and data slot -- this keeps track of poll events */
void ZmdUpdaterCore::timerSlot() {
	server->call("zmd.system.poll", pollID, 
	this, SLOT(timerData(const QValueList<QVariant>&, const QVariant&)),
	this, SLOT(faultData(int, const QString&, const QVariant&)));
}

void ZmdUpdaterCore::timerData(const QValueList<QVariant>& data, const QVariant &t) {

	if (data.front().canCast(QVariant::List) == true) {
		QMap<QString, QVariant> map = data.front().toList().front().toMap();
		Progress status;
		status.percent = map["percent"].toDouble();
		status.expectedTime = map["expected_time"].toInt();
		status.remainingTime = map["remaning_time"].toInt();
		status.name = map["name"].toString();
#ifdef DEBUG
		cout << "Timer data drop: " << status.percent << endl;
		cout << "Name: " << status.name << endl;
#endif
		emit(progress(status));
		if (map["status"].toInt() == 2) {
			ZMD_CLEAR;
			timer->stop();
		}
	}
}

void ZmdUpdaterCore::faultData(int code, const QString& message, const QVariant&t) {
#ifdef DEBUG
	cout << "Fault: " << message << endl;
	cout << "Fault Code: " << t.toString() << endl;
#endif
}
