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

#include "ZmdServerDialog.h"

#include <klocale.h>
#include <klineedit.h>
#include <kpushbutton.h>

#include <qlabel.h>
#include <qstring.h>
#include <qlayout.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>

/** Enum to represent the various types of ZMD Repo **/
enum { 	TYPE_ZYPP=0,  		/** Zypp **/
        TYPE_YUM, 				/** Yum (repo-md) **/
        TYPE_ZEN,					/** ZenWorks **/
        TYPE_MOUNT,				/** Mount (rpm dir) **/
        TYPE_NU,					/** NU **/
        TYPE_RCE, 				/** RCE **/
     };

ZmdServerDialog::ZmdServerDialog(QWidget *parent) :
    QDialog(parent,0,Qt::WShowModal)
{
  initGUI();
}

QValueList<QString> ZmdServerDialog::getServerInfo()
{
  QValueList<QString> list;
  list.append(name);
  list.append(server);
  switch (typeGroup->selectedId())
  {
  case TYPE_ZYPP:
    list.append("zypp");
    break;
  case TYPE_YUM:
    list.append("yum");
    break;
  case TYPE_ZEN:
    list.append("zenworks");
    break;
  case TYPE_MOUNT:
    list.append("mount");
    break;
  case TYPE_NU:
    list.append("nu");
    break;
  case TYPE_RCE:
    list.append("rce");
    break;
  }
  return list;
}

void ZmdServerDialog::initGUI()
{

  layout = new QVBoxLayout(this);
  nameLabel = new QLabel(i18n("Please enter server name below"), this);
  serverLabel = new QLabel(i18n("Please enter server URL below"), this);
  nameEdit = new KLineEdit(this);
  serverEdit = new KLineEdit(this);
  typeGroup = new QButtonGroup(2, Qt::Vertical, i18n("Server Type"), this);
  QRadioButton *zButton = new QRadioButton("ZYPP", typeGroup);
  QRadioButton *yButton = new QRadioButton("YUM", typeGroup);
  QRadioButton *aButton = new QRadioButton("ZENworks", typeGroup);
  QRadioButton *mButton = new QRadioButton("Mount", typeGroup);
  QRadioButton *nButton = new QRadioButton(i18n("NU Server"), typeGroup);
  QRadioButton *rButton = new QRadioButton(i18n("RCE Server"), typeGroup);
  cancelButton = new KPushButton(KStdGuiItem::cancel(), this);
  addButton = new KPushButton(KStdGuiItem::add(), this);

  layout->addWidget(nameLabel, 0, 0);
  layout->addWidget(nameEdit, 0, 0);
  layout->addWidget(serverLabel, 0, 0);
  layout->addWidget(serverEdit, 0, 0);
  layout->addWidget(typeGroup, 0, 0);
  layout->setMargin(10);
  layout->setSpacing(5);

  typeGroup->setExclusive(true);
  typeGroup->insert(zButton, TYPE_ZYPP);
  typeGroup->insert(yButton, TYPE_YUM);
  typeGroup->insert(aButton, TYPE_ZEN);
  typeGroup->insert(mButton, TYPE_MOUNT);
  typeGroup->insert(nButton, TYPE_NU);
  typeGroup->insert(rButton, TYPE_RCE);
  typeGroup->setButton(TYPE_ZYPP);

  buttonLayout = new QHBoxLayout(layout);
  buttonLayout->addWidget(cancelButton, 0, Qt::AlignLeft);
  buttonLayout->addSpacing(100);
  buttonLayout->addWidget(addButton, 0, Qt::AlignRight);
  addButton->setDefault(true);

  connect(addButton, SIGNAL(clicked()),
          this, SLOT(addButtonClicked()));

  connect(cancelButton, SIGNAL(clicked()),
          this, SLOT(cancelButtonClicked()));

  layout->setMargin(20);
  setCaption(i18n("Add Server"));
  show();
}

void ZmdServerDialog::addButtonClicked()
{
  name = nameEdit->text();
  server = serverEdit->text();
  accept();
}

void ZmdServerDialog::cancelButtonClicked()
{
  name = "";
  server = "";
  reject();
}

