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

#include "ZmdRugParser.h"

#include <kdebug.h>

#include <qstring.h>
#include <qstringlist.h>
#include <qdict.h>
#include <qvariant.h>

ZmdRugParser::ZmdRugParser(QObject *parent) : QObject(parent) {
	propertyMap.setAutoDelete(true);
	descriptionMap.setAutoDelete(true);
}

ZmdRugParser::ZmdRugParser(const QString rugOutput, QObject *parent) : QObject(parent) {
	data = rugOutput;
	parse();
}

void ZmdRugParser::setData(const QString rugOutput) {
	data = rugOutput;
	parse();
}

void ZmdRugParser::parse() {
	if (data.isEmpty() == true)
		return;

	data = data.remove(0,data.find("bind-ip"));
	data = data.replace('\n', "|");
	data = data.simplifyWhiteSpace();
	QStringList props;
	props = QStringList::split("|", data);
	
	for (QStringList::iterator iter = props.begin(); iter != props.end(); iter++) {
		QString name = *(iter);
		QString value = *(++iter);
		QString description = *(++iter);

		propertyMap.insert(name.stripWhiteSpace(), new QString(value.stripWhiteSpace()));
		descriptionMap.insert(name.stripWhiteSpace(), new QString(description.simplifyWhiteSpace()));
	}
}

QString ZmdRugParser::getProperty(QString name) {
	QString *data;

	data = propertyMap[name];
	if (data == NULL)
		return "";
	else
		return *data;
}

QString ZmdRugParser::getDescription(QString name) {
	QString *data;

	data = descriptionMap[name];
	if (data == NULL) 
		return "";
	else
		return *data;
}

