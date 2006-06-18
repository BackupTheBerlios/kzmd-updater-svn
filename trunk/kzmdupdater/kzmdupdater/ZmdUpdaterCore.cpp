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

ZmdUpdaterCore::ZmdUpdaterCore(QObject *parent) : UpdaterCore(parent) {
	server = new KXMLRPC::Server(KURL(SERVER_ADDY), this);
}

ZmdUpdaterCore::~ZmdUpdaterCore() {
}

void ZmdUpdaterCore::setUser(QString user) {
	KURL url(SERVER_ADDY);
	username = user;

	url.setUser(user);
	server->setUrl(url);
}

void ZmdUpdaterCore::setPass(QString pass) {
	KURL url(SERVER_ADDY);
	password = pass;

	url.setPass(pass);
	server->setUrl(url);
}

/* Service Handling (call and data slot) */

void ZmdUpdaterCore::getServices() {
	server->call("zmd.system.service_list", QValueList<QVariant>(),
	this, SLOT(serviceData(const QValueList<QVariant>&, const QVariant&)), 
	this, SLOT(faultData(int, const QString&, const QVariant&)));
}

void ZmdUpdaterCore::addService(Service serv) {

}

void ZmdUpdaterCore::removeService(Service serv) {
}

void ZmdUpdaterCore::serviceData(const QValueList<QVariant>& data, const QVariant& t) {
	if (data.front().canCast(QVariant::String) == true) {
		//Handle add and remove service
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
	server->call("zmd.system.catalog_list", QValueList<QVariant>(), 
	this, SLOT(catalogData(const QValueList<QVariant>&, const QVariant&)),
	this, SLOT(faultData(int, const QString&, const QVariant&)));
}

void ZmdUpdaterCore::catalogData(const QValueList<QVariant>& data, const QVariant& t) {

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
	}
	emit(catalogListing(catalogList));
}

void ZmdUpdaterCore::subscribeCatalog(Catalog cat) {
}

void ZmdUpdaterCore::unsubscribeCatalog(Catalog cat) {
}

void ZmdUpdaterCore::getPatches(Catalog cat) {
}

void ZmdUpdaterCore::getUpdates(Catalog cat) {
}

void ZmdUpdaterCore::runTransaction(QValueList<Patch>* patchList, QValueList<Package>* packageList) {
}

void ZmdUpdaterCore::faultData(int code, const QString& t, const QVariant& m) {
}
