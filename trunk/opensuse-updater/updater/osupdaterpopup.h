#include <qwidget.h>
#include <kpassivepopup.h>
#include <qpointarray.h>
#include <qpushbutton.h>
#include <qtooltip.h>
#include <qlayout.h>
#include <qtimer.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qbitmap.h>

#include "MainWindow.h"

class OSUpdaterPopup : public KPassivePopup
{
	Q_OBJECT
	public:
		OSUpdaterPopup(QWidget *parent,MainWindow *mainWindow);
		void setUpdates(int newUpdateValue);
	protected:
		void	paintEvent( QPaintEvent *e );
	private:
		int _updates;
		QVBoxLayout *_popupLayout;
		QHBoxLayout *_titleLayout;
		QHBoxLayout *_descriptionLayout;
		QLabel *_title;
		QPushButton *_closeButton;
		QLabel *_description;
		QHBoxLayout *_buttonLayout;
		QPushButton *_installButton;
		QPushButton *_cancelButton;
		QLabel * _iconLabel;
		QBitmap _mask;
};
