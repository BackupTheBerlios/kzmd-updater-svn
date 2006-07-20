#include "udstest.h"
#include <kio/job.h>

#include <iostream>
using namespace std;

#define KIO_ARGS QByteArray packedArgs; QDataStream stream( packedArgs, IO_WriteOnly ); stream

UDSTest::UDSTest(QObject *parent) : QObject(parent) {

	KURL url = "udshttp:/var/tmp/kzmd-http";
//	KIO_ARGS << (int)1 << url;
//	KIO::TransferJob *job = new KIO::TransferJob(url, KIO::CMD_SPECIAL, packedArgs,QCString("Test"), false);
	if (!job) {
		cout << "We could not create the specified job, something is rather wrong" << endl;
	}
	job->addMetaData( "UserAgent", "Test/1.0" );
	job->addMetaData( "content-type", "Content-Type: text/xml; charset=utf-8" );
	job->addMetaData( "ConnectTimeout", "50" );

	connect( job, SIGNAL( data( KIO::Job *, const QByteArray & ) ),
    this, SLOT( slotData( KIO::Job *, const QByteArray & ) ) );
	connect(job, SIGNAL(result(KIO::Job *)), this, SLOT(slotResult(KIO::Job *)));
	cout << "Waiting..." << endl;
}

void UDSTest::slotData(KIO::Job *j, const QByteArray& d) {
	QString printData(d);

	cout << printData << endl;
}

void UDSTest::slotResult(KIO::Job *j) {
	cout << "Got a result" << endl;

	if (j->error() != 0) {
		cout << j->errorString() << endl;
		exit(1);
	}
}
