
#include <qcstring.h>
#include <qsocket.h>
#include <qdatetime.h>
#include <qbitarray.h>
#include <qtextstream.h>
#include <qeventloop.h>

#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/poll.h>

#include <kapplication.h>
#include <kdebug.h>
#include <kmessagebox.h>
#include <kinstance.h>
#include <kglobal.h>
#include <kstandarddirs.h>
#include <klocale.h>
#include <kurl.h>
#include <ksock.h>

#include "udshttp.h"

using namespace KIO;

#define LOG
#define DEBUGCODE 7101
#define SOCKET_TIMEOUT (100*60)

extern "C"
{
    int kdemain(int argc, char **argv)
    {
        KInstance instance( "kio_udshttp" );
        
        kdDebug(DEBUGCODE) << "*** Starting kio_udshttp " << endl;
        
        if (argc != 4) {
            kdDebug(DEBUGCODE) << "Usage: kio_udshttp  protocol domain-socket1 domain-socket2" << endl;
            exit(-1);
        }

        kio_udshttpProtocol slave(argv[2], argv[3]);
        slave.dispatchLoop();
        
        kdDebug(DEBUGCODE) << "*** kio_udshttp Done" << endl;
        return 0;
    }
}

kio_udshttpProtocol::kio_udshttpProtocol(const QCString &pool_socket, const QCString &app_socket)
    : SlaveBase("kio_udshttp", pool_socket, app_socket) {

 	m_socket = NULL; 
	m_httpVersion = HTTP_1_1;
	m_userAgent = "kio_udshttp/1.0";
	m_connectionDone = false;
}


kio_udshttpProtocol::~kio_udshttpProtocol() {
	m_connectionDone = true;
	httpCloseConnection();
}


/*******************************************************************
 *
 *				Virtual Functions From SlaveBase
 *
 *						Called By TransferJobs
 *
 *
 *******************************************************************/

void kio_udshttpProtocol::special(const QByteArray &data) {

	int tmp;
	KURL url;
	QDataStream stream(data, IO_ReadOnly);

	stream >> tmp;
	switch (tmp) {

		case 1: //HTTP_POST
			stream >> url;
			post(url);
			break;
		case 99:
			httpCloseConnection();
			break;
	};

}

void kio_udshttpProtocol::post(const KURL& url) {

	m_url = url;

	kdWarning(DEBUGCODE) << "Entering post" << endl;

	fetchMeta();
	parseUrl();
	httpOpenConnection();
	m_httpMethod = HTTP_POST;

	if (m_data.isEmpty() == true) {
		int result;
		QByteArray tempData;

		kdWarning(DEBUGCODE) << "About to fetch data" << endl;
		dataReq();
		if ((result = readData(tempData)) < 0) {
			error(ERR_INTERNAL, i18n("There were errors getting data from job"));
		} else if (result == 0) {
			kdWarning(DEBUGCODE) << "Got 0 bytes of data" << endl;
			data(QByteArray());
			finished();
		}
		m_data.insert(0, tempData);
	}
	kdWarning(DEBUGCODE) << "Got data: " << m_data << endl;
	buildReqLine();
	buildHeader();



	sendSocketData();
	getSocketResponse();
}

void kio_udshttpProtocol::get(const KURL& url ) {

	m_url = url;

	kdWarning(DEBUGCODE) << "Entering get" << endl;

	fetchMeta();
	parseUrl();
	httpOpenConnection();
	m_httpMethod = HTTP_GET;
	buildReqLine();
	buildHeader();
	

	sendSocketData();
	getSocketResponse();
}

void kio_udshttpProtocol::head(const KURL& url) {

	m_url = url;

	kdWarning(DEBUGCODE) << "Entering head" << endl;

	fetchMeta();
	parseUrl();
	httpOpenConnection();
	m_httpMethod = HTTP_HEAD;
	buildReqLine();
	buildHeader();
	

	sendSocketData();
	getSocketResponse();
}

void kio_udshttpProtocol::mimetype(const KURL& url) {
	head(url);
	data(QCString(m_contentType.local8Bit()));
	data(QByteArray());
	kdWarning(DEBUGCODE) << "Sent data to job" << endl;
	finished();
}

/*********************************************************************
 *
 *	Private member functions
 *
 *********************************************************************/

void kio_udshttpProtocol::httpOpenConnection() {

	if (m_socket != NULL)
		return;
	if (m_realSocketUrl.isEmpty())
		return;
	m_connectionDone = false;
	m_socket = new KSocket(m_realSocketUrl);
}

void kio_udshttpProtocol::httpCloseConnection() {
	if (m_socket->socket() > 0 && m_connectionDone == true) {
		close(m_socket->socket());
		delete m_socket;
		m_socket = NULL;
	} 
}

//Sends data and cleary out our temp holding variables
void kio_udshttpProtocol::sendSocketData() {
	QString d;
	ssize_t amountSent = 0;

	if (m_reqLine.isEmpty() == false) {
		d += m_reqLine;
		m_reqLine.truncate(0);
	}

	if (m_header.isEmpty() == false) {
		d += m_header;
		m_header.truncate(0);
	}

	if (m_data.isEmpty() == false) {
		d += m_data;
		m_data.truncate(0);
	}

	while ((amountSent += send(m_socket->socket(), d.ascii(), d.length(), 0)) < d.length())
		continue;
}

