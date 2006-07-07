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

enum { 	REMOTE_BUTTON_ON, 
		REMOTE_BUTTON_OFF,
		CERT_BUTTON_YES, 
		CERT_BUTTON_NO, 
		ROLLBACK_BUTTON_ON, 
		ROLLBACK_BUTTON_OFF };

ZmdAdvancedConfig::ZmdAdvancedConfig(QWidget *parent) : QWidget(parent, "AdvancedTab", 0) {
	
	initGUI();
	
	parser = new ZmdRugParser(this);	
	saveProc = NULL;
	proc = new QProcess(QString("rug"), this);
	proc->addArgument("get-prefs");
	connect(proc, SIGNAL(processExited()), this, SLOT(stdinReady()));
	connect(proc, SIGNAL(readyReadStderr()), this, SLOT(errorReady()));
	if (!proc->start())
		KMessageBox::error(this, i18n("To use this part of the updater, rug must be in your path"));
}

void ZmdAdvancedConfig::initGUI() {

	mainLayout = new QGridLayout(this, 4, 2);
	hostLabel = new QLabel(i18n("ZMD Is Listening on: "), this);
	hostEdit = new KLineEdit(this);
	remoteLabel = new QLabel(i18n("ZMD TCP Support: "), this);
	remoteButtons = new QHButtonGroup(this);
	certLabel = new QLabel(i18n("Require GPG Certificates For Servers: "), this);
	certButtons = new QHButtonGroup(this);
	logLabel = new QLabel(i18n("ZMD Logging Level: "), this);
	logBox = new QComboBox(false, this);
	rollbackLabel = new QLabel(i18n("Rollback Support: "), this);
	rollbackButtons = new QHButtonGroup(this);

	QRadioButton *onButton = new QRadioButton(i18n("On"), remoteButtons);
	QRadioButton *offButton = new QRadioButton(i18n("Off"), remoteButtons);
	remoteButtons->insert(onButton, REMOTE_BUTTON_ON);
	remoteButtons->insert(offButton, REMOTE_BUTTON_OFF);

	onButton = new QRadioButton(i18n("Yes"), certButtons);
	offButton = new QRadioButton(i18n("No"), certButtons);
	certButtons->insert(onButton,CERT_BUTTON_YES);
	certButtons->insert(offButton,CERT_BUTTON_NO);

	onButton = new QRadioButton(i18n("On"), rollbackButtons);
	offButton = new QRadioButton(i18n("Off"), rollbackButtons);
	rollbackButtons->insert(onButton, ROLLBACK_BUTTON_ON);
	rollbackButtons->insert(offButton, ROLLBACK_BUTTON_OFF);

	certButtons->setExclusive(true);
	certButtons->setButton(CERT_BUTTON_NO);
	remoteButtons->setExclusive(true);
	remoteButtons->setButton(REMOTE_BUTTON_OFF);
	rollbackButtons->setExclusive(true);
	rollbackButtons->setButton(ROLLBACK_BUTTON_OFF);

	logBox->insertItem("off",-1);
	logBox->insertItem("fatal",-1);
	logBox->insertItem("error",-1);
	logBox->insertItem("warn",-1);
	logBox->insertItem("info",-1);
	logBox->insertItem("debug",-1);
	logBox->setCurrentText("false");

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


	logBox->setDisabled(true);
	hostEdit->setDisabled(true);
	certButtons->setDisabled(true);
	remoteButtons->setDisabled(true);
	rollbackButtons->setDisabled(true);

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
	}

	if (parser->getProperty("remote-enabled") == "True") {
		remoteButtons->setButton(REMOTE_BUTTON_ON);
	} else {
		remoteButtons->setButton(REMOTE_BUTTON_OFF);
	}

	if (parser->getProperty("require-verified-certs") == "True") {
		certButtons->setButton(CERT_BUTTON_YES);
	} else {
		certButtons->setButton(CERT_BUTTON_NO);
	}
	
	data = "";
	data = parser->getProperty("log-level");
	if (data.isEmpty() == false) {
		logBox->setCurrentText(data);
	}
	
	if (parser->getProperty("rollback") == "True") {
		rollbackButtons->setButton(ROLLBACK_BUTTON_ON);
	} else {
		rollbackButtons->setButton(ROLLBACK_BUTTON_OFF);
	}
	logBox->setDisabled(false);
	hostEdit->setDisabled(false);
	certButtons->setDisabled(false);
	remoteButtons->setDisabled(false);
	rollbackButtons->setDisabled(false);

	connect(remoteButtons, SIGNAL(clicked(int)), this, SLOT(settingsChange(int)));		
	connect(certButtons, SIGNAL(clicked(int)), this, SLOT(settingsChange(int)));	
	connect(rollbackButtons, SIGNAL(clicked(int)), this, SLOT(settingsChange(int)));
	connect(hostEdit, SIGNAL(returnPressed()), this, SLOT(settingsChange()));
	connect(logBox, SIGNAL(activated(const QString&)), this, SLOT(settingsChange(const QString&)));
}

void ZmdAdvancedConfig::settingsChange() {
	kdWarning() << "Setting changed" << endl;
	saveProc = new QProcess(QString("rug"), this);
	saveProc->addArgument("set-prefs");
	saveProc->addArgument("bind-ip");
	saveProc->addArgument(hostEdit->text());
	saveProc->start();
}

void ZmdAdvancedConfig::settingsChange(const QString &newText) {
	kdWarning() << "Setting changed" << endl;
	saveProc = new QProcess(QString("rug"), this);
	saveProc->addArgument("set-prefs");
	saveProc->addArgument("log-level");
	saveProc->addArgument(logBox->currentText());
	saveProc->start();
}

void ZmdAdvancedConfig::settingsChange(int id) {

	//note: since 0 = REMOTE_BUTTON_ON and 1 = REMOTE_BUTTON_OFF, the opposite matches our bool
	QString settingName;
	bool settingValue;

	switch (id) {

		case REMOTE_BUTTON_ON:
			settingName = "remote-enabled";
			settingValue = true;
			break;
		case REMOTE_BUTTON_OFF:
			settingName = "remote-enabled";
			settingValue = false;
			break;
		case ROLLBACK_BUTTON_ON:
			settingName = "rollback";
			settingValue = true;
			break;
		case ROLLBACK_BUTTON_OFF:
			settingName = "rollback";
			settingValue = false;
			break;
		case CERT_BUTTON_YES:
			settingName = "require-verified-certs";
			settingValue = true;
			break;
		case CERT_BUTTON_NO:
			settingName = "require-verified-certs";
			settingValue = false;
			break;
	}
	if (settingName.isEmpty() == false) {
		saveProc = new QProcess(QString("rug"), this);
		saveProc->addArgument("set-prefs");
		saveProc->addArgument(settingName);
		saveProc->addArgument((settingValue == true) ? "True" : "False");
		kdWarning() << saveProc->arguments().join(" ") << endl;
		if (!saveProc->start()) {
			KMessageBox::error(this, i18n("Rug must be installed to configure ZMD, is it in your path?"));
			return;
		}
		connect(saveProc, SIGNAL(readyReadStderr()), this, SLOT(errorReady()));
	}
}

void ZmdAdvancedConfig::errorReady() {
	QByteArray byteData;
	QString *data;

	if (saveProc != NULL)
		byteData = saveProc->readStderr();
	else  // the else case here is really only going to be true once
		byteData = proc->readStderr();
	data = new QString(byteData);
	if (data->stripWhiteSpace() != "") {
		KMessageBox::error(this, *data);
	}
	delete data;
}

ZmdAdvancedConfig::~ZmdAdvancedConfig() {

}

