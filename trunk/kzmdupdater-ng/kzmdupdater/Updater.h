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

#ifndef _UPDATER_H_
#define _UPDATER_H_

#include <ksystemtray.h>
#include <qobject.h>
#include <qlistview.h>

//Used to set the applet state
enum { 	APPLET_NO_UPDATES,
		APPLET_UPDATES,
		APPLET_CHECKING };

#define PROGRAM_NAME "kzmdupdater"

class Updater : public QObject {

	Q_OBJECT

	public:

		Updater() : QObject() {}

	signals:

		void updateApplet(int);

	protected slots:

		virtual void configureUpdater() = 0;
		virtual void startInstall() = 0;
		virtual void populateUpdateList(QListView*) = 0;
};


#endif

