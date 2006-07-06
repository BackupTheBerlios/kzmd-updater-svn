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

#ifndef _ZMD_ADVANCED_CONFIG_H_
#define _ZMD_ADVANCED_CONFIG_H_

#include <klineedit.h>
#include <klocale.h>

#include <qwidget.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qhbuttongroup.h>

class QProcess;
class QComboBox;
class ZmdRugParser;

class ZmdAdvancedConfig : public QWidget {

	Q_OBJECT

	public:

		ZmdAdvancedConfig(QWidget *parent=0);
		~ZmdAdvancedConfig();

	private slots:

		void stdinReady();

	private:
	
		void initGUI();
		
		QGridLayout *mainLayout;

		QLabel *hostLabel;
		KLineEdit *hostEdit;
		QString oldHost;

		QLabel *remoteLabel;
		QHButtonGroup *remoteButtons;
		bool oldRemote;

		QLabel *certLabel;
		QHButtonGroup *certButtons;
		bool oldCert;

		QLabel *logLabel;
		QComboBox *logBox;
		QString oldLog;

		QLabel *rollbackLabel;
		QHButtonGroup *rollbackButtons;
		bool oldRollback;

		QProcess *proc;
		ZmdRugParser *parser;

};

#endif

