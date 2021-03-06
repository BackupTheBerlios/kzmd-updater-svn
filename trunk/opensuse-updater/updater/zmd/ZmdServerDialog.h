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

#ifndef _ZMD_SERVER_DIALOG_H_
#define _ZMD_SERVER_DIALOG_H_

#include <qdialog.h>

/**
	@file

	Defines our ZmdServerDialog

	@author Narayan Newton <narayannewton@gmail.com>
**/

class KLineEdit;
class KPushButton;
class QLabel;
class QString;
class QVBoxLayout;
class QHBoxLayout;
class QButtonGroup;

/**
	Our add/remove server dialog, fetches name, uri and type
	from the user.
**/
class ZmdServerDialog : public QDialog
{

  Q_OBJECT

public:

  ZmdServerDialog(QWidget *parent=0);

  QValueList<QString> getServerInfo();

private slots:

  void addButtonClicked();
  void cancelButtonClicked();

private:

  void initGUI();

  QVBoxLayout	*layout;
  QHBoxLayout	*buttonLayout;

  QLabel *nameLabel;
  QLabel *serverLabel;
  KLineEdit *nameEdit;
  KLineEdit *serverEdit;
  KPushButton *cancelButton;
  KPushButton *addButton;
  QButtonGroup *typeGroup;

  QString server;
  QString name;
};

#endif

