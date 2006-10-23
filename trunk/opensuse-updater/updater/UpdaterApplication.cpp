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

#include <kglobal.h>
#include <kconfig.h>
#include <kdebug.h>

#include "UpdaterApplication.h"
#include "opensuseupdater_settings.h"

#include "Updater.h"
#include "MainWindow.h"
#include "Constants.h"

//Backends
#include "ZmdUpdater.h"
#include "ZYppUpdater.h"

UpdaterApplication::UpdaterApplication() : KUniqueApplication(true,true,false)
{

 	UpdaterSettings::EnumBackend::type backend;
  
  
 	if ( backend == UpdaterSettings::EnumBackend::zmd )
  {
      kdDebug() << "Using ZMD backend..." << endl;
 			updater = new ZmdUpdater();
  }
 	else if ( backend == UpdaterSettings::EnumBackend::zypp )
  {
      kdDebug() << "Using ZYPP backend..." << endl;
      updater = new ZYppUpdater();
  }
  else
  {
      kdDebug() << "No backend selected. Using ZYPP backend..." << endl;
 			updater = new ZYppUpdater();
      kdDebug() << "backend initialized" << endl;
 	}

	main = new MainWindow(updater->capabilities());
	setMainWidget(main);

	//Connects the signals 

	//Signal that controls applet state
	connect(updater, SIGNAL(updateApplet(int, int)), main, SLOT(slotAppletState(int, int)));

  connect(updater, SIGNAL(updateAppletError(const QString &)), main, SLOT(slotAppletError(const QString &)));
  
	//Signal which allows updater "plugins" to force an update refresh
	connect(updater, SIGNAL(refreshList()), main, SLOT(checkUpdates()));

	//Signal to hide selection buttons if updater doesn't pack QCheckListItems
	connect(updater, SIGNAL(disableSelectButtons()), main, SLOT(disableSelectButtons()));

	//Starts installation
	connect(main, SIGNAL(startInstall()), updater, SLOT(startInstall()));

	//Starts updater backend configuration
	connect(main, SIGNAL(configureUpdater()), updater, SLOT(configureUpdater()));

	//Start update list populate, makes backend check for updates
	connect(main, SIGNAL(populateUpdateList(QListView*)), updater, SLOT(populateUpdateList(QListView*)));

	//Signal to inform the mainwindow of new updates in the list
	connect(updater, SIGNAL(populateDone()), main, SLOT(populateDone()));

	//Signals to get backend to put together a description for selected update and to return that description
	connect(main, SIGNAL(updateSelected(QListViewItem*)), updater, SLOT(updateSelected(QListViewItem*)));
	connect(updater, SIGNAL(returnDescription(QString)), main, SLOT(gotDescription(QString)));

	//Signal to tell the backend to show a menu at the specified position
	connect(main, SIGNAL(updateMenu(QListViewItem*, const QPoint&)), updater, SLOT(updateMenu(QListViewItem*, const QPoint&)));

	main->checkUpdates();

}

UpdaterApplication::~UpdaterApplication() {
	delete main;
	delete updater;
}

