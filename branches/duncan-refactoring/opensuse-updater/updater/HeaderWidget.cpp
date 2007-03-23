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

#include "HeaderWidget.h"

#include <kiconloader.h>

#include <qlabel.h>
#include <qlayout.h>

HeaderWidget::HeaderWidget(QWidget *parent) : QWidget(parent) {

	layout = new QHBoxLayout(this);
	suseLogo = new QLabel(this);
	text = new QLabel(this);
	pic = new QPixmap(UserIcon("suse_logo.png"));

	pic->setOptimization(QPixmap::BestOptim);
	suseLogo->setPixmap(*pic);

	layout->addWidget(suseLogo, false, Qt::AlignLeft);
	layout->addWidget(text, true, Qt::AlignRight);
	layout->insertSpacing(1, 30);

	text->setText("<b>Default</b><br>");
	text->setMinimumWidth(400);
	show();
}

void HeaderWidget::setDescription(QString description) {
	text->setText(description);
}

HeaderWidget::~HeaderWidget() {
	delete pic;
}

#include "HeaderWidget.moc"

