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

#ifndef _TRAY_ICON_H_
#define _TRAY_ICON_H_

#include <ksystemtray.h>

/**
	@file

	Defines TrayIcon, our tray icon. Shocking.
**/

/**
	TrayIcon is our tray icon, which exists basically just to allow
	us to show tool tips.

	@author Narayan Newton <narayannewton@gmail.com>
**/
class TrayIcon : public KSystemTray {

	Q_OBJECT

	public:

		TrayIcon(QWidget *parent=0);

	public slots:
    
    void setState(int state, const QString &description = QString() );
		void setUpdates(int count);

	private:

		int updateCount; 
		int appletState;
};

#endif

