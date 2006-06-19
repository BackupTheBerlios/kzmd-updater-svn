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

	cout << server->url().url() << endl;
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
			cout << cat.name << endl;
		}
		emit(catalogListing(catalogList));
	}
}

void ZmdUpdaterCore::subscribeCatalog(Catalog cat) {
	IS_ZMD_BUSY;
}

void ZmdUpdaterCore::unsubscribeCatalog(Catalog cat) {
	IS_ZMD_BUSY;
}

/* Package Handling (call and data slot) */
void ZmdUpdaterCore::getPatches(Catalog cat) {
	IS_ZMD_BUSY;
}

void ZmdUpdaterCore::getUpdates(Catalog cat) {
	IS_ZMD_BUSY;
}

/* Transaction call, data slot, timer */

void ZmdUpdaterCore::runTransaction(QValueList<Patch> patchList, QValueList<Package> packageList) {
	IS_ZMD_BUSY;
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
		emit(progress(status));
		if (map["status"].toInt() == 2)
			ZMD_CLEAR;
	}
}

void ZmdUpdaterCore::faultData(int code, const QString& message, const QVariant&t) {
	cout << "Fault: " << message << endl;
}
