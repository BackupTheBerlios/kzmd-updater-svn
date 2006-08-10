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

#include <kapplication.h>
#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <kdebug.h>

#include <iostream>
using namespace std;

#include "MainWindow.h"

/**
  	@author Narayan Newton <narayannewton@gmail.com>
 */

static KCmdLineOptions options[] = 
{
	{ "basic-auth <argument>", I18N_NOOP("Authenticate the given user with basic privs"), "none"},
	KCmdLineLastOption
};

#define PROGRAM_NAME "kzmduserconfig"
#define VERSION "0.1"

int main(int argc, char **argv) {

	KAboutData about(PROGRAM_NAME, PROGRAM_NAME, VERSION, "ZMD UserConfig");
	KCmdLineArgs::addCmdLineOptions(options);
	KCmdLineArgs::init(argc, argv, &about);
	KApplication::addCmdLineOptions();
	
	KApplication *app = new KApplication();

	KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
	MainWindow *main = new MainWindow();

	if (args->getOption("basic-auth") != "none") {
		main->basicAuth(args->getOption("basic-auth"));
	}

	app->setMainWidget(main);
	return app->exec();
}

