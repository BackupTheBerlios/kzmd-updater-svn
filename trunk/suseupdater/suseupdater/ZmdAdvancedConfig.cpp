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
#include "ZmdAdvancedConfig.h"

#include <kdebug.h>
#include <kmessagebox.h>

#include <qradiobutton.h>
#include <qprocess.h>
#include <qcombobox.h>

#include "ZmdRugParser.h"

enum { REMOTE_BUTTON_ON, REMOTE_BUTTON_OFF };
enum { CERT_BUTTON_YES, CERT_BUTTON_NO };
enum { ROLLBACK_BUTTON_ON, ROLLBACK_BUTTON_OFF };

ZmdAdvancedConfig::ZmdAdvancedConfig(QWidget *parent) : QWidget(parent, "AdvancedTab", 0) {
	
	initGUI();
	
	parser = new ZmdRugParser(this);
	proc = new QProcess(QString("rug"));
	proc->addArgument("get-prefs");
	connect(proc, SIGNAL(processExited()), this, SLOT(stdinReady()));
	if (!proc->start())
		KMessageBox::error(this, i18n("To use this part of the updater, rug must be in your path"));

	//init our data variables
	oldHost = "";
	oldRemote = false;
	oldCert = false;
}

void ZmdAdvancedConfig::initGUI() {

	mainLayout = new QGridLayout(this, 4, 2);
	hostLabel = new QLabel(i18n("ZMD Is Listening on: "), this);
	hostEdit = new KLineEdit(this);
	remoteLabel = new QLabel(i18n("ZMD TCP Support: "), this);
	remoteButtons = new QHButtonGroup(this);
	QRadioButton *onButton = new QRadioButton(i18n("On"), remoteButtons);
	QRadioButton *offButton = new QRadioButton(i18n("Off"), remoteButtons);
	certLabel = new QLabel(i18n("Require GPG Certificates For Servers: "), this);
	certButtons = new QHButtonGroup(this);
	onButton = new QRadioButton(i18n("Yes"), certButtons);
	offButton = new QRadioButton(i18n("No"), certButtons);
	logLabel = new QLabel(i18n("ZMD Logging Level"), this);
	logBox = new QComboBox(false, this);
	rollbackLabel = new QLabel(i18n("Rollback Support: "), this);
	rollbackButtons = new QHButtonGroup(this);
	onButton = new QRadioButton(i18n("On"), rollbackButtons);
	offButton = new QRadioButton(i18n("Off"), rollbackButtons);

	certButtons->setExclusive(true);
	remoteButtons->setExclusive(true);
	rollbackButtons->setExclusive(true);

	logBox->insertItem("off",-1);
	logBox->insertItem("fatal",-1);
	logBox->insertItem("error",-1);
	logBox->insertItem("warn",-1);
	logBox->insertItem("info",-1);
	logBox->insertItem("debug",-1);

	mainLayout->addWidget(hostLabel, 0, 0);
	mainLayout->addWidget(hostEdit, 0, 1);
	mainLayout->addWidget(remoteLabel, 1, 0);
	mainLayout->addWidget(remoteButtons, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);
	mainLayout->addWidget(certLabel, 2, 0);
	mainLayout->addWidget(certButtons, 2, 1, Qt::AlignLeft | Qt::AlignVCenter);
	mainLayout->addWidget(rollbackLabel, 4, 0);
	mainLayout->addWidget(rollbackButtons, 4, 1, Qt::AlignLeft | Qt::AlignVCenter);
	mainLayout->addWidget(logLabel, 3, 0);
	mainLayout->addWidget(logBox, 3, 1);
	mainLayout->setSpacing(10);
	mainLayout->setMargin(10);
	show();
}

void ZmdAdvancedConfig::stdinReady() {
	QString data;
	QByteArray byteData;

	byteData = proc->readStdout();
	parser->setData(byteData);
	
	data = parser->getProperty("bind-ip");
	if (data.isEmpty() == false) {
		hostEdit->setText(data);
		oldHost = data;
	}

	if (parser->getProperty("remote-enabled") == "True") {
		remoteButtons->setButton(REMOTE_BUTTON_ON);
		oldRemote = true;
	} else {
		remoteButtons->setButton(REMOTE_BUTTON_OFF);
		oldRemote = false;
	}

	if (parser->getProperty("require-verified-certs") == "True") {
		certButtons->setButton(CERT_BUTTON_YES);
		oldCert = true;
	} else {
		certButtons->setButton(CERT_BUTTON_NO);
		oldCert = false;
	}
	
	data = "";
	data = parser->getProperty("log-level");
	if (data.isEmpty() == false) {
		logBox->setCurrentText(data);
		oldLog = data;
	}
	
	if (parser->getProperty("rollback") == "True") {
		rollbackButtons->setButton(ROLLBACK_BUTTON_ON);
		oldRollback = true;
	} else {
		rollbackButtons->setButton(ROLLBACK_BUTTON_OFF);
		oldRollback = false;
	}
}

ZmdAdvancedConfig::~ZmdAdvancedConfig() {

	QProcess *saveProc;
	//Save settings here
	kdWarning() << "Saving settings." << endl;
	
	if (hostEdit->text() != oldHost) {
		saveProc = new QProcess(QString("rug"), this);
		saveProc->addArgument("set-prefs");
		saveProc->addArgument("bind-ip");
		saveProc->addArgument(hostEdit->text());
		saveProc->start();
	}
	//note: since 0 = REMOTE_BUTTON_ON and 1 = REMOTE_BUTTON_OFF, the opposite matches our bool
	if ((!remoteButtons->selectedId()) != oldRemote) {
		saveProc = new QProcess(QString("rug"), this);
		saveProc->addArgument("set-prefs");
		saveProc->addArgument("remote-enabled");
		saveProc->addArgument( ((!remoteButtons->selectedId()) == true) ? "True" : "False" );
		saveProc->start();
	}
	if ((!certButtons->selectedId()) != oldCert) {
		saveProc = new QProcess(QString("rug"), this);
		saveProc->addArgument("set-prefs");
		saveProc->addArgument("require-verified-certs");
		saveProc->addArgument( ((!certButtons->selectedId()) == true) ? "True" : "False" );
		saveProc->start();
	}
	if (logBox->currentText() != oldLog) {
		saveProc = new QProcess(QString("rug"), this);
		saveProc->addArgument("set-prefs");
		saveProc->addArgument("log-level");
		saveProc->addArgument(logBox->currentText());
		saveProc->start();
	}
	if ((!rollbackButtons->selectedId()) != oldRollback) {
		saveProc = new QProcess(QString("rug"), this);
		saveProc->addArgument("set-prefs");
		saveProc->addArgument("rollback");
		saveProc->addArgument( ((!rollbackButtons->selectedId()) == true) ? "True" : "False" );
		saveProc->start();
	}
}
