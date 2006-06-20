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

#ifndef _INSTALL_WINDOW_
#define _INSTALL_WINDOW_

#include <kprogress.h>
#include <kpushbutton.h>

#include <qwidget.h>
#include <qlistview.h>
#include <qlabel.h>
#include <qlayout.h>

#include "HeaderWidget.h"
#include "UpdaterCore.h"

class InstallWindow : public QWidget {

	Q_OBJECT

	public:

		InstallWindow(UpdaterCore *_core, QWidget *parent=0);
		~InstallWindow();

		void setCore(UpdaterCore *newCore);
		void setPackageList(QValueList<Package>, QValueList<Patch>);

	private slots:

		void abortButtonClicked();

		//Core Signals

		void progress(Progress);
		void finished(int);


	private:

		void initGUI();

		HeaderWidget 			*header;
		QListView 				*installList;
		KProgress 				*progressBar;
		KPushButton 			*abortButton;

		QVBoxLayout 			*mainLayout;

		UpdaterCore 			*core;

		QValueList<QVariant> 	updates;
		QValueList<QVariant> 	installs;
		QValueList<QVariant> 	removes;
};


#endif
