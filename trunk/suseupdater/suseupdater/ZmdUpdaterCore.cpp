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

#include <kdebug.h>

#include "ZmdUpdaterCore.h"
#include "Constants.h"


/********************************************************************
 *
 *                     Init/User/Pass Methods
 *
 ********************************************************************/

ZmdUpdaterCore::ZmdUpdaterCore(QObject *parent) : QObject(parent) {
  server = new KXmlRpcServer(KURL());
  server->setUserAgent("ZMDUPDATER/0.1");

  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(timerSlot()));
  
  timeoutCount = 0;
}

ZmdUpdaterCore::~ZmdUpdaterCore() {
  delete server;
}

/********************************************************************
 *
 *                    User/Pass Functions 
 *
 ********************************************************************/

void ZmdUpdaterCore::setUser(QString user) {
  KURL url(server->url());
  username = user; //we never actually use this

  url.setUser(user);
  server->setUrl(url);
}

void ZmdUpdaterCore::setPass(QString pass) {
  KURL url(server->url());
  password = pass; //we never actually use this

  url.setPass(pass);
  server->setUrl(url);
}

void ZmdUpdaterCore::addUser(Identity id) {
  IS_ZMD_BUSY;

  QValueList<QVariant> data;
  QMap<QString,QVariant> map = id.toMap();
  data.append(QVariant(map));

  server->call("zmd.system.identity_add", data,
  this, SLOT(identityData(const QValueList<QVariant>&, const QVariant&)), 
  this, SLOT(faultData(int, const QString&, const QVariant&)));
}

void ZmdUpdaterCore::removeUser(QString user) {
  IS_ZMD_BUSY;

  server->call("zmd.system.identity_remove", user,
  this, SLOT(identityData(const QValueList<QVariant>&, const QVariant&)), 
  this, SLOT(faultData(int, const QString&, const QVariant&)));
}

void ZmdUpdaterCore::modifyUser(Identity id) {
  IS_ZMD_BUSY;

  QValueList<QVariant> data;
  QMap<QString,QVariant> map = id.toMap();
  data.append(QVariant(map));

  server->call("zmd.system.identity_modify", data,
  this, SLOT(identityData(const QValueList<QVariant>&, const QVariant&)), 
  this, SLOT(faultData(int, const QString&, const QVariant&)));
}

void ZmdUpdaterCore::listUsers() {
  IS_ZMD_BUSY;

  server->call("zmd.system.identity_list", QValueList<QVariant>(),
  this, SLOT(identityData(const QValueList<QVariant>&, const QVariant&)), 
  this, SLOT(faultData(int, const QString&, const QVariant&)));
}

void ZmdUpdaterCore::identityData(const QValueList<QVariant>& data, const QVariant& t) {

  if (data.front().canCast(QVariant::List) == true) {
    //We got a list back, is identity list
    QValueList<QVariant> list;
    list = (data.front().toList());
    QValueList<QVariant>::iterator iter;
    QValueList<Identity> identityList;

    for (iter = list.begin(); iter != list.end(); iter++) {
      QMap<QString, QVariant> map = (*iter).toMap();
      Identity id;
      id.fromMap(map);
      identityList.append(id);
    }
    emit(userListing(identityList));
  } 

}

/********************************************************************
 *
 *                    Server Settings 
 *
 ********************************************************************/

