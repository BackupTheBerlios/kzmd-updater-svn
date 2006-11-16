/**************************************************************************
*   Copyright (C) 2006 by Novell Inc.                                     *
*   Copyright (C) 2006 by Narayan Newton <narayannewton@gmail.com>        *
*   Copyright (C) 2003 - 2004 by Frerich Raabe <raabe@kde.org>            *
*                                Tobias Koenig <tokoe@kde.org>            *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <kdebug.h>
#include <klocale.h>
#include <kmdcodec.h>

#include <qvariant.h>
#include <qdom.h>

#include "kxmlrpcquery.h"

using namespace KNetwork;

/**

	@file

	Implementation of KXmlRpcResult and KXmlRpcQuery

**/
//small macro taken from HTTP IOSlave
#define KIO_ARGS QByteArray packedArgs; QDataStream kioArgsStream( packedArgs, IO_WriteOnly ); kioArgsStream

#define ZMD_UNIX_SOCKET "/var/run/zmd/zmd-web.socket"
#define BUFFER_SIZE 6000

KXmlRpcQuery *KXmlRpcQuery::create( const QVariant &id, QObject *parent, const char *name ) {

  return new KXmlRpcQuery( id, parent, name );
}

KXmlRpcQuery::KXmlRpcQuery( const QVariant &id, QObject *parent, const char *name )
  : QObject( parent, name ), m_id( id )
{
 
}

void KXmlRpcQuery::call( const QString &server, const QString &method,
                  const QValueList<QVariant> &args, const QString &userAgent )
{

 struct sockaddr_un serveraddr;
 struct sockaddr_un clientaddr;
 
 int    sd=-1, rc, bytesReceived;
 char   buffer[BUFFER_SIZE];
   
 QString xmlMarkup = markupCall( method, args );
 
 QByteArray postData;
 QDataStream stream( postData, IO_WriteOnly );
 stream.writeRawBytes( xmlMarkup.utf8(), xmlMarkup.utf8().length() );
 
 sd = socket(AF_UNIX, SOCK_STREAM, 0);
 if (sd < 0)
 {
    kdError() << "Can't create unix socket." << endl;
    return;
 }
 
 kdDebug() << "socket created" << endl;
 
 memset(&serveraddr, 0, sizeof(serveraddr));
 serveraddr.sun_family = AF_UNIX;
 strcpy(serveraddr.sun_path, ZMD_UNIX_SOCKET);
 
 rc = ::connect(sd, (struct sockaddr *)&serveraddr, SUN_LEN(&serveraddr));
 if (rc < 0)
 {
    kdError() << "Can't connect unix socket." << endl;
    return;
 }
 kdDebug() << "socket connected" << endl;
 
 rc = send(sd, postData.data(), postData.size(), 0);
 if (rc < 0)
 {
    kdError() << "Can't send." << endl;
    return;
 }
 kdDebug() << "sent " << rc << " bytes" << endl;
 
 bytesReceived = 0;
 
 shutdown(sd,1);
 
 //rc = recv(sd, & buffer[bytesReceived], BUFFER_SIZE - bytesReceived, 0);
 rc = read(sd, & buffer[bytesReceived], BUFFER_SIZE - bytesReceived );
 
 if (rc < 0)
 {
   kdError() << "Can't receive" << endl;
   return;
 }
 else if (rc == 0)
 {
   kdError() << "Server closed connection" << endl;
   return;
 }
 bytesReceived += rc;
  
 kdDebug() << "Received " << bytesReceived << " bytes" << endl;
 
 if (sd != -1)
  close(sd);
  
 QString data_s = QString::fromUtf8( buffer, bytesReceived );

  kdDebug() << "got: " << endl << data_s << endl;
	QDomDocument doc;
	QString errMsg;
	int errLine, errCol;
	if ( !doc.setContent( data_s, false, &errMsg, &errLine, &errCol  ) )
  {
		emit fault( -1, i18n( "Received invalid XML markup: %1 at %2:%3" ).arg( errMsg ).arg( errLine ).arg( errCol ), m_id );
		emit finished( this );
		return ;
  }
  
  if ( isMessageResponse( doc ) )
		emit message( parseMessageResponse( doc ).data(), m_id );
	else if ( isFaultResponse( doc ) ) {
		emit fault( parseFaultResponse( doc ).errorCode(), parseFaultResponse( doc ).errorString(), m_id );
	} else {
		emit fault( 1, i18n( "Unknown type of XML markup received" ), m_id );
	}

  emit finished( this );
}

bool KXmlRpcQuery::isMessageResponse( const QDomDocument &doc ) const {

	return doc.documentElement().firstChild().toElement().tagName().lower() == "params";
}

KXmlRpcResult KXmlRpcQuery::parseMessageResponse( const QDomDocument &doc ) const {

	KXmlRpcResult response;
	response.m_success = true;

	QDomNode paramNode = doc.documentElement().firstChild().firstChild();
	while ( !paramNode.isNull() ) {

		response.m_data << demarshal( paramNode.firstChild().toElement() );
		paramNode = paramNode.nextSibling();
	}
	return response;
}

bool KXmlRpcQuery::isFaultResponse( const QDomDocument &doc ) const {

	return doc.documentElement().firstChild().toElement().tagName().lower() == "fault";
}

