/**************************************************************************
*   Copyright (C) 2006 by Narayan Newton <narayannewton@gmail.com>        *
*   Copyright (C) 2003 - 2004 by Frerich Raabe <raabe@kde.org>            *
*                                Tobias Koenig <tokoe@kde.org>            *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/

#ifndef _KXML_RPC_QUERY_H_
#define _KXML_RPC_QUERY_H_

#include <kbufferedsocket.h>
#include <kio/job.h>

#include <qobject.h>
#include <qvaluelist.h>
#include <qvariant.h>

//pre-decls
class QString;
class QDomDocument;
class QDomElement;
class KNetwork::KBufferedSocket;

/**

	@file

	This file defines KXmlRpcResult and KXmlRpcQuery, our internal classes

**/


/**
	KXmlRpcResult is an internal class that represents a response from the XML-RPC 
	server. This is an internal class and is only used by KXmlRpcQuery

**/

class KXmlRpcResult {

	friend class KXmlRpcQuery;

	public:
		
		KXmlRpcResult() {}

		bool success() const {
			return m_success;
		}


		int errorCode() const {
			return m_errorCode;
		}

		QString errorString() const {
			return m_errorString;
		}

		QValueList<QVariant> data() const {
			return m_data;
		}

	private:

		bool m_success;
		int m_errorCode;
		QString m_errorString;
		QValueList<QVariant> m_data;
};

/** 
			KXmlRpcQuery is a class that represents an individual XML-RPC call.
			This is an internal class and is only used by the Server class.
 
**/

class KXmlRpcQuery : public QObject {

	Q_OBJECT

	public:

		static KXmlRpcQuery *create( const QVariant &id = QVariant(),
							QObject *parent = 0, const char *name = 0 );

	public slots:

		void call( const QString &server, const QString &method,
				const QValueList<QVariant> &args = QValueList<QVariant>(),
				const QString &userAgent = "KDE-XMLRPC" );

	signals:

		void message( const QValueList<QVariant> &result, const QVariant &id );
		void fault( int, const QString&, const QVariant &id );
		void finished( KXmlRpcQuery* );

	private:
	
		bool isMessageResponse( const QDomDocument &doc ) const;
		bool isFaultResponse( const QDomDocument &doc ) const;

		KXmlRpcResult parseMessageResponse( const QDomDocument &doc ) const;
		KXmlRpcResult parseFaultResponse( const QDomDocument &doc ) const;

		QString markupCall( const QString &method,
							const QValueList<QVariant> &args ) const;
		QString marshal( const QVariant &v ) const;
		QVariant demarshal( const QDomElement &e ) const;

		KXmlRpcQuery( const QVariant &id, QObject *parent = 0, const char *name = 0 );
		~KXmlRpcQuery();

		QByteArray m_buffer;
		QVariant m_id;
};

#endif

