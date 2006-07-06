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

#ifndef _ZMD_CONFIG_WINDOW_H_
#define _ZMD_CONFIG_WINDOW_H_

#include <kpushbutton.h>

#include <qwidget.h>
#include <qlayout.h>
#include <qtabwidget.h>

//Column IDs for the configure window
enum { CONFW_NAME=0, CONFW_URI, CONFW_ID };

#include "ZmdEditServers.h"
#include "ZmdAdvancedConfig.h"
#include "HeaderWidget.h"
#include "ZmdUpdaterCore.h"

class ZmdConfigWindow : public QWidget {

	Q_OBJECT

	public:

		ZmdConfigWindow(ZmdUpdaterCore *_core=0, QWidget *parent=0);

	signals:

		void refreshUpdates();

	private slots:

		void serverChange();
		void tabChanged(QWidget *);

	private:

		void initGUI();

		QVBoxLayout *mainLayout;
		HeaderWidget *header;
		QTabWidget *tabs;
		KPushButton *closeButton;

		ZmdEditServers *editServers;
		ZmdAdvancedConfig *advancedConfig;		
		ZmdUpdaterCore *core;
};

#endif

