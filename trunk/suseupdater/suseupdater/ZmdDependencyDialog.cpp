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

#include "ZmdDependencyDialog.h"

ZmdDependencyDialog::ZmdDependencyDialog(QWidget *parent) : QDialog(parent) {

	initGUI();
}

void ZmdDependencyDialog::initGUI() {

	mainLayout = new QVBoxLayout(this);
	okButton = new KPushButton(i18n("Ok"), this);
	cancelButton = new KPushButton(i18n("Cancel"), this);
	packages = new KTextEdit(this);

	mainLayout->addWidget(packages);
	packages->setReadOnly(true);
	mainLayout->setMargin(10);
	mainLayout->setSpacing(10);

	buttonLayout = new QHBoxLayout(mainLayout);
	buttonLayout->addWidget(okButton,0,Qt::AlignRight);
	buttonLayout->addWidget(cancelButton,0,Qt::AlignLeft);
	connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

void ZmdDependencyDialog::setTitle(QString text) {
	setCaption(text);
}

void ZmdDependencyDialog::setText(QString text) {
	packages->setText(text);
}
