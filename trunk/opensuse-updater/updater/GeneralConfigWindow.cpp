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

#include <klocale.h>
#include <kconfig.h>
#include <kapp.h>
#include <kpushbutton.h>

#include <qlayout.h>
#include <qvbuttongroup.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qradiobutton.h>

#include "HeaderWidget.h"
#include "GeneralConfigWindow.h"
#include "Constants.h"

enum { GCONFIG_ZMD = 0, GCONFIG_ZYPP };

GeneralConfigWindow::GeneralConfigWindow() : QWidget(NULL, NULL, Qt::WDestructiveClose) {

	config = KGlobal::config();
	config->setGroup("General");
	initGUI();
	readConfig();
}

void GeneralConfigWindow::initGUI() {

	mainLayout = new QVBoxLayout(this);
	header = new HeaderWidget(this);
	intervalSpin = new QSpinBox(this);
	backendGroup = new QVButtonGroup(i18n("Get updates via:"), this);
	cancelButton = new KPushButton(KStdGuiItem::cancel(), this);
	okButton = new KPushButton(KStdGuiItem::ok(), this);
	autostartButton = new QCheckBox(i18n("Automatically start updater on login"), this);

	header->setDescription(i18n("<b>Configure The Updater:</b><br> Below you can select the updater you would  like to use and when we should check for updates<br><u>After switching backends, this applet must be restarted</u>"));
	mainLayout->addWidget(header, false, 0);

	intervalSpin->setPrefix(i18n("Check For Updates Every "));
	intervalSpin->setSuffix(i18n(" Minutes"));
	intervalSpin->setMinValue(0);
	mainLayout->addWidget(intervalSpin, false, 0);

	mainLayout->addWidget(autostartButton, false, 0);

	QRadioButton *zmdButton = new QRadioButton(i18n("Novell ZenWorks"), backendGroup);
  QRadioButton *zyppButton = new QRadioButton(i18n("Default"), backendGroup);
//	QRadioButton *zyppButton = new QRadioButton(i18n("ZYPP Updater"), backendGroup);
	backendGroup->setRadioButtonExclusive(true);
	mainLayout->addWidget(backendGroup, false, 0);

	buttonLayout = new QHBoxLayout(mainLayout);
	buttonLayout->addWidget(okButton, false, Qt::AlignLeft);
	buttonLayout->addWidget(cancelButton, false, Qt::AlignRight);

	connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(okButton, SIGNAL(clicked()), this, SLOT(okButtonClicked()));

	mainLayout->setSpacing(10);
	mainLayout->setMargin(10);
	setCaption(i18n("Configuration"));
}

void GeneralConfigWindow::readConfig() {

	intervalSpin->setValue((config->readEntry("Interval").toInt()));
	if (intervalSpin->value() <= 0)
		intervalSpin->setValue(15);

	QString backend = config->readEntry("Backend");
  
  if ( backend == "zmd" )
  {
    backendGroup->setButton(GCONFIG_ZMD);
  }
  if ( backend == "zypp" )
  {
    backendGroup->setButton(GCONFIG_ZYPP);
  }
  else
  {
    backendGroup->setButton(GCONFIG_ZYPP);
  }

	switch(config->readEntry("Autostart") == "true") {

		case true:
			autostartButton->setChecked(true);
			break;
		case false:
			autostartButton->setChecked(false);
			break;
		default:
			autostartButton->setChecked(false);
			break;
	}
}

void GeneralConfigWindow::okButtonClicked() {

	config->setGroup("General");

	switch (backendGroup->selectedId()) {
	case GCONFIG_ZMD:
		config->writeEntry("Backend", "zmd");
		break;
	case GCONFIG_ZYPP:
    config->writeEntry("Backend", "zypp");
    break;
  default:
		config->writeEntry("Backend", "zypp");
		break;
	}
  
	config->writeEntry("Interval", intervalSpin->value());
	config->writeEntry("Autostart", autostartButton->isChecked());
	emit(configChanged());
	close();
}