void ZmdUpdaterCore::setServer(KURL url) {

  QString user;
  QString pass;
  KURL serverUrl;

  serverUrl = server->url();
  user = serverUrl.user();
  pass = serverUrl.pass();
  serverUrl = url;
  serverUrl.setUser(user);
  serverUrl.setPass(pass);

  server->setUrl(serverUrl);
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
    }
    emit(serviceListing(serviceList));
  } else if (data.front().canCast(QVariant::String) == true) {
    if (data.front().toString() == "") {
#ifdef DEBUG
      kdWarning() << "Service Removed" << endl;
#endif
      emit(serviceRemoved());
    }
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
    */
#ifdef DEBUG
    kdWarning() << "Catalog Sub Changed" << endl;
#endif
    emit(catalogSubscriptionChanged());
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
      kdWarning() << "Catalog Drop name: " << cat.name << endl;
      kdWarning() << "Catalog service: " << cat.service << endl;
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
    kdWarning() << "Patch drop: " << patch.name << endl;
    kdWarning() << "Patch ID: " << patch.id << endl;
#endif
  }
  emit(patchListing(patchList));

}
/********************************************************************
 *
 *                     Get Package/Patch Info/Details
 *
 ********************************************************************/

void ZmdUpdaterCore::getPackageInfo(QString packageName) {

  QValueList<QVariant> wrapper;
  QValueList<QVariant> args;

  args.append("Name");
  args.append("is");
  args.append(packageName);

  wrapper.append(args);

  server->call("zmd.packsys.query", wrapper, 
  this, SLOT(infoPackageData(const QValueList<QVariant>&, const QVariant&)),
  this, SLOT(faultData(int, const QString&, const QVariant&)));
}

void ZmdUpdaterCore::getPatchInfo(QString patchName) {

  QValueList<QVariant> wrapper;
  QValueList<QVariant> args;

  args.append("Name");
  args.append("is");
  args.append(patchName);

  wrapper.append(args);

  server->call("zmd.packsys.query_patches", wrapper, 
  this, SLOT(infoPatchData(const QValueList<QVariant>&, const QVariant&)),
  this, SLOT(faultData(int, const QString&, const QVariant&)));
}

void ZmdUpdaterCore::getPackageDetails(Package pack) {
  QMap<QString, QVariant> map;
  QValueList<QVariant> args;

  map = pack.toMap();
  args.append(map);

  //We need an id in packageDetails, but it does not return it
  temp = pack.id;

  server->call("zmd.packsys.package_details", args, 
  this, SLOT(infoPackageData(const QValueList<QVariant>&, const QVariant&)),
  this, SLOT(faultData(int, const QString&, const QVariant&)));

}

void ZmdUpdaterCore::getPatchDetails(Patch patch) {
  QMap<QString, QVariant> map;
  QValueList<QVariant> args;

  map = patch.toMap();
  args.append(map);

  //We need an id in patchDetails, but it does not return it
  temp = patch.id;

  server->call("zmd.packsys.patch_details", args, 
  this, SLOT(infoPatchData(const QValueList<QVariant>&, const QVariant&)),
  this, SLOT(faultData(int, const QString&, const QVariant&)));

}

void ZmdUpdaterCore::infoPackageData(const QValueList<QVariant>& data, const QVariant& t) {

  //We either get a List or a Map in this return. If its a list, then we just called getInfo
  if (data.front().canCast(QVariant::List) == true) {

    QValueList<QVariant>::const_iterator iter;
    for (iter = (data.front().toList().begin()); iter != (data.front().toList().end()); iter++) {
      Package pack;
      pack.fromMap((*iter).toMap());

      emit(packageInfo(pack));
    }
  } else {
  //And if its a map, we just called getDetails
    PackageDetails packDet;

    packDet.fromMap(data.front().toMap());
    packDet.id = temp;
    temp = "";
    emit(packageDetails(packDet));
  }
}

void ZmdUpdaterCore::infoPatchData(const QValueList<QVariant>& data, const QVariant& t) {

  //We either get a List or a Map in this return. If its a list, then we just called getInfo
  if (data.front().canCast(QVariant::List) == true) {

    QValueList<QVariant>::const_iterator iter;
    for (iter = (data.front().toList().begin()); iter != (data.front().toList().end()); iter++) {
      Patch patch;
      patch.fromMap((*iter).toMap());

      emit(patchInfo(patch));
    }
  } else {
  //And if its a map, we just called getDetails
    PatchDetails patchDet;

    patchDet.fromMap(data.front().toMap());
    patchDet.id = temp;
    temp = "";
    emit(patchDetails(patchDet));
  }
}