KXmlRpcResult KXmlRpcQuery::parseFaultResponse( const QDomDocument &doc ) const {

	KXmlRpcResult response;
	response.m_success = false;

	QDomNode errorNode = doc.documentElement().firstChild().firstChild();
	const QVariant errorVariant = demarshal( errorNode.toElement() );
	response.m_errorCode = errorVariant.toMap() [ "faultCode" ].toInt();
	response.m_errorString = errorVariant.toMap() [ "faultString" ].toString();

	return response;
}

QString KXmlRpcQuery::markupCall( const QString &cmd, const QValueList<QVariant> &args ) const {

	QString markup = "<?xml version=\"1.0\" ?>\r\n<methodCall>\r\n";

	markup += "<methodName>" + cmd + "</methodName>\r\n";

	if ( !args.isEmpty() ) {

		markup += "<params>\r\n";
		QValueList<QVariant>::ConstIterator it = args.begin();
		QValueList<QVariant>::ConstIterator end = args.end();
		for ( ; it != end; ++it )
			markup += "<param>\r\n" + marshal( *it ) + "</param>\r\n";
		markup += "</params>\r\n";
	}

	markup += "</methodCall>\r\n";

	return markup;
}

QString KXmlRpcQuery::marshal( const QVariant &arg ) const {

	switch ( arg.type() ) {

		case QVariant::String:
		case QVariant::CString:
			return "<value><string>" + arg.toString() + "</string></value>\r\n";
		case QVariant::Int:
			return "<value><int>" + QString::number( arg.toInt() ) + "</int></value>\r\n";
		case QVariant::Double:
			return "<value><double>" + QString::number( arg.toDouble() ) + "</double></value>\r\n";
		case QVariant::Bool:
			{
				QString markup = "<value><boolean>";
				markup += arg.toBool() ? "1" : "0";
				markup += "</boolean></value>\r\n";
				return markup;
			}
		case QVariant::ByteArray:
			return "<value><base64>" + KCodecs::base64Encode( arg.toByteArray() ) + "</base64></value>\r\n";
		case QVariant::DateTime:
			return "<value><datetime.iso8601>" + arg.toDateTime().toString( Qt::ISODate ) + "</datetime.iso8601></value>\r\n";
		case QVariant::List:
			{
				QString markup = "<value><array><data>\r\n";
				const QValueList<QVariant> args = arg.toList();
				QValueList<QVariant>::ConstIterator it = args.begin();
				QValueList<QVariant>::ConstIterator end = args.end();
				for ( ; it != end; ++it )
					markup += marshal( *it );
				markup += "</data></array></value>\r\n";
				return markup;
			}
		case QVariant::Map:
			{
				QString markup = "<value><struct>\r\n";
				QMap<QString, QVariant> map = arg.toMap();
				QMap<QString, QVariant>::ConstIterator it = map.begin();
				QMap<QString, QVariant>::ConstIterator end = map.end();
				for ( ; it != end; ++it ) {
					markup += "<member>\r\n";
					markup += "<name>" + it.key() + "</name>\r\n";
					markup += marshal( it.data() );
					markup += "</member>\r\n";
				}
				markup += "</struct></value>\r\n";
				return markup;
			}
		default:
			kdWarning() << "Failed to marshal unknown variant type: " << arg.type() << endl;
	};
	return QString::null;
}

QVariant KXmlRpcQuery::demarshal( const QDomElement &elem ) const {

	Q_ASSERT( elem.tagName().lower() == "value" );

	const QDomElement typeElement = elem.firstChild().toElement();
	const QString typeName = typeElement.tagName().lower();

	if ( typeName == "string" )
		return QVariant( typeElement.text() );
	else if ( typeName == "i4" || typeName == "int" )
		return QVariant( typeElement.text().toInt() );
	else if ( typeName == "double" )
		return QVariant( typeElement.text().toDouble() );
	else if ( typeName == "boolean" ) {

	 if ( typeElement.text().lower() == "true" || typeElement.text() == "1" )
		return QVariant( true );
	else
		return QVariant( false );
	}
	else if ( typeName == "base64" )
		return QVariant( KCodecs::base64Decode( typeElement.text().latin1() ) );
	else if ( typeName == "datetime" || typeName == "datetime.iso8601" )
		return QVariant( QDateTime::fromString( typeElement.text(), Qt::ISODate ) );
	else if ( typeName == "array" ) {
		QValueList<QVariant> values;
		QDomNode valueNode = typeElement.firstChild().firstChild();
		while ( !valueNode.isNull() ) {
			values << demarshal( valueNode.toElement() );
			valueNode = valueNode.nextSibling();
		}
	return QVariant( values );
	} else if ( typeName == "struct" ) {

		QMap<QString, QVariant> map;
		QDomNode memberNode = typeElement.firstChild();
		while ( !memberNode.isNull() ) {
			const QString key = memberNode.toElement().elementsByTagName( "name" ).item( 0 ).toElement().text();
			const QVariant data = demarshal( memberNode.toElement().elementsByTagName( "value" ).item( 0 ).toElement() );
			map[ key ] = data;
			memberNode = memberNode.nextSibling();
		}
		return QVariant( map );
	} else {
		kdWarning() << "Cannot demarshal unknown type " << typeName << endl;
	}
	return QVariant();
}

KXmlRpcQuery::~KXmlRpcQuery()
{
}
