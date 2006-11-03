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

#ifndef _ZMD_DEPENDENCY_DIALOG_H_
#define _ZMD_DEPENDENCY_DIALOG_H_

#include <qdialog.h>

/**
	@file

	Defines our dependency dialog

	@author Narayan Newton <narayannewton@gmail.com>
**/

class KPushButton;
class KTextEdit;
class QLabel;
class QVBoxLayout;
class QHBoxLayout;

/**
	A window that prompts the user to approve a dep resolve.
	This is the last chance for a user to stop a transaction.
**/
class ZmdDependencyDialog : public QDialog
{

  Q_OBJECT

public:

  ZmdDependencyDialog(QWidget *parent=0);

  void setTitle(QString);
  void setText(QString);

private:

  void initGUI();

  QVBoxLayout *mainLayout;
  QHBoxLayout *buttonLayout;

  KPushButton *okButton;
  KPushButton *cancelButton;
  KTextEdit *packages;
};

#endif

