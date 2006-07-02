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

#include <kapp.h>
#include <kconfig.h>

#include "UpdaterApplication.h"
#include "Constants.h"

UpdaterApplication::UpdaterApplication() : KUniqueApplication(true,true,false) {

	KConfig *config = kapp->config();
	config->setGroup("General");

	switch (config->readEntry("Backend").toInt()) {

		case BACKEND_ZMD:
			updater = new ZmdUpdater();
			break;
		case BACKEND_ZYPP:
		case BACKEND_SMART:

		default:
			updater = new ZmdUpdater();

	}

	main = new MainWindow();
	setMainWidget(main);

	connect(updater, SIGNAL(updateApplet(int)), main, SLOT(appletState(int)));
	connect(updater, SIGNAL(refreshList()), main, SLOT(checkUpdates()));
	connect(updater, SIGNAL(disableSelectButtons()), main, SLOT(disableSelectButtons()));
	connect(main, SIGNAL(startInstall()), updater, SLOT(startInstall()));
	connect(main, SIGNAL(configureUpdater()), updater, SLOT(configureUpdater()));
	connect(main, SIGNAL(populateUpdateList(QListView*)), updater, SLOT(populateUpdateList(QListView*)));
	connect(main, SIGNAL(updateSelected(QListViewItem*)), updater, SLOT(updateSelected(QListViewItem*)));
	connect(updater, SIGNAL(returnDescription(QString)), main, SLOT(gotDescription(QString)));
	main->checkUpdates();

}

UpdaterApplication::~UpdaterApplication() {
	delete main;
}

