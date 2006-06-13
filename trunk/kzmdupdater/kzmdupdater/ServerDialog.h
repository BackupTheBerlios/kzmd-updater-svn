#ifndef _SERVER_DIALOG_H_
#define _SERVER_DIALOG_H_

#include <klocale.h>
#include <klineedit.h>
#include <kpushbutton.h>

#include <qlabel.h>
#include <qdialog.h>
#include <qstring.h>
#include <qlayout.h>


class ServerDialog : public QDialog {

	Q_OBJECT

	public:

		ServerDialog(QWidget *parent=0);
		~ServerDialog();

		QValueList<QString> getServerInfo();

	private slots:
		
		void addButtonClicked();
		void cancelButtonClicked();

	private:

		void initGUI();

		QVBoxLayout *layout;
		QHBoxLayout *buttonLayout;
		QLabel *nameLabel;
		QLabel *serverLabel;
		KLineEdit *nameEdit;
		KLineEdit *serverEdit;
		KPushButton *cancelButton;
		KPushButton *addButton;

		QString server;
		QString name;
};


#endif
