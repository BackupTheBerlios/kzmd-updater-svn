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

#ifndef _UPDATE_LIST_ITEM_H_
#define _UPDATE_LIST_ITEM_H_

#include <qlistview.h>

/**
	@file

	Defines UpdateListItem.
**/

/**
	UpdateListItem basically just allows for special "refcounting."
	It keeps track of user clicks and increments/decrements a counter

	@author Narayan Newton <narayannewton@gmail.com>
**/
class UpdateListItem : public QListViewItem {

	public:

		UpdateListItem(QListView *, const QString& );

	
	private:

};

#endif

