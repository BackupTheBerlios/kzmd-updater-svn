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

#include <kuniqueapplication.h>
#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <kdebug.h>
#include "SUSEUpdater.h"

#define VERSION "0.0.1"

int main(int argc, char **argv) {

	KAboutData about("kzmdupdater", "kzmdupdater", VERSION, "SUSE Updater");
	KCmdLineArgs::init(argc, argv, &about);
	KUniqueApplication::addCmdLineOptions();


	if (!KUniqueApplication::start()) {
		kdError() << "ERROR: We are already running" << endl;
		exit(0);
	}	
	KUniqueApplication app(true,true,false);
	SUSEUpdater *mainApp = new SUSEUpdater();
	app.setMainWidget(mainApp);
	app.exec();
	return 0;
}
