#include "SUSEUpdater.h"
#include <kiconloader.h>
#include <iostream>
using namespace std;

#define TRAY_ICON_GREEN "trayicon-green.png"

SUSEUpdater::SUSEUpdater() : KMainWindow(0L, "kzmdupdater") {

	KIconLoader iconLoader("kzmdupdater");
	mainBox = new QVBox(this);
	trayApplet = new KSystemTray(this);
	header = new HeaderWidget(mainBox);
	updateList = new QListView(mainBox);
	packageDescription = new KTextEdit(mainBox);
	buttons = new QWidget(mainBox);
	buttonsLayout = new QHBoxLayout(buttons);
	configureButton = new QPushButton("Configure Updater",buttons);
	cancelButton = new QPushButton("Cancel",buttons);
	installButton = new QPushButton("Install",buttons);

	trayApplet->setPixmap(UserIcon(TRAY_ICON_GREEN));
	trayApplet->setScaledContents(true);

	configureButton->setMinimumHeight(30);
	cancelButton->setMinimumHeight(30);
	installButton->setMinimumHeight(30);

	buttonsLayout->addWidget(configureButton,false, Qt::AlignLeft);
	buttonsLayout->insertSpacing(1, 250);
	buttonsLayout->addWidget(cancelButton,false, Qt::AlignRight);
	buttonsLayout->addWidget(installButton,false, Qt::AlignRight);

	header->setDescription("<b>Available Updates:</b><br> The following are software upgrades and patches to add features and fix bugs.<br> <u>Select those you would like and press install.</u>");

	packageDescription->setReadOnly(true);

	updateList->addColumn("Name");
	updateList->addColumn("Old Version");
	updateList->addColumn("New Version");
	updateList->addColumn("Size");
	connect(updateList, SIGNAL(selectionChanged(QListViewItem*)), this, SLOT(slotPackageSelected(QListViewItem*)));

	mainBox->setSpacing(10);
	mainBox->setMargin(10);
	setCentralWidget(mainBox);
	trayApplet->show();
	hide();
	checkUpdates();
}

SUSEUpdater::~SUSEUpdater() {
	delete configureButton;
	delete cancelButton;
	delete installButton;
	delete buttons;
	delete packageDescription;
	delete updateList;
	delete header;
	delete trayApplet;
}

void SUSEUpdater::checkUpdates() {

	QCheckListItem *testItem = new QCheckListItem(updateList, QString("Testing"), QCheckListItem::CheckBox);

	testItem->setText(1,"0.3");

	testItem = new QCheckListItem(updateList, "Testing 2", QCheckListItem::CheckBox);
}

void SUSEUpdater::slotPackageSelected(QListViewItem *packageSelected) {
	packageDescription->setText(((QCheckListItem*)packageSelected)->text());
}

void SUSEUpdater::slotExit() {
	close();
}
