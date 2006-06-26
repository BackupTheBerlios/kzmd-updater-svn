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

#ifndef _ZMD_UPDATER_CORE_UTILS_H_
#define _ZMD_UPDATER_CORE_UTILS_H_

#include <qmap.h>

class Progress {

	public:

		Progress() : expectedTime(-1), remainingTime(-1), status(-1), percent(-1.0L) {}

		Progress& operator=(const QMap<QString,QVariant> &map) {
			QValueList<QVariant>::const_iterator iter;

			name = map["name"].toString();
			status = map["status"].toInt();
			expectedTime = map["expected_time"].toInt();
			remainingTime = map["remaining_time"].toInt();
			percent = map["percent"].toDouble();

			for (iter = map["messages"].toList().begin(); iter != map["messages"].toList().end();
				 iter++) {
				messages.append((*iter).toString());
			}
			return *this;
		}

		QString name;
		int status;
		int expectedTime;
		int remainingTime;
		double percent;	
		QValueList<QString> messages;
};

class Service {

	public:

		Service() : activated(3) {}


		Service& operator=(const QMap<QString,QVariant> &map) {
			name = map["name"].toString();
			id = map["id"].toString();
			uri = map["uri"].toString();
			type = map["type"].toString();
			activated = map["active"].toInt();
			return *this;
		}

		QMap<QString, QVariant> toMap() {
			QMap<QString,QVariant> map;
			if (name != "")
				map["name"] = name;
			if (id != "")
				map["id"] = id;
			if (uri != "")
				map["uri"] = uri;
			if (type != "")
				map["type"] = type;
			if (activated != 3)
				map["active"] = activated;
			return map;
		}

		QString name;
		QString id;
		QString uri;
		QString type;
		bool activated;
	
};

class Catalog {

	public:

		Catalog() : subscribed(3) {}

		Catalog& operator=(const QMap<QString,QVariant> &map) {
			name = map["name"].toString();
			id = map["id"].toString();
			displayName = map["display_name"].toString();
			subscribed = map["subscribed"].toBool();
			service = map["service"].toString();
			return *this;
		}

		QMap<QString, QVariant> toMap() {
			QMap<QString,QVariant> map;
			if (name != "")
				map["name"] = name;
			if (id != "")
				map["id"] = id;
			if (displayName != "")
				map["display_name"] = displayName;
			if (subscribed != 3)
				map["subscribed"] = subscribed;
			return map;
		}

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

		Package() : installed(3) {}

		Package& operator=(const QMap<QString,QVariant> &map) {
			name = map["name"].toString();
			id = map["id"].toString();
			version = map["version"].toString();
			catalog = map["catalog"].toString();
			description = map["summary"].toString();
			installed = map["installed"].toBool();
			return *this;
		}

		QMap<QString, QVariant> toMap() {
			QMap<QString,QVariant> map;
			if (name != "")
				map["name"] = name;
			if (id != "")
				map["id"] = id;
			if (version != "")
				map["version"] = version;
			if (catalog != "")
				map["catalog"] = catalog;
			if (description != "")
				map["summary"] = description;
			if (installed != 3)
				map["installed"] = installed;
			return map;
		}


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

		Package& operator=(const QMap<QString,QVariant> &map) {
			name = map["name"].toString();
			id = map["id"].toString();
			version = map["version"].toString();
			catalog = map["catalog"].toString();
			description = map["summary"].toString();
			installed = map["installed"].toBool();
			category = map["category"].toString();
			return *this;
		}

		QString category;
};


#endif
