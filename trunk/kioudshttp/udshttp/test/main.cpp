#include <kapplication.h>
#include "udstest.h"

int main(int argc, char **argv) {

	KApplication app(argc, argv, "Testing", false, false);

	UDSTest *test = new UDSTest();
	return app.exec();
}
