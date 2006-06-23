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
#include <kmessagebox.h>

#include <qvariant.h>


InstallWindow::InstallWindow(UpdaterCore *_core, QWidget *parent) : 
	QWidget(parent,0,Qt::WDestructiveClose) {
	core = _core;
	initGUI();
}

InstallWindow::~InstallWindow() {
}

void InstallWindow::initGUI() {

	header = new HeaderWidget(this);
	transactionList = new KTextEdit(this);
	progressBar = new KProgress(100, this);
	abortButton = new KPushButton(i18n("Abort Upgrade"), this);
	mainLayout = new QVBoxLayout(this);

	header->setDescription("<b>Installing updates and patches:</b><br> Below is a description of the packages being installed.<br>");

	mainLayout->addWidget(header, false, 0);
	mainLayout->addWidget(transactionList, false, 0);
	mainLayout->addWidget(progressBar, false, 0);
	mainLayout->addWidget(abortButton, false, Qt::AlignRight);

	transactionList->setReadOnly(true);

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
	static QString text;
	static bool isDownload = false; //We will keep track of this to tell us
									//if we are watching a download or not
	static bool watchingPackage = false; 

	/* QString text will only be blank the first time, because its static
	   So, we use it to test if this is our first run
	*/
	if (text.isEmpty())
		transactionList->setText("");

	if (isDownload) {
		progressBar->advance((int)status.percent);
		if (watchingPackage == false) {
			text = transactionList->text();
			transactionList->setText(text + "\n" + status.name + " Is Downloading...");
			watchingPackage = true;
		}
		if (status.percent >= 100) {
			text =transactionList->text();
			transactionList->setText(text + " Done.");
			watchingPackage = false;
			isDownload = false;
		}
	} else {
		progressBar->advance((int)status.percent);
		if (watchingPackage == false) {
			text = transactionList->text();
			transactionList->setText(text + "\n" + status.name + " Is Being Installed...");
			watchingPackage = true;
		}
		if (status.percent >= 100) {
			text =transactionList->text();
			transactionList->setText(text + " Done.");
			watchingPackage = false;
//			isDownload = true; //Next
		}	
	}
}

void InstallWindow::finished(int status) {
	if (status == ERROR_DEP_FAIL) {
		KMessageBox::error(this, i18n("Sorry, we couldn't resolve the dependencies for this update."));
		close();
	} else {
		transactionList->setText("Done!");
		close();
	}
}

void InstallWindow::setPackageList(QValueList<Package> installs, 
								   QValueList<Package> updates,
								   QValueList<Package> removals) {
	installList = installs;
	updateList = updates;
	removeList = removals;
}

void InstallWindow::startUpdate() {
	core->runTransaction(installList, updateList, removeList);	
	connect(core, SIGNAL(progress(Progress)), this, SLOT(progress(Progress)));
	connect(core, SIGNAL(transactionFinished(int)), this, SLOT(finished(int)));
	transactionList->setText(i18n("Resolving Dependencies..."));
}
