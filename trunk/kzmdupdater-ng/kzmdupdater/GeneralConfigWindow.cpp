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

#include <qradiobutton.h>

#include "GeneralConfigWindow.h"
#include "Constants.h"

enum { GCONFIG_ZMD = 0, GCONFIG_ZYPP };

GeneralConfigWindow::GeneralConfigWindow() : QWidget(NULL, NULL, Qt::WDestructiveClose) {

	config = kapp->config();
	config->setGroup("General");
	initGUI();
	readConfig();
}

void GeneralConfigWindow::initGUI() {

	mainLayout = new QVBoxLayout(this);
	header = new HeaderWidget(this);
	intervalSpin = new QSpinBox(this);
	backendGroup = new QVButtonGroup(i18n("Select Your Preferred Updater"), this);
	cancelButton = new KPushButton(i18n("Cancel"), this);
	okButton = new KPushButton(i18n("Okay"), this);

	header->setDescription(i18n("<b>Configure The Updater:</b><br> Below you can select the updater you would  like to use and when we should check for updates<br><u>This program needs to be restarted before new settings can take affect</u>"));
	mainLayout->addWidget(header, false, 0);

	intervalSpin->setPrefix(i18n("Check For Updates Every "));
	intervalSpin->setSuffix(i18n(" Minutes"));
	intervalSpin->setMinValue(0);
	mainLayout->addWidget(intervalSpin, false, 0);

	QRadioButton *zmdButton = new QRadioButton(i18n("ZMD Updater"), backendGroup);
//	QRadioButton *zyppButton = new QRadioButton(i18n("ZYPP Updater"), backendGroup);
	backendGroup->setRadioButtonExclusive(true);
	mainLayout->addWidget(backendGroup, false, 0);

	buttonLayout = new QHBoxLayout(mainLayout);
	buttonLayout->addWidget(cancelButton, false, Qt::AlignLeft);
	buttonLayout->addWidget(okButton, false, Qt::AlignRight);

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

	switch(config->readEntry("Backend").toInt()) {
		case BACKEND_ZMD:
			backendGroup->setButton(GCONFIG_ZMD);
			break;
		default:
			backendGroup->setButton(GCONFIG_ZMD);
			break;
	}
}

void GeneralConfigWindow::okButtonClicked() {

	int backend;

	config->setGroup("General");

	switch (backendGroup->selectedId()) {
	case GCONFIG_ZMD:
		backend = BACKEND_ZMD;
		break;
	case GCONFIG_ZYPP:
//		backend = BACKEND_ZYPP;
		backend = BACKEND_ZMD;
		break;
	}

	config->writeEntry("Backend", backend);
	config->writeEntry("Interval", intervalSpin->value());
	close();
}
