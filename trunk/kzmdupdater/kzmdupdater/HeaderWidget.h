#ifndef _HEADER_WIDGET_H_
#define _HEADER_WIDGET_H_

#include <qlabel.h>
#include <kiconloader.h>
#include <qlayout.h>

class HeaderWidget : public QWidget {

	Q_OBJECT

	public:

		HeaderWidget(QWidget *parent=0);
		~HeaderWidget();

		void setDescription(QString description);

	private:

		QHBoxLayout *layout;
		QLabel *suseLogo;
		QLabel *text;
};

#endif
