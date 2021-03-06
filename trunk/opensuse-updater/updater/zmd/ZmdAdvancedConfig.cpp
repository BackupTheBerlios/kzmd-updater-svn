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

#include <klocale.h>
#include <kdebug.h>
#include <kmessagebox.h>
#include <klineedit.h>
#include <kpassdlg.h>
#include <kapp.h>
#include <kconfig.h>

#include <qradiobutton.h>
#include <qprocess.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qhbuttongroup.h>
#include <qhbox.h>
#include <qvgroupbox.h>

#include "ZmdRugParser.h"
#include "ZmdUpdaterCore.h"
#include "ZmdUpdater.h"

#define DEBUG

/** Buttons **/
enum {
  REMOTE_BUTTON_ON, 	/** ZMD TCP ON **/
  REMOTE_BUTTON_OFF, 	/** ZMD TCP OFF **/
  CERT_BUTTON_YES, 		/** Certificate On **/
  CERT_BUTTON_NO,			/** Certificate Off **/
  ROLLBACK_BUTTON_ON, /** Rollbacks On **/
  ROLLBACK_BUTTON_OFF,/** Rollbacks Off **/
};

ZmdAdvancedConfig::ZmdAdvancedConfig(ZmdUpdaterCore *_core, QWidget *parent) :
    QWidget(parent, "AdvancedTab", 0)
{

  core = _core;
  restartZMDOnExit = false;
  initGUI();

  parser = new ZmdRugParser(this);
  saveProc = NULL;
  proc = new QProcess(QString("rug"), this);
  proc->addArgument("get-prefs");

  connect(proc, SIGNAL(processExited()),
          this, SLOT(stdinReady()));

  connect(proc, SIGNAL(readyReadStderr()),
          this, SLOT(errorReady()));

  if (!proc->start())
    KMessageBox::error(this, i18n("To use this part of the updater, rug must be in your path"));
}

