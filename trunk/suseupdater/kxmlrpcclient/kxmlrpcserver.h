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

#ifndef _KXML_RPC_SERVER_H_
#define _KXML_RPC_SERVER_H_

#include <kurl.h>

#include <qobject.h>
#include <qvaluelist.h>
#include <qvariant.h>

/**   
   @file
  
  	This file defines KXmlRpcServer, our main class.
	It is the primary method of interaction with the library and is the object which represents the xml-rpc server.

	@author Narayan Newton <narayannewton@gmail.com>
	@author Frerich Raabe <raabe@kde.org>
	@author Tobias Koenig <tokoe@kde.org>
  
**/

//pre-decls
class KXmlRpcQuery;
class KXmlRpcServer;


/**
		KXmlRpcServer is a class that represents the xmlrpc server we are
		communicating with. This is the main (only) class you
		need to worry about for building an xml-rpc client.
		This class has one main method, "call", which is overloaded extensively
		to handle different arguments.

		@author Narayan Newton <narayannewton@gmail.com>

		@code
			KXmlRpcServer *serv = new KXmlRpcServer(KURL("http://localhost"), this);
			serv->setUserAgent("Test/1.0");
			serv->call("xmlrpc.command1", "Hi!", 
					   this, SLOT(gotData(const QValueList<QVariant>&, const QVariant)),
					   this, SLOT(gotError(const QString&, const QVariant&)));
		@endcode

**/

class KXmlRpcServer : public QObject {

	Q_OBJECT
	
	public:

		/**
				The standard init function with few (possibly no) arguments

				@param parent the parent of this object, defaults to NULL.
				@param name the name of the object, defaults to NULL.


		**/

		KXmlRpcServer( QObject *parent = 0, const char *name = 0 );

		/**
				The not so standard init function that takes a server url 
				as an argument 
	
				@param url the url for the xml-rpc server we will be connecting to
				@param parent the parent of this object, defaults to NULL.
				@param name the name of the object, defaults to NULL.

		**/

		KXmlRpcServer( const KURL &url, QObject *parent = 0, const char *name = 0 );

		~KXmlRpcServer();


		/**
			Gets the current url of the xml-rpc server.

			@return returns a QString set to the url of the xml-rpc server

		**/

		const KURL &url() const { return m_url; }

		/**
			Sets the url for the xml-rpc server 
			
			@param url the url for the xml-rpc server we will be connecting to


		**/

		void setUrl( const KURL &url );

		/**
			Gets the current user agent

			@return returns a QString set to the user agent

		**/

		QString userAgent() const { return m_userAgent; }

		/**
			Sets the url for the xml-rpc server

			@param userAgent the user agent to use for connecting to the xml-rpc server


		**/

		void setUserAgent( const QString &userAgent ) { m_userAgent = userAgent; }


		/**
			The main function for KXMLRPC. This make a xml-rpc call to the server set via
			the constructor or via setUrl. You pass in the method, the argument list and
			a slot for data arrival and a slot for possible errors.

			This method is HIGHLY over-loaded and relies heavily on QValueLists and QVariants.

			The following are the types of arguments supported as arguments:

				QValueList<QVariant>
				QVariant
				QString
				QCString
				QByteArray
				QDateTime
				QStringList
				int
				bool
				double

			@param method the method on the server we are going to be calling
			@param arg the argument or arguments you will be passing to the method
			@param obj the QObject of the error slot
			@param faultSlot the error slot itself
			@param obj the QObject of the data slot
			@param messageSlot the slot receiving the data
			@param id the id for our KXmlRpcServer object, defaults to empty

		**/

		template <typename T>
		void call( const QString &method, const QValueList<T> &arg,
				QObject* obj, const char* messageSlot, 
				QObject* obj, const char* faultSlot,
				const QVariant &id = QVariant() );


	public slots:
	
		void call( const QString &method, const QValueList<QVariant> &args,
				QObject* msgObj, const char* messageSlot,
				QObject* faultObj, const char* faultSlot,
				const QVariant &id = QVariant() );

		void call( const QString &method, const QVariant &arg,
				QObject* msgObj, const char* messageSlot,
				QObject* faultObj, const char* faultSlot,
				const QVariant &id = QVariant() );

		void call( const QString &method, int arg ,
				QObject* msgObj, const char* messageSlot,
				QObject* faultObj, const char* faultSlot,
				const QVariant &id = QVariant() );

		void call( const QString &method, bool arg,
				QObject* msgObj, const char* messageSlot,
				QObject* faultObj, const char* faultSlot,
				const QVariant &id = QVariant() );

		void call( const QString &method, double arg,
				QObject* msgObj, const char* messageSlot,
				QObject* faultObj, const char* faultSlot,
				const QVariant &id = QVariant() );

		void call( const QString &method, const QString &arg,
				QObject* msgObj, const char* messageSlot,
				QObject* faultObj, const char* faultSlot,
				const QVariant &id = QVariant() );

		void call( const QString &method, const QCString &arg ,
				QObject* msgObj, const char* messageSlot,
				QObject* faultObj, const char* faultSlot,
				const QVariant &id = QVariant() );

		void call( const QString &method, const QByteArray &arg,
				QObject* msgObj, const char* messageSlot,
				QObject* faultObj, const char* faultSlot,
				const QVariant &id = QVariant() );

		void call( const QString &method, const QDateTime &arg,
				QObject* msgObj, const char* messageSlot,
				QObject* faultObj, const char* faultSlot,
				const QVariant &id = QVariant() );

		void call( const QString &method, const QStringList &arg,
				QObject* msgObj, const char* messageSlot,
				QObject* faultObj, const char* faultSlot,
				const QVariant &id = QVariant() );

	private slots:

		void queryFinished( KXmlRpcQuery* );

	private:
		
		KURL m_url;
		QString m_userAgent;

		QValueList<KXmlRpcQuery*> mPendingQueries;
};

template <typename T>
void KXmlRpcServer::call( const QString &method, const QValueList<T> &arg,
						QObject* msgObj, const char* messageSlot, 							
						QObject* faultObj, const char* faultSlot,
						const QVariant &id ) {

	QValueList<QVariant> args;

	typename QValueList<T>::ConstIterator it = arg.begin();
	typename QValueList<T>::ConstIterator end = arg.end();
	for ( ; it != end; ++it )
		args << QVariant( *it );

	return call( method, args, faultObj, faultSlot, msgObj, messageSlot, id );
}

#endif

