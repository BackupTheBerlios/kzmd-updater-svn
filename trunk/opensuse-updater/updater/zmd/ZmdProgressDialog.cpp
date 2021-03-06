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

#include "ZmdProgressDialog.h"

#include <klocale.h>
#include <kprogress.h>

#include <qlayout.h>
#include <qlabel.h>

ZmdProgressDialog::ZmdProgressDialog(bool progressGUI, QWidget *parent) :
    QDialog(parent)
{

  if (progressGUI)
    initProgressGUI();
  else
    initTextGUI();
  setModal(true);

  //Don't close on closeEvent
  reallyDone = false;
}

void ZmdProgressDialog::initProgressGUI()
{
  mainLayout = new QVBoxLayout(this);
  progressBar = new KProgress(this);
  description = new QLabel(this);

  mainLayout->addWidget(description,0,0);
  mainLayout->addWidget(progressBar,0,0);
  mainLayout->setSpacing(10);
  mainLayout->setMargin(10);
  resize(300,100);
}

void ZmdProgressDialog::closeEvent(QCloseEvent *e)
{
  if (reallyDone)
    e->accept();
  else
    e->ignore();
}

void ZmdProgressDialog::initTextGUI()
{
  mainLayout = new QVBoxLayout(this);
  description = new QLabel(this);

  mainLayout->addWidget(description,0,0);
  mainLayout->setMargin(10);
  resize(200,100);
}

void ZmdProgressDialog::setTitle(QString title)
{
  setCaption(title);
}

void ZmdProgressDialog::setDescription(QString text)
{
  description->setText(text);
}

void ZmdProgressDialog::progress(Progress prog)
{
  progressBar->setValue((int)prog.percent);
}

void ZmdProgressDialog::finished(QString name, int status, QString error)
{
  reallyDone = true;
  close();
}

