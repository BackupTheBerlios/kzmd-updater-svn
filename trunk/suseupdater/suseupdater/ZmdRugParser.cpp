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

