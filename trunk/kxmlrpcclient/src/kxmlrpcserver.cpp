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

#include <qfile.h>

#include <kdebug.h>
#include <kio/job.h>
#include <klocale.h>
#include <kmdcodec.h>
#include <kurl.h>

#include <qvariant.h>

#include "kxmlrpcserver.h"
#include "kxmlrpcquery.h"

KXmlRpcServer::KXmlRpcServer( const KURL &url, QObject *parent, const char *name ) : QObject( parent, name ) {

	if ( url.isValid() )
		m_url = url;

	m_userAgent = "KDE XMLRPC resources";
}

KXmlRpcServer::~KXmlRpcServer() {

	QValueList<KXmlRpcQuery*>::Iterator it;
	for ( it = mPendingQueries.begin(); it !=mPendingQueries.end(); ++it )
		(*it)->deleteLater();

	mPendingQueries.clear();
}

void KXmlRpcServer::queryFinished( KXmlRpcQuery *query ) {

	mPendingQueries.remove( query );
	query->deleteLater();
}

void KXmlRpcServer::setUrl( const KURL &url ) {

	m_url = url.isValid() ? url : KURL();
}

void KXmlRpcServer::call( const QString &method, const QValueList<QVariant> &args,
                   QObject* msgObj, const char* messageSlot,
                   QObject* faultObj, const char* faultSlot, const QVariant &id ) {

	if ( m_url.isEmpty() )
		kdWarning() << "Cannot execute call to " << method << ": empty server URL" << endl;

	KXmlRpcQuery *query = KXmlRpcQuery::create( id, this );
	connect( query, SIGNAL( message( const QValueList<QVariant> &, const QVariant& ) ), msgObj, messageSlot );
	connect( query, SIGNAL( fault( int, const QString&, const QVariant& ) ), faultObj, faultSlot );
	connect( query, SIGNAL( finished( KXmlRpcQuery* ) ), this, SLOT( queryFinished( KXmlRpcQuery* ) ) );
	mPendingQueries.append( query );

	query->call( m_url.url(), method, args, m_userAgent );
}

void KXmlRpcServer::call( const QString &method, const QVariant &arg,
                   QObject* msgObj, const char* messageSlot,
                   QObject* faultObj, const char* faultSlot,
                   const QVariant &id ) {

	QValueList<QVariant> args;
	args << arg ;
	call( method, args, msgObj, messageSlot, faultObj, faultSlot, id );
}

void KXmlRpcServer::call( const QString &method, int arg,
                   QObject* msgObj, const char* messageSlot,
                   QObject* faultObj, const char* faultSlot,
                   const QVariant &id ) {

	QValueList<QVariant> args;
	args << QVariant( arg );
	call( method, args, msgObj, messageSlot, faultObj, faultSlot, id );
}

void KXmlRpcServer::call( const QString &method, bool arg,
                   QObject* msgObj, const char* messageSlot,
                   QObject* faultObj, const char* faultSlot,
                   const QVariant &id ) {

	QValueList<QVariant> args;
	args << QVariant( arg );
	call( method, args, msgObj, messageSlot, faultObj, faultSlot, id );
}

void KXmlRpcServer::call( const QString &method, double arg ,
                   QObject* msgObj, const char* messageSlot,
                   QObject* faultObj, const char* faultSlot,
                   const QVariant &id ) {

	QValueList<QVariant> args;
	args << QVariant( arg );
	call( method, args, msgObj, messageSlot, faultObj, faultSlot, id );
}

void KXmlRpcServer::call( const QString &method, const QString &arg ,
                   QObject* msgObj, const char* messageSlot,
                   QObject* faultObj, const char* faultSlot,
                   const QVariant &id ) {

	QValueList<QVariant> args;
	args << QVariant( arg );
	call( method, args, msgObj, messageSlot, faultObj, faultSlot, id );
}

void KXmlRpcServer::call( const QString &method, const QCString &arg,
                   QObject* msgObj, const char* messageSlot,
                   QObject* faultObj, const char* faultSlot,
                   const QVariant &id ) {

	QValueList<QVariant> args;
	args << QVariant( arg );
	call( method, args, msgObj, messageSlot, faultObj, faultSlot, id );
}

void KXmlRpcServer::call( const QString &method, const QByteArray &arg ,
                   QObject* msgObj, const char* messageSlot,
                   QObject* faultObj, const char* faultSlot,
                   const QVariant &id ) {

	QValueList<QVariant> args;
	args << QVariant( arg );
	call( method, args, faultObj, faultSlot, msgObj, messageSlot, id );
}

void KXmlRpcServer::call( const QString &method, const QDateTime &arg,
                   QObject* msgObj, const char* messageSlot,
                   QObject* faultObj, const char* faultSlot,
                   const QVariant &id ) {

	QValueList<QVariant> args;
	args << QVariant( arg );
	call( method, args, msgObj, messageSlot, faultObj, faultSlot, id );
}

void KXmlRpcServer::call( const QString &method, const QStringList &arg,
                   QObject* msgObj, const char* messageSlot,
                   QObject* faultObj, const char* faultSlot,
                   const QVariant &id ) {

	QValueList<QVariant> args;
	QStringList::ConstIterator it = arg.begin();
	QStringList::ConstIterator end = arg.end();
	for ( ; it != end; ++it )
		args << QVariant( *it );
	call( method, args, msgObj, messageSlot, faultObj, faultSlot, id );
}

