#ifndef _SUSE_UPDATER_
#define _SUSE_UPDATER_

#include <kmainwindow.h>
#include <ksystemtray.h>
#include <qlistview.h>
#include <ktextedit.h>
#include <qlabel.h>
#include <qvbox.h>
#include <qpushbutton.h>

#include "HeaderWidget.h"

class SUSEUpdater : public KMainWindow {

	Q_OBJECT

	public:

		SUSEUpdater();
		~SUSEUpdater();

	public slots:

		void slotExit();
		void slotPackageSelected(QListViewItem *packageSelected);

	private:

		void checkUpdates();

		KSystemTray *trayApplet;
		QListView *updateList;
		KTextEdit *packageDescription;
		HeaderWidget *header;

		QPushButton *installButton;
		QPushButton *cancelButton;
		QPushButton *configureButton;

		QVBox *mainBox;
		QWidget *buttons;
		QHBoxLayout *buttonsLayout;
};

#endif