void ZmdAdvancedConfig::initGUI()
{

  mainLayout = new QGridLayout(this, 2, 2);

  //Connection stuff
  connectionBox = new QVGroupBox(this);
  connectionBox->setTitle(i18n("Connection Options"));

  QHBox *hostBox = new QHBox(connectionBox);
  hostLabel = new QLabel(i18n("ZMD Is Listening on: "), hostBox);
  hostEdit = new KLineEdit(hostBox);

  QHBox *remoteBox = new QHBox(connectionBox);
  remoteLabel = new QLabel(i18n("ZMD TCP Support: "), remoteBox);
  remoteButtons = new QHButtonGroup(remoteBox);

  QRadioButton *onButton = new QRadioButton(i18n("On"), remoteButtons);
  QRadioButton *offButton = new QRadioButton(i18n("Off"), remoteButtons);
  remoteButtons->insert(onButton, REMOTE_BUTTON_ON);
  remoteButtons->insert(offButton, REMOTE_BUTTON_OFF);
  onButton = offButton = 0;

  //Security stuff
  securityBox = new QVGroupBox(this);
  securityBox->setTitle(i18n("Security Options"));

  QHBox *certBox = new QHBox(securityBox);
  certLabel = new QLabel(i18n("Require GPG Certificates For Servers: "), certBox);
  certButtons = new QHButtonGroup(certBox);

  onButton = new QRadioButton(i18n("Yes"), certButtons);
  offButton = new QRadioButton(i18n("No"), certButtons);
  certButtons->insert(onButton,CERT_BUTTON_YES);
  certButtons->insert(offButton,CERT_BUTTON_NO);
  onButton = offButton = 0;

  QHBox *secLevBox = new QHBox(securityBox);
  securityLevelLabel = new QLabel(i18n("Security Level: "), secLevBox);
  securityLevelBox = new QComboBox(false, secLevBox);

  //Proxy stuff
  proxyBox = new QVGroupBox(this);
  proxyBox->setTitle(i18n("Proxy Options"));

  QHBox *urlBox = new QHBox(proxyBox);
  proxyUrlLabel = new QLabel(i18n("Proxy Server:"), urlBox);
  proxyUrlEdit = new KLineEdit(urlBox);

  QHBox *userBox = new QHBox(proxyBox);
  proxyUsernameLabel = new QLabel(i18n("User Name:"), userBox);
  proxyUsernameEdit = new KLineEdit(userBox);

  QHBox *passBox = new QHBox(proxyBox);
  proxyPasswordLabel = new QLabel(i18n("Password:"), passBox);
  proxyPasswordEdit = new KLineEdit(passBox);

  //"Other" stuff
  otherBox = new QVGroupBox(this);
  otherBox->setTitle(i18n("Other Options"));

  QHBox *logLevelBox = new QHBox(otherBox);
  logLabel = new QLabel(i18n("ZMD Logging Level: "), logLevelBox);
  logBox = new QComboBox(false, logLevelBox);

  QHBox *rollbackBox = new QHBox(otherBox);
  rollbackLabel = new QLabel(i18n("Rollback Support: "), rollbackBox);
  rollbackButtons = new QHButtonGroup(rollbackBox);

  onButton = new QRadioButton(i18n("On"), rollbackButtons);
  offButton = new QRadioButton(i18n("Off"), rollbackButtons);
  rollbackButtons->insert(onButton, ROLLBACK_BUTTON_ON);
  rollbackButtons->insert(offButton, ROLLBACK_BUTTON_OFF);
  onButton = offButton = 0;

  QHBox *downloadsBox = new QHBox(otherBox);
  maxDownloadsLabel = new QLabel(i18n("Max Simultaneous Downloads"), downloadsBox);
  maxDownloadsSpinner = new QSpinBox(downloadsBox);

  certButtons->setExclusive(true);
  certButtons->setButton(CERT_BUTTON_NO);
  remoteButtons->setExclusive(true);
  remoteButtons->setButton(REMOTE_BUTTON_OFF);
  rollbackButtons->setExclusive(true);
  rollbackButtons->setButton(ROLLBACK_BUTTON_OFF);

  logBox->insertItem("off", -1);
  logBox->insertItem("fatal", -1);
  logBox->insertItem("error", -1);
  logBox->insertItem("warn", -1);
  logBox->insertItem("info", -1);
  logBox->insertItem("debug", -1);
  logBox->setCurrentText("false");

  securityLevelBox->insertItem("signature", -1);
  securityLevelBox->insertItem("checksum", -1);
  securityLevelBox->insertItem("none", -1);
  securityLevelBox->setCurrentText("signature");

  maxDownloadsSpinner->setMinValue(1);

  mainLayout->addWidget(connectionBox, 0, 0);
  mainLayout->addWidget(securityBox, 1, 0);
  mainLayout->addWidget(proxyBox, 0, 1);
  mainLayout->addWidget(otherBox, 1, 1);

  mainLayout->setSpacing(10);
  mainLayout->setMargin(10);

  //Disable the entire GUI until we get the first data drop from rug
  logBox->setDisabled(true);
  hostEdit->setDisabled(true);
  certButtons->setDisabled(true);
  remoteButtons->setDisabled(true);
  rollbackButtons->setDisabled(true);
  maxDownloadsSpinner->setDisabled(true);
  securityLevelBox->setDisabled(true);
  proxyUrlEdit->setDisabled(true);
  proxyUsernameEdit->setDisabled(true);
  proxyPasswordEdit->setDisabled(true);

  show();
}

