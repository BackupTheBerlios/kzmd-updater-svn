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

#ifndef _ZMD_ADVANCED_CONFIG_H_
#define _ZMD_ADVANCED_CONFIG_H_

#include <qwidget.h>

/**
	@file

	Defines the widget that allows for user configuration of zmd

	@author Narayan Newton <narayannewton@gmail.com>
**/

class QProcess;
class QComboBox;
class QLabel;
class QHButtonGroup;
class KLineEdit;
class QGridLayout;
class QVGroupBox;
class QSpinBox;
class ZmdRugParser;
class ZmdUpdaterCore;

/**
	A widget that gives users control over security, connection,
	proxy and download options for ZMD.
**/
class ZmdAdvancedConfig : public QWidget
{

  Q_OBJECT

public:

  ZmdAdvancedConfig(ZmdUpdaterCore *_core, QWidget *parent=0);
  ~ZmdAdvancedConfig();

private slots:

  void stdinReady();
  void radioButtonsChange(int id);
  void hostUrlChange();
  void logLevelChange(const QString&);
  void securityLevelChange(const QString&);
  void maxDownloadsValueChange(int);
  void proxyUrlChange();
  void proxyEnableUserPass();
  void proxyUsernameChange();
  void proxyPasswordChange();
  void errorReady();

private:

  void initGUI();
  void saveSettings(QString setting, QString value);

  QGridLayout *mainLayout;
  QVGroupBox *securityBox;
  QVGroupBox *connectionBox;
  QVGroupBox *proxyBox;
  QVGroupBox *otherBox;

  QLabel *hostLabel;
  KLineEdit *hostEdit;

  QLabel *remoteLabel;
  QHButtonGroup *remoteButtons;

  QLabel *securityLevelLabel;
  QComboBox *securityLevelBox;

  QLabel *certLabel;
  QHButtonGroup *certButtons;

  QLabel *logLabel;
  QComboBox *logBox;

  QLabel *rollbackLabel;
  QHButtonGroup *rollbackButtons;

  QLabel *maxDownloadsLabel;
  QSpinBox *maxDownloadsSpinner;

  QLabel *proxyUrlLabel;
  KLineEdit *proxyUrlEdit;

  QLabel *proxyUsernameLabel;
  KLineEdit *proxyUsernameEdit;

  QLabel *proxyPasswordLabel;
  KLineEdit *proxyPasswordEdit;

  //proc to fetch current settings
  QProcess *proc;
  //proc to save settings
  QProcess *saveProc;

  //Our  parser for actually "reading" the settings
  ZmdRugParser *parser;

  //backend and variable to watch for required backend restart.
  ZmdUpdaterCore *core;
  bool restartZMDOnExit;
};

#endif

