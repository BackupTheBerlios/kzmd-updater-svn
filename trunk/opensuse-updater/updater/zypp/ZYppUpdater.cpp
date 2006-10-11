/* 
   Copyright (C) 2006 Novell Inc

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

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/poll.h>
#include <sys/types.h>
#include <fcntl.h>

#include <qobject.h>
#include <qpopupmenu.h>

#include <kprocess.h>
#include <kdebug.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kprocess.h>
#include <kconfig.h>
#include <kapp.h>

#include "ZYppUpdater.h"
#include "MainWindow.h"
//#include "ZYppInstallWindow.h"
//#include "ZYppConfigWindow.h"
#include "UpdateListItem.h"

ZYppUpdater::ZYppUpdater() : Updater()
{

}

void ZYppUpdater::processExited( KProcess * )
{}
 
void ZYppUpdater::showLog()
{}

void ZYppUpdater::slotProcessExited( KProcess *proc )
{}

void ZYppUpdater::slotReceivedStdout(KProcess *proc, char *buffer, int buflen)
{}

void ZYppUpdater::slotReceivedStderr(KProcess *proc, char *buffer, int buflen)
{}

void ZYppUpdater::doCheckForUpdates()
{
  kdDebug() << "checking..." << endl;

  if ( _process ) {
    kdDebug() << "Check still running." << endl;
    return;
  }

  _process = new KProcess;

  //*mProcess << "online_update" << "-k" << "-s";
  *_process << "zypp-checkpatches";
  //if ( !mCommandLineOptions.isEmpty() ) *mProcess << mCommandLineOptions;
  
  QObject::connect( _process, SIGNAL( processExited( KProcess * ) ),
           SLOT( slotProcessExited( KProcess * ) ) );
  QObject::connect( _process, SIGNAL( receivedStdout(KProcess *, char *, int ) ),
           SLOT( slotReceivedStdout(KProcess *, char *, int ) ) );
  QObject::connect( _process, SIGNAL( receivedStderr(KProcess *, char *, int ) ),
           SLOT( slotReceivedStderr(KProcess *, char *, int ) ) );


  _process->start( KProcess::NotifyOnExit, KProcess::AllOutput );
  //mStatusLabel->setText( i18n("Checking...") );
}

void ZYppUpdater::populateUpdateList(QListView *updateList)
{
  //currentPackages.clear();
  //currentPatches.clear();
}

void ZYppUpdater::updateSelected(QListViewItem *item)
{

}

void ZYppUpdater::updateMenu(QListViewItem *item, const QPoint& point)
{
// 	if (item != NULL && tempList != NULL)
//   {
// 		QPopupMenu *menu = new QPopupMenu(tempList);
// 		if (item->text(COLUMN_LOCK) == "") {
// 			menu->insertItem(i18n("Hold Back Package"), this, SLOT(holdPackage()));
// 		} else {
// 			menu->insertItem(i18n("Don't Hold Back Package"), this, SLOT(removeHold()));
// 		}
// 		menu->popup(point);
// 	}
}


/*****************************************************************************
									These are all noops because we do not support
									package holds yet
*****************************************************************************/
// void ZYppUpdater::holdPackage()
// {
// }
// 
// void ZYppUpdater::removeHold()
// {
// }
/******************************************************************************
******************************************************************************/

void ZYppUpdater::startInstall()
{
// 	if (tempList != NULL) {
// 		QValueList<Package> upList;
// 		QValueList<Package> instList;
// 		UpdateListItem *item = (UpdateListItem*)(tempList->firstChild());
// 
// 		if (item == NULL) {
// 			return;
// 		}
// 
// 		do {
// 			if (item->state() == QCheckListItem::On) {
//         QString id;
// 				id = item->text(COLUMN_ID); //gets the id
// 
//         if ( currentPackages.find( id ) != currentPackages.end() ) {
//           Package p;
//           p = currentPackages[id];
//           if ( p.installed == true )
//             upList.append(p);
//           else
//             instList.append(p);          
//         } else if ( currentPatches.find( id ) != currentPatches.end() ) {
//           Patch p;
//           p = currentPatches[id];
//           if ( p.installed == true )
//             upList.append((Package)p); //can patches be upgraded?
//           else 
//             instList.append((Package)p);
//         }
// 			}
// 		} while ((item = (UpdateListItem*)(item->nextSibling())) != 0);
// 
// 		if (instList.size() > 0 || upList.size() > 0) {
// 			ZYppInstallWindow *win = new ZYppInstallWindow(core); //deletes itself
// 			win->setPackageList(instList, upList, QValueList<Package>());
// 			win->startUpdate();
// 
// 			//Allow the install window to signal an update refresh
// 			connect(win, SIGNAL(refreshUpdates()), 
// 							this, SLOT(startRefresh()));
// 
// 			win->show();
// 		}
// 	}
}

// void ZYppUpdater::startRefresh()
// {
// 	//emit(refreshList());
// }

void ZYppUpdater::configureUpdater()
{
	//ZYppConfigWindow *win = new ZYppConfigWindow(core); //deletes itself
	//win->show();
}

