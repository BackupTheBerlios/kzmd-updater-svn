/* 
   Copyright (C) 2006 Benjamin Weber <benji.weber@gmail.com>

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

#include <qwidget.h>
#include <kpassivepopup.h>
#include <qpointarray.h>
#include <qpushbutton.h>
#include <qtooltip.h>
#include <qlayout.h>
#include <qtimer.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qbitmap.h>

#include "MainWindow.h"

#ifndef OSUPDATERPOPUP_H
#define OSUPDATERPOPUP_H

class OSUpdaterPopup : public KPassivePopup
{
	Q_OBJECT
	public:
		OSUpdaterPopup(QWidget *parent,MainWindow *mainWindow);
		void setUpdates(int newUpdateValue);
	protected:
		void	paintEvent( QPaintEvent *e );
	private:
		int _updates;
		QVBoxLayout *_popupLayout;
		QHBoxLayout *_titleLayout;
		QHBoxLayout *_descriptionLayout;
		QLabel *_title;
		QPushButton *_closeButton;
		QLabel *_description;
		QHBoxLayout *_buttonLayout;
		QPushButton *_installButton;
		QPushButton *_cancelButton;
		QLabel * _iconLabel;
		QBitmap _mask;
};

#endif