//Polls for a response on our socket
void kio_udshttpProtocol::getSocketResponse() {

	int count;
	char buffer[1024];
	struct pollfd fd;

	if (m_socket->socket() <= 0)
		return;

	fd.fd = m_socket->socket();
	fd.events = POLLIN;

	//clear any data we had sitting around
	m_outputData.truncate(0);

	if (poll(&fd, 1, SOCKET_TIMEOUT) > 0) {
		while ((count = recv(m_socket->socket(), buffer, 1024, MSG_DONTWAIT)) > 0) {
			buffer[count] = '\0';
			m_outputData += buffer;
		}
		parseResponse();
		httpCloseConnection();
		data(QCString(m_outputData.local8Bit()));
		data(QByteArray());
		finished();
		return;
	}
	error(ERR_SERVER_TIMEOUT, i18n("Timeout on server"));
}

//Parses out the real socket and path, sets m_realPath and m_realSocketUrl
void kio_udshttpProtocol::parseUrl() {

	int index;

	m_realSocketUrl = (m_url.url()).remove(0, 8); // udshttp: 8 chars
	index = m_realSocketUrl.find(":");
	if (index > 0) {
		m_realPath = m_realSocketUrl.mid(index+1);
		m_realSocketUrl.truncate(index);
	} 
	if (m_realSocketUrl[0] != '/') {
		error(ERR_MALFORMED_URL, i18n("Bad url"));
		return;
	}
}

void kio_udshttpProtocol::fetchMeta() {
	
	if (hasMetaData("content-type") == true) {
		m_contentType = metaData("content-type");
	}

	if (hasMetaData("UserAgent") == true) {
		m_userAgent = metaData("UserAgent");
	}
}


void kio_udshttpProtocol::buildReqLine() {
	switch (m_httpMethod) {

		case HTTP_GET:
			m_reqLine ="GET ";
			break;
		case HTTP_POST:
			m_reqLine = "POST ";
			break;
		case HTTP_HEAD:
			m_reqLine = "HEAD ";
			break;
	}
	if (m_realPath.isEmpty() == true && m_realPath.stripWhiteSpace() == "") {
		m_reqLine += "/index.html";
	} else {
		m_reqLine += m_realPath;
	}
	m_reqLine += " ";
	switch (m_httpVersion) {

		case HTTP_1_0:
			m_reqLine += "HTTP/1.0\r\n";
			break;
		case HTTP_1_1:
			m_reqLine += "HTTP/1.1\r\n";
			break;
	}
}

void kio_udshttpProtocol::buildHeader() {
	m_header = "";
	if (m_httpVersion == HTTP_1_1) {
		m_header += "Host: ";
		m_header += (m_realSocketUrl.isEmpty() == true) ? "localhost" : m_realSocketUrl;
		m_header += "\r\n";
	}
	if (m_fromLine != "") {
		m_header += "From: " + m_fromLine + "\r\n";
	}
	if (m_userAgent != "") {
		m_header += "User-Agent: " + m_userAgent + "\r\n";
	}
	if (m_httpMethod == HTTP_POST) {
		QString length;

		if (m_contentType.isEmpty() == true) {
			m_header += QString("Content-Type: ") + "text/html" + "\r\n"; 
		} else {
			if (m_contentType.find("Content-Type:") == -1) {
				m_header += QString("Content-Type: ");
			}
			m_header += m_contentType + "\r\n";
		}
		length.setNum(m_data.length());
		m_header += "Content-Length: " + length + "\r\n";
	}
	m_header += "\r\n"; //Extra break to signal that the header is done
}

void kio_udshttpProtocol::parseResponse() {
	QTextStream stream(m_outputData, IO_ReadOnly);
	QStringList tokens;
	QString line;
	unsigned int size;

	if ((line = stream.readLine()) == "" || line.isNull()) 
		return;

	tokens = QStringList::split(" ", line);
	int responseCode = tokens[1].toInt();
	switch (responseCode) {
		case 100:
			//HTTP_1_1 only 
			kdWarning(450) << "Got a continue" << endl;
			break;
		case 200:
			break;
		case 401:
		case 403:
			error(ERR_ACCESS_DENIED, i18n("Unauthorized access"));
			break;
		case 404:
			error(ERR_MALFORMED_URL, i18n("URL Invalid"));
		case 414:
			error(ERR_MALFORMED_URL, i18n("URL Invalid"));
			break;
		case 500:
			error(ERR_INTERNAL_SERVER, i18n("Server Error"));
			break;
		case 503:
			error(ERR_SERVICE_NOT_AVAILABLE, i18n("Service unavailable"));
			break;
		case 505:
			kdWarning(450) << "HTTP 1.1 not supported, dropping back to 1.0" << endl;
			m_httpVersion = HTTP_1_0;
			switch (m_httpMethod) {
				case HTTP_POST:
					post(m_url);
					break;
				case HTTP_GET:
					get(m_url);
					break;
			}
		default:
			kdError(DEBUGCODE) << "ERROR: " << tokens[2] << endl;
			return;
	}

	//This strips \r\n, so when it fails on != "" we just got a blank line
	while ((line = stream.readLine()) != "") {
		tokens = QStringList::split(" ", line);
		if (tokens[0] == "Date:") {
			continue;
		} else if (tokens[0] == "Content-Type:") {
			tokens.erase(tokens.begin());
			m_contentType += tokens.join(" ");
			kdWarning(DEBUGCODE) << "Content type is: " << m_contentType << endl;
			continue;
		} else if (tokens[0] == "Content-Size:") {
			size = tokens[1].toUInt();	
		} else if (tokens[0] == "Connection:") {
			if (tokens[1] == "close")
				kdWarning(DEBUGCODE) << "Got a close" << endl;
				m_connectionDone = true;
		}
	}
	m_outputData = stream.read();
}

