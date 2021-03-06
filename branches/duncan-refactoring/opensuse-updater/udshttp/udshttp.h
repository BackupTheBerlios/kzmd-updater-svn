
#ifndef _udshttp_H_
#define _udshttp_H_

#include <qstring.h>
#include <qcstring.h>
#include <qsocket.h>
#include <qmap.h>
#include <qfile.h>
#include <qtextstream.h>

#include <kurl.h>
#include <kio/global.h>
#include <kio/slavebase.h>

/**

	@file

	This file is the main def for the UDS HTTP protocol. It defines the various constants
	we use and the class itself.

	@author Narayan Newton <narayannewton@gmail.com>
*/


class QCString;
class KSocket;

/**
	An anonymous enum to define the version of HTTP we will be using.

	HTTP_1_0 is of course HTTP 1.0 and HTTP_1_1 is HTTP 1.1

**/
enum { HTTP_1_0, HTTP_1_1 };

/**
	An anonymous enum to define the HTTP method we will be performing

	HTTP_GET is GET
	HTTP_PUT is PUT
	HTTP_HEAD is HEAD

**/
enum { HTTP_GET, HTTP_POST, HTTP_HEAD };


/**********************************************************************************

	This is the main class for the uds kioslave. It is used through TransferJob 
	right now, as some of the http specific commands in KIO are biased toward the 
	HTTPProtocol slave. You may send it metadata, but it will ignore quite a lot 
	of it. You can however set the user agent and the content-type. These two 
	things are most important for HTTP, especially HTTP_1_1.

	@author Narayan Newton <narayannewton@gmail.com>

***********************************************************************************/

class kio_udshttpProtocol : public KIO::SlaveBase {

	public:
		kio_udshttpProtocol(const QCString &pool_socket, const QCString &app_socket);
		virtual ~kio_udshttpProtocol();

		/**
			Returns the mimetype of the file at the given URL

			@param url the url we are checking.
		*/
		virtual void mimetype(const KURL& url);

		/**
			Gets the document at the specified url.

			@param url the url that represents the doc we are fetching.
		*/
		virtual void get(const KURL& url);

		/**
			The handler for "special" functions, those specific to our slave.

			@param data the code that defines the "real" function we are going to execute
		*/
		virtual void special(const QByteArray &data);

		/**
			Executes an HTTP_POST to the given url of data we are passed from the job

			@param url the actual url we will be uploading too
		*/
		virtual void post(const KURL& url);

		/**
			Executes an HTTP_HEAD for the given url. This is used by mimetype.

			@param url the actual url we will be checking.
		*/
		virtual void head(const KURL& url);

		/**
			Close the connection, force close. 
		*/
		virtual void closeConnection();

	private:

		void httpOpenConnection();

		//decides whether or not to close the connection and does so
		//also sets up the timeouts. 
		void httpCloseConnection();

		//Send any data/headers and wait for response
		void sendSocketData();
		void getSocketResponse();

		void buildReqLine();
		void buildHeader();
		void parseResponse();
		void parseUrl();
		void fetchMeta();


		KSocket *m_socket;
		bool m_connectionDone;

		KURL m_url;
		QString m_realSocketUrl;
		QString m_realPath;

		QString m_fromLine;

		//Data, we split into 3 parts
		QString m_reqLine;
		QString m_header;
		QString m_data;

		//Metadata info from job, if there is any
		QString m_contentType;
		QString m_userAgent;
		unsigned int m_connectTimeout;

		//Holds the response
		QString m_outputData;

		unsigned int m_httpMethod;
		unsigned int m_httpVersion;
};

#endif

