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

#include <klocale.h>
#include <kmessagebox.h>

#include <qmessagebox.h>
#include <qvariant.h>

#include "InstallWindow.h"
#include "DependencyDialog.h"

InstallWindow::InstallWindow(ZmdUpdaterCore *_core, QWidget *parent) : 
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

void InstallWindow::gotDepInfo(QValueList<Package> installs,
							   QValueList<Package> updates,
							   QValueList<Package> removals) {
	QString text;
	QValueList<Package>::iterator iter;
	DependencyDialog diag;
	
	text = i18n("The following packages most also be installed/updated:\n");
	for (iter = installs.begin(); iter != installs.end(); iter++) {
		text += (*iter).name;
		text += " (I)\n";
	}
	for (iter = updates.begin(); iter != updates.end(); iter++) {
		text += (*iter).name;
		text += " (U)\n";
	}
	for (iter = removals.begin(); iter != removals.end(); iter++) {
		text += (*iter).name;
		text += " (R)\n";
	}
	diag.setTitle(i18n("Other Packages..."));
	diag.setText(text);
	if (diag.exec() == QDialog::Accepted) {
		connect(core, SIGNAL(progress(Progress)), this, SLOT(progress(Progress)));
		connect(core, SIGNAL(transactionFinished(int)), this, SLOT(finished(int)));	
		core->runTransaction();
	} else {
		core->cancelTransaction();
		close();
	}
}

void InstallWindow::progress(Progress status) {
	QString text;
	static bool isDownload = false; //We will keep track of this to tell us
									//if we are watching a download or not
	static bool watchingPackage = false; 

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
		if (status.percent >= 99) {
			text = transactionList->text();
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
	core->startTransaction(installList, updateList, removeList);	
	connect(core, SIGNAL(realPackages(QValueList<Package>, QValueList<Package>, 
			QValueList<Package>)), this, SLOT(gotDepInfo(QValueList<Package>,
			QValueList<Package>, QValueList<Package>)));
	transactionList->setText(i18n("Resolving Dependencies..."));
}
