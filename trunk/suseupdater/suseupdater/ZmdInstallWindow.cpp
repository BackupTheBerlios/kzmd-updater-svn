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
#include <kprogress.h>
#include <kpushbutton.h>
#include <ktextedit.h>

#include <qmessagebox.h>
#include <qvariant.h>
#include <qlistview.h>
#include <qlabel.h>
#include <qlayout.h>

#include "HeaderWidget.h"
#include "ZmdInstallWindow.h"
#include "ZmdDependencyDialog.h"

ZmdInstallWindow::ZmdInstallWindow(ZmdUpdaterCore *_core, QWidget *parent) : 
	QWidget(parent,0,Qt::WDestructiveClose | Qt::WShowModal) {
	core = _core;
	initGUI();

	//All of our many watch variables. God this is shit...
	watchingDownload = false;
	watchingPackage = false;
	downloadDone = false;
	packageDone = false;
	reallyDone = false;

	//connect our signals
	connect(core, SIGNAL(downloadProgress(Progress)), 
					this, SLOT(download(Progress)));

	connect(core, SIGNAL(progress(Progress)), 
					this, SLOT(progress(Progress)));

	connect(core, SIGNAL(transactionFinished(int,QString)), 
					this, SLOT(finished(int,QString)));	

	connect(core, SIGNAL(generalFault(QString, int)), 
					this, SLOT(generalFault(QString, int)));

	lastError = -1; //This cannot be 0, as we have a 0 error code
}

void ZmdInstallWindow::initGUI() {

	header = new HeaderWidget(this);
	transactionList = new KTextEdit(this);
	progressBar = new KProgress(100, this);
	mainLayout = new QVBoxLayout(this);

	header->setDescription("<b>Installing updates and patches:</b><br> Below is a description of the transaction and its progress.<br>");

#ifdef _ABORT_SUPPORTED_
	// we can't yet abort an upgrade
	abortButton = new KPushButton(i18n("Abort Upgrade"), this); 
	mainLayout->addWidget(abortButton, false, Qt::AlignRight);
	connect(abortButton, SIGNAL(clicked()), this, SLOT(abortButtonClicked()));
#endif

	mainLayout->addWidget(header, false, 0);
	mainLayout->addWidget(transactionList, false, 0);
	mainLayout->addWidget(progressBar, false, 0);

	transactionList->setReadOnly(true);

	mainLayout->setMargin(10);
	mainLayout->setSpacing(10);
	setCaption(i18n("Installing Packages..."));
	resize(300,350);

}

void ZmdInstallWindow::closeWindow() {
	reallyDone = true;
	close();
}

void ZmdInstallWindow::abortButtonClicked() {
	core->cancelTransaction();
	closeWindow();
}

void ZmdInstallWindow::gotDepInfo(QValueList<Package> installs,
							   									QValueList<Package> updates,
																  QValueList<Package> removals) {
	QString text;
	QValueList<Package>::iterator iter;
	ZmdDependencyDialog diag;

	if (installs.size() > 0) {	
		text = i18n("The following packages will be installed:\n");
		for (iter = installs.begin(); iter != installs.end(); iter++) {
			text += (*iter).name + " " + (*iter).version;
			text += "\n";
		}
	}
	if (updates.size() > 0) {
		text += i18n("The following packages will be updated:\n");
		for (iter = updates.begin(); iter != updates.end(); iter++) {
			text += (*iter).name + " " + (*iter).version;
			text += "\n";
		}
	}
	if (removals.size() > 0) {
		text += i18n("\nThe following packages must also be removed:\n");
		for (iter = removals.begin(); iter != removals.end(); iter++) {
			text += (*iter).name + " " + (*iter).version;
			text += "\n";
		}
	}
	diag.setTitle(i18n("Transaction Summary"));
	diag.setText(text);
	if (diag.exec() == QDialog::Accepted) {	
		core->runTransaction();
	} else {
		core->cancelTransaction();
		closeWindow();
	}
}

void ZmdInstallWindow::download(Progress status) {

	//If the following is true, we are starting at 100%...this is a Zypp bug
	if (status.status == 2 && watchingDownload == false) {
		transactionList->setText(transactionList->text() + "\nWe are currently unable to show the progress for the download. This is caused by a bug in ZMD, which should be fixed soon. We apologize for any inconvenience this may cause." + "\nPackages Are Downloading...");	
		progressBar->setDisabled(true);
		watchingDownload = true; //So we will be dropping into the real code on the next progress drop

	} else if (status.status < 4) {
		progressBar->setValue((int)status.percent);

		if (watchingDownload == false) { //if this is the first time we are watching a package
			transactionList->setText(transactionList->text() + "\n" + "Packages Are Downloading...");
			watchingDownload = true;
			downloadDone = false;
		}

		if (status.status == 2 && downloadDone == false) {
			transactionList->setText(transactionList->text() + "Done");
			downloadDone = true;
		}
	} else {
		//This should never run 
		KMessageBox::error(this, i18n("Download failed: ") + status.messages.front());
	}
}

void ZmdInstallWindow::progress(Progress status) {

	if (status.status > 0 && status.status != 4) {

		//if we get here and download Done is falt, we are encountering the zypp bug
		if (downloadDone == false) { //we have just started the transaction, download is now done
			transactionList->setText(transactionList->text() + "Done");
			//Re-enable when download is really done
			progressBar->setDisabled(false);
			downloadDone = true;
		}

		progressBar->setValue((int)status.percent);
		if (watchingPackage == false && status.status == 1) {
			//if we are not already watching a package and the transaction is running
			transactionList->setText(transactionList->text() + "\n" + "Packages Are Being Installed...");
			watchingPackage = true;
		}
		if (status.status ==2 && packageDone == false) {
			//if the transaction is done and we have not already marked it done
			transactionList->setText(transactionList->text() + " Done.");
			watchingPackage = false;
			packageDone = true;
		}	
	} else if (status.status == 4) {
		//This should never run
		KMessageBox::error(this, status.messages.front());
	}
}

void ZmdInstallWindow::finished(int status, QString error) {
	if (status == ERROR_DEP_FAIL) {
		KMessageBox::error(this, i18n("Sorry, we couldn't resolve the dependencies for this update:\n")+error);
	} else if (status == ERROR_TRANS_FAIL) {
		KMessageBox::error(this, i18n("Upgrade Failed: ") + error);
	} else {
		transactionList->setText("Done!");
	}
	closeWindow();
}

void ZmdInstallWindow::generalFault(QString message, int errorCode) {

	if (lastError != errorCode) {
		lastError = errorCode;
		KMessageBox::error(this, message);
	} else {
		//If we have shown this error before, don't show it again.
		return;
	}
}

void ZmdInstallWindow::setPackageList(QValueList<Package> installs, 
																		  QValueList<Package> updates,
								   										QValueList<Package> removals) {
	installList = installs;
	updateList = updates;
	removeList = removals;
}

void ZmdInstallWindow::startUpdate() {
	core->startTransaction(installList, updateList, removeList);	
	connect(core, SIGNAL(realPackages(QValueList<Package>, 
																		QValueList<Package>, 
																		QValueList<Package>)), 
					this, SLOT(gotDepInfo(QValueList<Package>,
																QValueList<Package>, 
																QValueList<Package>)));

	transactionList->setText(i18n("Resolving Dependencies..."));
}

void ZmdInstallWindow::closeEvent(QCloseEvent *e) {
	if (reallyDone) {
		emit(refreshUpdates());
		e->accept();
	} else {
		e->ignore();
	}
}

