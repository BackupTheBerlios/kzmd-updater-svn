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

#include "ZmdCatalogListItem.h"
#include "ZmdConfigWindow.h" 

ZmdCatalogListItem::ZmdCatalogListItem(QListViewItem *parent, const QString &text, ZmdUpdaterCore *_core) : QCheckListItem(parent, text, QCheckListItem::CheckBoxController), core(_core) {

	setTristate(false);
}

void ZmdCatalogListItem::stateChange(bool state) {
	Catalog cat;

	cat.name = text(CONFW_NAME);
	cat.id = text(CONFW_ID);

	if (cat.id == "")
		return;

	switch (this->state()) {
		
		case QCheckListItem::Off:
		case QCheckListItem::NoChange:
			setOn(false);
			core->unsubscribeCatalog(cat);
			break;
		case QCheckListItem::On:
			core->subscribeCatalog(cat);
			break;
	}
}