/*******************************************************************
 *
 *            Add/Remove/List Locks
 *
 ******************************************************************/

void ZmdUpdaterCore::lockPackage(PackageLock lock) {
  IS_ZMD_BUSY;

  QMap<QString, QVariant> map;
  QValueList<QVariant> wrapper;
  
  map = lock.toMap();

#ifdef DEBUG
  kdWarning() << "Lock info we are sending: " << endl;
  kdWarning() << (map["dependency"].toMap())["id"].toString() << endl;
  kdWarning() << map["catalog"].toString() << endl;
#endif

  wrapper.append(map);

  server->call("zmd.packsys.add_lock", wrapper, 
  this, SLOT(lockData(const QValueList<QVariant>&, const QVariant&)),
  this, SLOT(faultData(int, const QString&, const QVariant&)));
}

void ZmdUpdaterCore::unlockPackage(PackageLock lock) {
  IS_ZMD_BUSY;

  QValueList<QVariant> wrapper;

  if (lock.id == "") 
    return;

  wrapper.append(lock.id);

  server->call("zmd.packsys.remove_lock", wrapper, 
  this, SLOT(lockData(const QValueList<QVariant>&, const QVariant&)),
  this, SLOT(faultData(int, const QString&, const QVariant&)));
}

void ZmdUpdaterCore::getLocks() {
  IS_ZMD_BUSY;

  server->call("zmd.packsys.get_locks", QValueList<QVariant>(), 
  this, SLOT(lockData(const QValueList<QVariant>&, const QVariant&)),
  this, SLOT(faultData(int, const QString&, const QVariant&)));

}