void ZmdAdvancedConfig::stdinReady()
{
  QString data;
  QByteArray byteData;
  int maxDownloads = 0;

  byteData = proc->readStdout();
  parser->setData(byteData);

  data = parser->getProperty("bind-ip");
  if (data.isEmpty() == false)
  {
    hostEdit->setText(data);
  }

  if (parser->getProperty("remote-enabled") == "True")
  {
    remoteButtons->setButton(REMOTE_BUTTON_ON);
  }
  else
  {
    remoteButtons->setButton(REMOTE_BUTTON_OFF);
  }

  if (parser->getProperty("require-verified-certs") == "True")
  {
    certButtons->setButton(CERT_BUTTON_YES);
  }
  else
  {
    certButtons->setButton(CERT_BUTTON_NO);
  }

  data = "";
  data = parser->getProperty("log-level");
  if (data.isEmpty() == false)
  {
    logBox->setCurrentText(data);
  }

  if (parser->getProperty("rollback") == "True")
  {
    rollbackButtons->setButton(ROLLBACK_BUTTON_ON);
  }
  else
  {
    rollbackButtons->setButton(ROLLBACK_BUTTON_OFF);
  }

  if ((maxDownloads = QString(parser->getProperty("max-downloads")).toInt()) > 0)
  {
    maxDownloadsSpinner->setValue(maxDownloads);
  }
  else
  {
    maxDownloadsSpinner->setValue(1);
  }

  data = "";
  data = parser->getProperty("security-level");
  if (data.isEmpty() == false)
  {
    securityLevelBox->setCurrentText(data);
  }

  data = "";
  data = parser->getProperty("proxy-url");
  if (data.isEmpty() == false)
  {
    proxyUrlEdit->setText(data);
  }

  data = "";
  data = parser->getProperty("proxy-username");
  if (data.isEmpty() == false)
  {
    proxyUsernameEdit->setText(data);
  }

  data = "";
  data = parser->getProperty("proxy-password");
  if (data.isEmpty() == false)
  {
    proxyPasswordEdit->setText(data);
  }

  logBox->setDisabled(false);
  hostEdit->setDisabled(false);
  certButtons->setDisabled(false);
  remoteButtons->setDisabled(false);
  rollbackButtons->setDisabled(false);
  maxDownloadsSpinner->setDisabled(false);
  securityLevelBox->setDisabled(false);
  proxyUrlEdit->setDisabled(false);
  if (proxyUrlEdit->text().isEmpty() == false)
  {
    proxyUsernameEdit->setDisabled(false);
    proxyPasswordEdit->setDisabled(false);
  }

  connect(remoteButtons, SIGNAL(clicked(int)),
          this, SLOT(radioButtonsChange(int)));

  connect(certButtons, SIGNAL(clicked(int)),
          this, SLOT(radioButtonsChange(int)));

  connect(rollbackButtons, SIGNAL(clicked(int)),
          this, SLOT(radioButtonsChange(int)));

  connect(hostEdit, SIGNAL(returnPressed()),
          this, SLOT(hostUrlChange()));

  connect(logBox, SIGNAL(activated(const QString&)),
          this, SLOT(logLevelChange(const QString&)));

  connect(securityLevelBox, SIGNAL(activated(const QString&)),
          this, SLOT(securityLevelChange(const QString&)));

  connect(maxDownloadsSpinner, SIGNAL(valueChanged(int)),
          this, SLOT(maxDownloadsValueChange(int)));

  connect(proxyUrlEdit, SIGNAL(returnPressed()),
          this, SLOT(proxyUrlChange()));

  connect(proxyUrlEdit, SIGNAL(textChanged(const QString &)),
          this, SLOT(proxyEnableUserPass()));

  connect(proxyUsernameEdit, SIGNAL(returnPressed()),
          this, SLOT(proxyUsernameChange()));

  connect(proxyPasswordEdit, SIGNAL(returnPressed()),
          this, SLOT(proxyPasswordChange()));
}

/*******************************************************************
 *
 *							The General Settings Change Method
 *
 ******************************************************************/

void ZmdAdvancedConfig::saveSettings(QString setting, QString value)
{

#ifdef DEBUG
  kdWarning() << "Setting changed" << endl;
  kdWarning() << setting << " " << value << endl;
#endif

  saveProc = new QProcess(QString("rug"), this);
  saveProc->addArgument("set-prefs");
  saveProc->addArgument(setting);
  saveProc->addArgument(value);
  if (!saveProc->start())
  {
    KMessageBox::error(this, i18n("Rug must be installed to configure ZMD, is it in your path?"));
    return;
  }
  connect(saveProc, SIGNAL(readyReadStderr()),
          this, SLOT(errorReady()));
}

/*******************************************************************
 *
 *						The Actual Settings Change Slots
 *
 ******************************************************************/

