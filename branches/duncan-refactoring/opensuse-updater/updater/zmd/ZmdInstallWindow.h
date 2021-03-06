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

#ifndef _ZMD_INSTALL_WINDOW_
#define _ZMD_INSTALL_WINDOW_

#include <qwidget.h>
#include "ZmdUpdaterCore.h"

/**
	@file

	Defines our installation status window

	@author Narayan Newton <narayannewton@gmail.com>
**/

class KProgress;
class KPushButton;
class KTextEdit;
class QListView;
class QLabel;
class QVBoxLayout;
class HeaderWidget;

/**
	The installation status window, shows download
	and transaction progress.
**/
class ZmdInstallWindow : public QWidget
{

  Q_OBJECT

public:

  ZmdInstallWindow(ZmdUpdaterCore *_core, QWidget *parent=0);

  void setPackageList(QValueList<Package> installs,
                      QValueList<Package> updates,
                      QValueList<Package> removals);

  void startUpdate();

signals:

  //We fire this to refresh the update list after
  //we install some packages/patches
  void refreshUpdates();

private slots:

  void abortButtonClicked();
  void closeEvent(QCloseEvent*);
  //Core Signals

  void progress(Progress);
  void download(Progress);
  void finished(int, QString);
  void gotDepInfo(QValueList<Package>,
                  QValueList<Package>,
                  QValueList<Package>);
  void generalFault(QString, int);

private:

  void initGUI();
  void closeWindow();

  HeaderWidget *header;
  KTextEdit *transactionList;
  KProgress *progressBar;
  KPushButton *abortButton;

  QVBoxLayout *mainLayout;

  ZmdUpdaterCore	*core;

  QValueList<Package>	installList;
  QValueList<Package>	updateList;
  QValueList<Package>	removeList;

  //temp variables for the two progress functions
  bool watchingDownload;
  bool downloadDone;
  bool watchingPackage;
  bool packageDone;

  //variable to decide if we should really close the window
  bool reallyDone;

  //Here we hold the ID of the last error
  int lastError;
};


#endif

