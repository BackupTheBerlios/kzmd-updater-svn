#ifndef _ZMD_RUG_PARSER_H_
#define _ZMD_RUG_PARSER_H_

#include <qobject.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qdict.h>
#include <qvariant.h>

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

		//QStringList getPropertyList();
	
	private:

		void parse();

		QString data;
		QDict<QString> propertyMap;
		QDict<QString> descriptionMap;

		
};

#endif

