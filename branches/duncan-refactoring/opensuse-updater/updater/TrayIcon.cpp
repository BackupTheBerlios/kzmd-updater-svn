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
#include <kiconloader.h>
#include <klocale.h>

#include <qtooltip.h>

#include "TrayIcon.h"
#include "Updater.h"

TrayIcon::TrayIcon(QWidget *parent) : KSystemTray(parent), 
																			updateCount(0),
																			appletState(APPLET_NO_UPDATES) {
	KIconLoader iconLoader(PROGRAM_NAME);

	setPixmap(loadIcon(TRAY_ICON_GREEN));
	setScaledContents(true);
	setState(APPLET_NO_UPDATES);
  kdDebug() << "Tray Icon ready" << endl;
}

void TrayIcon::setState(int state, const QString &description )
{
  kdDebug() << "Set applet status to: " << state << endl;
	QToolTip::remove(this);
	appletState = state;
	switch (state)
  {
		case APPLET_CHECKING:
			setPixmap(loadIcon(TRAY_ICON_CHECKING));
			QToolTip::add(this, i18n("Checking for updates..."));
			break;
    case APPLET_PROBLEM:
			setPixmap(loadIcon(TRAY_ICON_PROBLEM));
			QToolTip::add(this, i18n("Error:\n" + description ));
			break;
		case APPLET_NO_UPDATES:
			setPixmap(loadIcon(TRAY_ICON_GREEN));
			QToolTip::add(this, i18n("No Updates Available"));
			break;
		case APPLET_UPDATES:
			setPixmap(loadIcon(TRAY_ICON_YELLOW));
			QToolTip::add(this, i18n("1 Update Available", "%n Updates Available", updateCount) );
      
		  break;
    case APPLET_CRITICAL_UPDATES:
			setPixmap(loadIcon(TRAY_ICON_RED));
			QToolTip::add(this, i18n("1 Important Update Available", "%n Important Updates Available", updateCount) );
			break;
	}
}

void TrayIcon::setUpdates(int count) {
	if (count >= 0)
		updateCount = count;
	else
		updateCount = 0;
	setState(appletState);
}