void ZmdAdvancedConfig::hostUrlChange()
{
  KConfig *config = kapp->config();
  config->setGroup("General");

  saveSettings("bind-ip", hostEdit->text());
  config->writeEntry("ZmdServer", hostEdit->text());
}

void ZmdAdvancedConfig::logLevelChange(const QString &newText)
{
  saveSettings("log-level", logBox->currentText());
}

void ZmdAdvancedConfig::securityLevelChange(const QString &newText)
{
  saveSettings("security-level", securityLevelBox->currentText());
}

void ZmdAdvancedConfig::radioButtonsChange(int id)
{

  /*	note: since:
  	0 = REMOTE_BUTTON_ON 
  	1 = REMOTE_BUTTON_OFF, 
  	the opposite matches our bool
  */

  QString settingName;
  bool settingValue = false;

  switch (id)
  {

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
  if (settingName.isEmpty() == false)
  {

    saveSettings(settingName, (settingValue == true) ? "True" : "False");

    if (settingName == "remote-enabled")
    {
      KConfig *config = kapp->config();
      config->setGroup("General");
      config->writeEntry("ZmdProto", ((settingValue == true) ? ZMD_TCP : ZMD_UDS));
      if (settingValue == true)
      {
        core->setServer(QString("http://") + QString(TCP_SERVER_ADDY) + QString(TCP_SERVER_POSTFIX));
        //we only restart when enabling.
        //Restarting to disable is too unstable
        restartZMDOnExit = true;
      }
      else
      {
        core->setServer(UDS_SERVER_ADDY);
      }
    }
  }
}

void ZmdAdvancedConfig::maxDownloadsValueChange(int value)
{
  saveSettings("max-downloads", QString().setNum(maxDownloadsSpinner->value()));
}

void ZmdAdvancedConfig::proxyUrlChange()
{
  saveSettings("proxy-url", (	proxyUrlEdit->text().isEmpty() ?
                              " " :
                              proxyUrlEdit->text()));
  if (proxyUrlEdit->text().isEmpty() == false)
  {
    proxyEnableUserPass();
  }
}

void ZmdAdvancedConfig::proxyEnableUserPass()
{
  if (proxyUrlEdit->text().isEmpty())
  {
    proxyUsernameEdit->setDisabled(true);
    proxyPasswordEdit->setDisabled(true);
  }
  else
  {
    proxyUsernameEdit->setDisabled(false);
    proxyPasswordEdit->setDisabled(false);
  }
}

void ZmdAdvancedConfig::proxyUsernameChange()
{
  saveSettings("proxy-username", (	proxyUsernameEdit->text().isEmpty() ?
                                   " " :
                                   proxyUsernameEdit->text()));
}

void ZmdAdvancedConfig::proxyPasswordChange()
{
  saveSettings("proxy-password", (	proxyPasswordEdit->text().isEmpty() ?
                                   " " :
                                   proxyPasswordEdit->text()));
}

//Error slot
void ZmdAdvancedConfig::errorReady()
{
  QByteArray byteData;
  QString *data;

  if (saveProc != NULL)
    byteData = saveProc->readStderr();
  else  // the else case here is really only going to be true once
    byteData = proc->readStderr();
  data = new QString(byteData);
  if (data->stripWhiteSpace() != "")
  {
    KMessageBox::error(this, *data);
  }
  delete data;
}

ZmdAdvancedConfig::~ZmdAdvancedConfig()
{

  //Make sure someone hasn't change something
  //without pressing enter
  if (hostEdit->isModified() == true)
    hostUrlChange();
  if (proxyUrlEdit->isModified() == true)
    proxyUrlChange();
  if (proxyUsernameEdit->isModified() == true)
    proxyUsernameChange();
  if (proxyPasswordEdit->isModified() == true)
    proxyPasswordChange();

  if (restartZMDOnExit == true)
  {
    proc = new QProcess(QString("rug"), this);
    proc->addArgument("restart");
    proc->start();
    kdWarning() << "Restarting ZMD" << endl;
  }
}

