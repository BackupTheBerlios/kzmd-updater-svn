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

#ifndef _FEED_DIALOG_H_
#define _FEED_DIALOG_H_

#include <qdialog.h>

class QDialog;
class QLabel;
class QGridLayout;
class QString;
class KLineEdit;
class KPushButton;

class FeedDialog : public QDialog {

	Q_OBJECT

	public:

		FeedDialog(QWidget *parent=0);

		QString url();

	private slots:

		void okButtonClicked();
		void cancelButtonClicked();

	private:

		void initGUI();

		QGridLayout *layout;
		QLabel *feedLabel;
		KLineEdit *feedEdit;
		KPushButton *okButton;
		KPushButton *cancelButton;

		QString feedUrl;
};

#endif
