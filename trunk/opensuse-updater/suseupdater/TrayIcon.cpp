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

#include <kiconloader.h>
#include <klocale.h>

#include <qtooltip.h>

#include "TrayIcon.h"
#include "Updater.h"

TrayIcon::TrayIcon(QWidget *parent) : KSystemTray(parent), 
																			updateCount(0),
																			appletState(APPLET_NO_UPDATES) {
	KIconLoader iconLoader(PROGRAM_NAME);

	setPixmap(UserIcon(TRAY_ICON_GREEN));
	setScaledContents(true);
	setState(APPLET_NO_UPDATES);
}

void TrayIcon::setState(int state) {
	QToolTip::remove(this);
	appletState = state;
	switch (state) {
		case APPLET_CHECKING: //We do not have a special icon for checking updates
		case APPLET_NO_UPDATES:
			setPixmap(UserIcon(TRAY_ICON_GREEN));
			QToolTip::add(this, i18n("No Updates Available"));
			break;
		case APPLET_UPDATES:
			setPixmap(UserIcon(TRAY_ICON_RED));
			QToolTip::add(this, QString().setNum(updateCount) + 
													( updateCount == 1 ? 
														i18n(" Update Available") :
														i18n(" Updates Available")));
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

