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
		int updates;
		QVBoxLayout *popupLayout;
		QHBoxLayout *titleLayout;
		QHBoxLayout *descriptionLayout;
		QLabel *title;
		QPushButton *closeButton;
		QLabel *description;
		QHBoxLayout *buttonLayout;
		QPushButton *installButton;
		QPushButton *cancelButton;
		QLabel * iconLabel;
		QBitmap mask;
};