void ZmdUpdaterCore::lockData(const QValueList<QVariant>& data, const QVariant &t) {

  if (data.front().canCast(QVariant::List) == true) {
    QMap<QString, QVariant> map;
    QValueList<QVariant> list;
    QValueList<PackageLock> lockList;

#ifdef DEBUG
    kdWarning() << "got a list of locks" << endl;
#endif
    list = data.front().toList();
    for (QValueList<QVariant>::iterator iter = list.begin(); iter != list.end(); iter++) {
      PackageLock lock;

      map = (*iter).toMap();
      lock.fromMap(map);
      
#ifdef DEBUG
      kdWarning() << "Lock info: " << endl;
      kdWarning() << lock.id << endl;
      kdWarning() << lock.pack.name << endl;
#endif

      lockList.append(lock);
    }
    emit(lockListing(lockList));
  } else {
    kdWarning() << "Got something in the lock data function that was not a list" << endl;
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

  //clear our lists first, if something went wrong last time we may have strays
  packagesToInstall.clear();
  packagesToUpdate.clear();
  packagesToRemove.clear();
  
  for (QValueList<Package>::iterator iter = installList.begin();
     iter != installList.end(); iter++) {
     QMap<QString, QVariant> map;
     map = (*iter).toMap();
     packagesToInstall[map["id"].toString()] = QVariant(map);
  }
  for (QValueList<Package>::iterator iter = updateList.begin();
     iter != updateList.end(); iter++) {
     QMap<QString, QVariant> map;
     map = (*iter).toMap();
     packagesToUpdate[map["id"].toString()] = QVariant(map);
     //packagesToUpdate.append(QVariant(map));
  }
  for (QValueList<Package>::iterator iter = removeList.begin();
     iter != removeList.end(); iter++) {
     QMap<QString, QVariant> map;
     map = (*iter).toMap();
     packagesToRemove[map["id"].toString()] = QVariant(map);
     //packagesToRemove.append(QVariant(map));
  }

#ifdef DEBUG
  kdWarning() << "Asking for dep verification" << endl;
#endif
  server->call("zmd.packsys.verify", QValueList<QVariant>(), 
  this, SLOT(transactData(const QValueList<QVariant>&, const QVariant&)),
  this, SLOT(faultData(int, const QString&, const QVariant&)));
}

void ZmdUpdaterCore::runTransaction() {
  
  QValueList<QVariant> argList;

  argList.append(packagesToInstall.values());
  argList.append(packagesToUpdate.values());
  argList.append(packagesToRemove.values());
  argList.append(0); //Run it, no dry run

  server->call("zmd.packsys.transact", argList, 
  this, SLOT(transactData(const QValueList<QVariant>&, const QVariant&)),
  this, SLOT(faultData(int, const QString&, const QVariant&)));

  //Don't need these anymore
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
    QValueList<QVariant>::iterator listIter;
    map = data.front().toMap();
    list = map["installs"].toList();

    /***********************************************************************
      In all these cases, we get a struct (QMAP) with an array explaining
      the reason for including a package and then the package itself
      we only want the package
    ***********************************************************************/
    for ( listIter = list.begin(); listIter != list.end(); listIter++) {
      QMap<QString, QVariant> tempMap = (*listIter).toMap()["resolvable"].toMap();
      QString packageId = tempMap["id"].toString();

      if (packagesToInstall.find(packageId) == packagesToInstall.end()) {
        tempMap.erase("catalog"); //Catalog is often invalid XML
        packagesToInstall[packageId] = QVariant(tempMap);
      }
    }
    list = map["upgrades"].toList();
    for (listIter = list.begin(); listIter != list.end(); listIter++) {
      QMap<QString, QVariant> tempMap = (*listIter).toMap()["resolvable"].toMap();
      QString packageId = tempMap["id"].toString();

      if (packagesToUpdate.find(packageId) == packagesToUpdate.end()) {
        tempMap.erase("catalog"); //Catalog is often invalid XML
        packagesToUpdate[packageId] = QVariant(tempMap);
      }
    }
    list = map["removals"].toList();
    for (listIter = list.begin(); listIter != list.end(); listIter++) {
      QMap<QString, QVariant> tempMap = (*listIter).toMap()["resolvable"].toMap();
      QString packageId = tempMap["id"].toString();

      if (packagesToRemove.find(packageId) == packagesToRemove.end()) {
        tempMap.erase("catalog"); //Catalog is often invalid XML
        packagesToRemove[packageId] = QVariant(tempMap);
      }

    }

    argList.append(packagesToInstall.values());
    argList.append(packagesToUpdate.values());
    argList.append(packagesToRemove.values());

    if (verification) { //If this is true, the info we just got is verification info 
      server->call("zmd.packsys.resolve_dependencies", argList, 
      this, SLOT(transactData(const QValueList<QVariant>&, const QVariant&)),
      this, SLOT(faultData(int, const QString&, const QVariant&))); 
      verification = false; //next time through we do the transact

    } else {

      QValueList<Package> installs;
      QValueList<Package> removals;
      QValueList<Package> updates;

      
      installs = mapListToPackageList(packagesToInstall.values());
      removals = mapListToPackageList(packagesToRemove.values());
      updates = mapListToPackageList(packagesToUpdate.values());
      emit(realPackages(installs, updates, removals));
      verification = true; //next time through we do the resolving again

    }


  } else { //or else we got two IDs for transact
    ZMD_BLOCK(data.front().toList().last().toString()); // block on the transaction ID
    downloadID = data.front().toList().front().toString(); //save the downloadID
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

#ifdef _ABORT_SUPPORTED_
  server->call("zmd.packsys.abort_transaction", QValueList<QVariant>(), 
  this, SLOT(abortData(const QValueList<QVariant>&, const QVariant&)),
  this, SLOT(faultData(int, const QString&, const QVariant&))); 
#endif
}

void ZmdUpdaterCore::abortData(const QValueList<QVariant>& data, const QVariant& t) {
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
    kdWarning() << "Status: " << status.status << endl;
    kdWarning() << "Name: " << status.name << endl;
    kdWarning() << "Percent: " << status.percent << endl;
    kdWarning() << "Message: " << status.messages.front() << endl;
#endif

    if (status.name == "Downloading Packages") {
      if (status.percent > 99) {
        downloadID = "";
      } 
      if (status.status == 4) {
        ZMD_CLEAR;
        timer->stop();
        downloadID = "";
        emit(transactionFinished(ERROR_TRANS_FAIL, status.messages.front()));
      } else {
        emit(downloadProgress(status)); 
      }
    } else {
      if (status.messages.front() == "Preparing..." && status.status == 4) {
        status.status = 1; //this is a bug in ZMD, somehow it transmits this as an error
      }
      if (downloadID.isEmpty() && map["status"].toInt() > 1) {
        ZMD_CLEAR;
        timer->stop();
        
        if (temp != "") {
          if (status.status == 4) {
            emit(serviceAdded(temp, ERROR_INVALID, status.messages.front()));
          } else {
            emit(serviceAdded(temp, ERROR_NONE, QString()));
          }
          temp = "";
        } else {
          if (status.status == 4)
            emit(transactionFinished(ERROR_TRANS_FAIL, status.messages.front()));
          else
            emit(transactionFinished(ERROR_NONE, QString()));
        }
      } else {
        emit(progress(status));
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

    case -1:
      emit(generalFault("We just had some communication trouble with ZMD, it is likely this will not impact your current operation", code));
      break;
    case 0:
      //Thread dies or transaction in progress or many other things
      emit(transactionFinished(ERROR_TRANS_FAIL, message));
      break;
    case 23:
      //Could not connection to host
      emit(generalFault(message, code));
      break;
    case 24: //Connection broken
      emit(generalFault(message, code));
      break;
    case 49:
      //Timeout
      //Don't say anything, sometimes we are just busy...until we get to 4
      if (timeoutCount++ < 4)
        break;
      else 
        emit(generalFault(message, code));
      break;
    case -601:
      //Resolveable not found
      emit(transactionFinished(ERROR_DEP_FAIL, message));
      ZMD_CLEAR;
      timer->stop();
      packagesToInstall.clear();
      packagesToRemove.clear();
      packagesToUpdate.clear();
      break;
    case -603:
      //Dep Failure
      emit(transactionFinished(ERROR_DEP_FAIL, message));
      ZMD_CLEAR;
      timer->stop();
      //clear out the package list after we get a dep failure. 
      packagesToInstall.clear();
      packagesToRemove.clear();
      packagesToUpdate.clear();
      break;
    case -605:
      //Invalid package specified
      emit(transactionFinished(ERROR_INVALID, message));
      ZMD_CLEAR;
      timer->stop();
      break;
    case -606:
      //Invalid catalog specified
      break;
    case -607:
      //Invalid progress ID
      if (temp == "") {
        //If temp is empty, we did not store a service name and thus this is a transaction
        emit(transactionFinished(ERROR_INVALID, message));
      } else {
        emit(serviceAdded(temp, ERROR_INVALID, message));
        temp = "";
      }
      ZMD_CLEAR;
      timer->stop();
      break;
    case -617:
      //Invalid service specified
      emit(serviceAdded(temp, ERROR_INVALID, message));
      ZMD_CLEAR;
      timer->stop();
      temp = "";
      break;
    case -619: 
      //Invalid service type specified
      emit(serviceAdded(temp, ERROR_INVALID_TYPE, message));
      ZMD_CLEAR;
      timer->stop();
      temp = "";
      break;
    case -667:
      //Invalid id
      emit(generalFault(message, code));
      break;
    default:
      //Things we do not handle
      emit(generalFault(message, code));
      break;
  }
#ifdef DEBUG
  kdError() << "Fault: " << message << endl;
  kdError() << "Fault Code: " << code << endl;
#endif
}
