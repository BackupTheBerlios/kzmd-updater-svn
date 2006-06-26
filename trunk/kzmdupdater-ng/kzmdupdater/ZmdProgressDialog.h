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

#ifndef _ZMD_PROGRESS_DIALOG_H_
#define _ZMD_PROGRESS_DIALOG_H_

#include <klocale.h>
#include <kprogress.h>

#include <qdialog.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qstring.h>

#include "ZmdUpdaterCore.h"

/*

	A "custom" progress dialog. It can either show a progress bar and respond
	to a progress signal sent from updater core or it can show a text message
	and wait for a finished signal from updater core. In either mode, we are
	going to be modal.

*/

class ZmdProgressDialog : public QDialog {

	Q_OBJECT

	public:

		//first argument true for a progress bar, false for text only.
		ZmdProgressDialog(bool progressGUI, QWidget *parent=0);
		~ZmdProgressDialog();

		void setTitle(QString);
		void setDescription(QString);
		void connectProgress(const QObject *object, const char *member);
		void connectFinished(const QObject *object, const char *member);

	public slots:
		
		void progress(Progress);
		void finished(QString,int);

	private slots:

		void closeEvent(QCloseEvent *e);

	private:

		void initProgressGUI();
		void initTextGUI();

		QVBoxLayout *mainLayout;
		KProgress *progressBar;
		QLabel *description;

		//Mark if we should really exit
		bool reallyDone;
};

#endif
