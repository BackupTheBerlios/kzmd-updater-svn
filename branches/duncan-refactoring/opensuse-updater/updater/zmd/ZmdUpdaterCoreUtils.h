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


class Identity
{

public:

  Identity()
  {}

  void fromMap(const QMap<QString, QVariant> &map)
  {

    name = map["name"].toString();
    pass = map["password"].toString();
    privs = map["privs"].toString();
  }

  QMap<QString, QVariant> toMap()
  {

    QMap<QString, QVariant> map;

    if (name != "")
      map["name"] = name;
    if (pass != "")
      map["password"] = pass;
    if (privs != "")
      map["privs"] = privs;

    return map;
  }

  QString name;
  QString pass;
  QString privs;
};


class Progress
{

public:

  Progress() : status(-1), expectedTime(-1), remainingTime(-1), percent(-1.0L)
  {}

  void fromMap(const QMap<QString,QVariant> &map)
  {
    QValueList<QVariant>::const_iterator iter;

    name = map["name"].toString();
    status = map["status"].toInt();
    expectedTime = map["expected_time"].toInt();
    remainingTime = map["remaining_time"].toInt();
    percent = map["percent"].toDouble();

    for (iter = map["messages"].toList().begin(); iter != map["messages"].toList().end();
         iter++)
    {
      messages.append((*iter).toString());
    }
  }

  QString name;
  int status;
  int expectedTime;
  int remainingTime;
  double percent;
  QValueList<QString> messages;
};

class Service
{

public:

  Service() : activated(3)
  {}

  void fromMap(const QMap<QString,QVariant> &map)
  {
    name = map["name"].toString();
    id = map["id"].toString();
    uri = map["uri"].toString();
    type = map["type"].toString();
    activated = map["active"].toInt();
  }

  QMap<QString, QVariant> toMap()
  {
    QMap<QString,QVariant> map;
    if (name != "")
      map["name"] = name;
    if (id != "")
      map["id"] = id;
    if (uri != "")
      map["uri"] = uri;
    if (type != "")
      map["type"] = type;
    return map;
  }

  QString name;
  QString id;
  QString uri;
  QString type;
  bool activated;

};

class Catalog
{

public:

  Catalog() : subscribed(3)
  {}

  void fromMap(const QMap<QString,QVariant> &map)
  {
    name = map["name"].toString();
    id = map["id"].toString();
    displayName = map["display_name"].toString();
    subscribed = map["subscribed"].toBool();
    service = map["service"].toString();
  }

  QMap<QString, QVariant> toMap()
  {
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

class PackageDetails
{

public:

  PackageDetails() : installOnly(0), installSize(0)
  {}

  void fromMap(const QMap<QString, QVariant> &map)
  {
    summary = map["summary"].toString();
    description = map["description"].toString();
    vendor = map["vendor"].toString();
    installOnly = map["install_only"].toBool();
    installSize = map["install_size"].toInt();
  }

  QString id;
  QString summary;
  QString description;
  QString vendor;
  bool installOnly;
  int installSize;
};

class PatchDetails
{

public:

  PatchDetails() : creationTime(0),
      rebootRequired(false),
      restartRequired(false),
      interactive(false)
  {}

  void fromMap(const QMap<QString, QVariant> &map)
  {
    creationTime = map["creation_time"].toInt();
    rebootRequired = map["reboot_req"].toBool();
    restartRequired = map["restart_req"].toBool();
    interactive = map["interactive"].toBool();
  }

  QString id;
  int creationTime;
  bool rebootRequired;
  bool restartRequired;
  bool interactive;
};

/*

	This is the "base" class. It can represent either a package
	or a patch for the backend
*/
class Package
{

public:

  Package() : installed(3)
  {}

  void fromMap(const QMap<QString,QVariant> &map)
  {
    int rawType = 0;

    rawType = map["type"].toInt();
    switch (rawType)
    {
    case 0:
      type = "package";
      break;
    case 1:
      type = "script";
      break;
    case 2:
      type = "message";
      break;
    case 3:
      type = "patch";
      break;
    case 4:
      type = "pattern";
      break;
    case 5:
      type = "product";
      break;
    }

    name = map["name"].toString();
    id = map["id"].toString();
    version = map["version"].toString();
    catalog = map["catalog"].toString();
    description = map["summary"].toString();
    installed = map["installed"].toBool();
  }

  QMap<QString, QVariant> toMap()
  {
    QMap<QString,QVariant> map;

    if (type == "patch")
    {
      map["type"] = (int)3;
    }
    else if (type == "package")
    {
      map["type"] = (int)0;
    }
    else
    {
      map["type"] = (int)0;
    }

    if (name != "")
      map["name"] = name;
    if (id != "")
      map["id"] = id.toInt();
    /*
    			if (version != "")
    				map["version"] = version;
    */

    if (catalog != "")
      map["catalog"] = catalog;

    return map;
  }

  QString type;
  QString name;
  QString id;
  QString version;
  QString catalog;
  QString description;
  bool installed;

  //The details
  PackageDetails details;
};

class Patch : public Package
{

public:

  Patch() : Package()
  {}

  void fromMap(const QMap<QString,QVariant> &map)
  {
    name = map["name"].toString();
    type = "patch";
    id = map["id"].toString();
    version = map["version"].toString();
    catalog = map["catalog"].toString();
    description = map["summary"].toString();
    installed = map["installed"].toBool();
    category = map["category"].toString();
    status = map["status"].toInt();
    rebootRequired = map["reboot_req"].toBool();
    restartRequired = map["restart_req"].toBool();
  }

  QString category;
  int status;
  bool rebootRequired;
  bool restartRequired;

  //The details
  PatchDetails details;

  //The deps
  QValueList<Package> patchDeps;
};

class PackageLock
{

public:

  PackageLock()
  {}

  void fromMap(const QMap<QString, QVariant> &map)
  {
    id = map["id"].toString();
    catalog = map["catalog"].toString();
    glob = map["glob"].toString();

    if (map.contains("dependency") == true)
    {
      QMap<QString, QVariant> depMap;

      depMap = map["dependency"].toMap();
      pack.fromMap(depMap);
    }
  }

  QMap<QString, QVariant> toMap()
  {
    QMap<QString, QVariant> map;

    if (id != "")
      map["id"] = id;
    if (catalog != "")
      map["catalog"] = catalog;
    if (glob != "")
      map["glob"] = glob;
    if (pack.id != "")
      map["dependency"] = pack.toMap();
    return map;
  }

  QString id;
  QString catalog;
  QString glob;
  Package pack;
};

#endif

