#ifndef _UDS_TEST_H_
#define _UDS_TEST_H_

#include <qobject.h>
#include <kio/job.h>



class UDSTest : public QObject {

	Q_OBJECT

	public:

		UDSTest(QObject *parent=0);

	private slots:

		void slotData(KIO::Job *, const QByteArray&);
		void slotResult(KIO::Job *);
};

#endif

