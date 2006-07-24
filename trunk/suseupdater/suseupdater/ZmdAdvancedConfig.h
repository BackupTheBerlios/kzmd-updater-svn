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

#include <qwidget.h>

class QProcess;
class QComboBox;
class QLabel;
class QHButtonGroup;
class KLineEdit;
class QGridLayout;
class ZmdRugParser;
class ZmdUpdaterCore;

class ZmdAdvancedConfig : public QWidget {

	Q_OBJECT

	public:

		ZmdAdvancedConfig(ZmdUpdaterCore *_core, QWidget *parent=0);
		~ZmdAdvancedConfig();
	
	private slots:

		void stdinReady();		
		void settingsChange();
		void settingsChange(int id);
		void settingsChange(const QString&);
		void errorReady();

	private:
	
		void initGUI();
		
		QGridLayout *mainLayout;

		QLabel *hostLabel;
		KLineEdit *hostEdit;

		QLabel *remoteLabel;
		QHButtonGroup *remoteButtons;

		QLabel *certLabel;
		QHButtonGroup *certButtons;

		QLabel *logLabel;
		QComboBox *logBox;

		QLabel *rollbackLabel;
		QHButtonGroup *rollbackButtons;

		QProcess *proc;
		QProcess *saveProc;
		ZmdRugParser *parser;
		ZmdUpdaterCore *core;
};

#endif

