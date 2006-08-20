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

#ifndef _ZMD_RUG_PARSER_H_
#define _ZMD_RUG_PARSER_H_

#include <qobject.h>
#include <qdict.h>

/**
	@file

	Defines ZmdRugParser, our options parser

	@author Narayan Newton <narayannewton@gmail.com>
**/

class QVariant;
class QString;
class QStringList;

/**
	Class that parses the preferences output from rug. Stores the properties and
	allows you to fetch them from the class.
**/
class ZmdRugParser : public QObject {

	Q_OBJECT

	public:

		ZmdRugParser(QObject *parent=0);
		ZmdRugParser(const QString rugOutput, QObject *parent=0);

		void setData(const QString);

		QString getProperty(QString);
		QString getDescription(QString);

	private:

		void parse();

		QString data;
		QDict<QString> propertyMap;
		QDict<QString> descriptionMap;
};

#endif

