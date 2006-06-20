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

#include "InstallWindow.h"
#include <klocale.h>

#include <qvariant.h>


InstallWindow::InstallWindow(UpdaterCore *_core, QWidget *parent) : 
	QWidget(parent,0,Qt::WDestructiveClose) {
	core = _core;
	initGUI();

	connect(core, SIGNAL(progress(Progress)), this, SLOT(progress(Progress)));
	connect(core, SIGNAL(transactionFinished(int)), this, SLOT(finished(int)));
}

InstallWindow::~InstallWindow() {
}

void InstallWindow::initGUI() {

	header = new HeaderWidget(this);
	installList = new QListView(this);
	progressBar = new KProgress(this);
	abortButton = new KPushButton(i18n("Abort Upgrade"), this);
	mainLayout = new QVBoxLayout(this);

	header->setDescription("<b>Installing updates and patches:</b><br> Below is a description of the packages being installed.<br>");

	mainLayout->addWidget(header, false, 0);
	mainLayout->addWidget(installList, false, 0);
	mainLayout->addWidget(progressBar, false, 0);
	mainLayout->addWidget(abortButton, false, Qt::AlignRight);

	abortButton->setMinimumHeight(30);
	connect(abortButton, SIGNAL(clicked()), this, SLOT(close()));

	mainLayout->setMargin(10);
	mainLayout->setSpacing(10);
	setCaption(i18n("Installing Packages..."));
	resize(300,350);
}

void InstallWindow::abortButtonClicked() {
}

void InstallWindow::progress(Progress status) {
}

void InstallWindow::finished(int status) {
}

void InstallWindow::setCore(UpdaterCore *newCore) {
	core = newCore;
}

void InstallWindow::setPackageList(QValueList<Package> packageList, QValueList<Patch> patchList) {
//for now we only load the update list

	QValueList<Package>::iterator iter;

	for (iter = packageList.begin(); iter != packageList.end(); iter++) {
		QMap<QString, QVariant> map;
		map["id"] = (*iter).id;
		map["name"] = (*iter).name;
/*		map["version"] = "";
		map["arch"] = "";
		map["catalog"] = "";
		map["install"] = true;
		map["summary"] = "";
*/
		updates.append(map);
	}

}
