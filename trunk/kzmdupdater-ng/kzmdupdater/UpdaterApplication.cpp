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

#include "UpdaterApplication.h"

UpdaterApplication::UpdaterApplication() : KUniqueApplication(true,true,false) {

	main = new MainWindow();
	setMainWidget(main);
	//We need to fetch this option from a config file later on
	updater = new ZmdUpdater();

	connect(updater, SIGNAL(updateApplet(int)), main, SLOT(appletState(int)));
	connect(main, SIGNAL(startInstall()), updater, SLOT(startInstall()));
	connect(main, SIGNAL(configureUpdater()), updater, SLOT(configureUpdater()));
	connect(main, SIGNAL(populateUpdateList(QListView*)), updater, SLOT(populateUpdateList(QListView*)));
	main->checkUpdates();

}

UpdaterApplication::~UpdaterApplication() {
	delete main;
}
