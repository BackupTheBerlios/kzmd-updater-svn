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

#include "FeedDialog.h"

#include <klocale.h>
#include <kpushbutton.h>
#include <klineedit.h>

#include <qlabel.h>
#include <qlayout.h>

FeedDialog::FeedDialog(QWidget *parent) : QDialog(parent) {
	setModal(true);
	initGUI();
}

void FeedDialog::initGUI() {
	layout = new QGridLayout(this);
	feedLabel = new QLabel(i18n("URL Of RSS Feed: "), this);
	feedEdit = new KLineEdit(this);
	okButton = new KPushButton(KStdGuiItem::ok(), this);
	cancelButton = new KPushButton(KStdGuiItem::cancel(), this);

	connect(okButton, SIGNAL(clicked()), this, SLOT(okButtonClicked()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelButtonClicked()));

	layout->addMultiCellWidget(feedLabel, 0, 0, 0, 1);
	layout->addMultiCellWidget(feedEdit, 1, 1, 0, 1);
	layout->addWidget(okButton, 2, 0);
	layout->addWidget(cancelButton, 2, 1);
	layout->setSpacing(10);
	layout->setMargin(10);
	setCaption(i18n("Add Package Server Feed"));
	show();
}

QString FeedDialog::url() {
	return feedUrl;
}

void FeedDialog::okButtonClicked() {
	feedUrl = feedEdit->text();
	accept();
}
	
void FeedDialog::cancelButtonClicked() {
	feedUrl = "";
	reject();
}

