#include <kapplication.h>
#include "SUSEUpdater.h"

int main(int argc, char **argv) {

	KApplication app(argc, argv, "KZMD-Updater");
	SUSEUpdater *mainApp = new SUSEUpdater();

	mainApp->setGeometry(QRect(0,0,550,500));
	return app.exec();
}
