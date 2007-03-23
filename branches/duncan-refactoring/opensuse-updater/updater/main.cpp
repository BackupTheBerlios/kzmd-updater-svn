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

#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <kdebug.h>

#include "UpdaterApplication.h"
#include "Constants.h"

/**
  	@author Narayan Newton <narayannewton@gmail.com>
 */

/**
	Simple stub main to start up our app and make sure its not already running.
*/

int main(int argc, char **argv) {

	KAboutData about(PROGRAM_NAME, PROGRAM_NAME, VERSION, "SUSE Updater");
	KCmdLineArgs::init(argc, argv, &about);
	KUniqueApplication::addCmdLineOptions();

	if (!KUniqueApplication::start()) {
		kdError() << "ERROR: We are already running" << endl;
		exit(0);
	}	
	UpdaterApplication app;
	app.disableSessionManagement();

	app.exec();
	return 0;
}

