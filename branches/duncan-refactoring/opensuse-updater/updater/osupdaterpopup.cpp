#include "osupdaterpopup.h"
#include "MainWindow.h"

#include <klocale.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qlabel.h>
#include <kdialog.h>
#include <kglobalsettings.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qbitmap.h>
#include <kiconloader.h>
#include <kicontheme.h>

/**
 * Construct the popup
 * Add extra widgets to the passive popup & connect buttons to mainwindow signals.
 */
OSUpdaterPopup::OSUpdaterPopup(QWidget * parent,MainWindow * mainWindow) : KPassivePopup(parent)
{
	setTimeout(30000);

	_popupLayout = new QVBoxLayout(this, 11, 20, "popupLayout");

	_titleLayout = new QHBoxLayout(_popupLayout,KDialog::spacingHint(),"titleLayout");

	_title = new QLabel("<b>openSUSE updater.</b>",this);
	_title->setTextFormat(Qt::RichText);
	_title->setAlignment(Qt::SingleLine);

	_closeButton = new QPushButton("x",this,"closebutton");
	_closeButton->setMinimumSize(QSize(20,20));
	_closeButton->setMaximumSize(QSize(20,20));
	connect(_closeButton, SIGNAL(clicked()),this, SLOT(hide()));

	_titleLayout->addWidget(_title);
	_titleLayout->addStretch();
	_titleLayout->addWidget(_closeButton);
	
	_description = new QLabel(this);

	KIconLoader *loader = KGlobal::iconLoader();
	QPixmap icon = loader->loadIcon("msg_warning",KIcon::Panel);
	_iconLabel = new QLabel(this);
	_iconLabel->setPixmap(icon);
	_descriptionLayout = new QHBoxLayout(_popupLayout,KDialog::spacingHint(),"descriptionlayout");

	_descriptionLayout->addWidget(_iconLabel);
	_descriptionLayout->addWidget(_description);
	
	_buttonLayout = new QHBoxLayout(_popupLayout,KDialog::spacingHint(),"buttonLayout");

	_installButton = new QPushButton(i18n("Install"),this,"installbutton");
	_cancelButton = new QPushButton(i18n("Ignore"),this,"ignorebutton");
	connect(_cancelButton, SIGNAL(clicked()),this, SLOT(hide()));
	connect(_installButton, SIGNAL(clicked()),mainWindow, SIGNAL(startInstall()));
	connect(_installButton, SIGNAL(clicked()),this, SLOT(hide()));

	_buttonLayout->addStretch();
	_buttonLayout->addWidget(_installButton);
	_buttonLayout->addWidget(_cancelButton);
	_buttonLayout->addStretch();
}

/**
 * Called when new updates are detected, with the new number of unapplied updates.
 */
void OSUpdaterPopup::setUpdates(int newUpdateValue)
{
	_updates = newUpdateValue;
	if (_updates > 0)
  {
    _description->setText(i18n("1 Update Available", "%n Updates Available", _updates));
		show();
  }
}

/**
 * Draw custom background colours & round corners
 */
void OSUpdaterPopup::paintEvent( QPaintEvent *e )
{
	QPainter bufferPainter(this);
	bufferPainter.setPen(QPen(QColorGroup::Background,2,SolidLine));
	bufferPainter.setBrush( colorGroup().brush( QColorGroup::Background ) );

	bufferPainter.setPen(QPen(QColorGroup::Background,3,SolidLine));
	//draw the line under the title
	bufferPainter.drawLine(0,_title->y() + _title->height() + 3,this->width(),_title->y() + _title->height() + 3);
	bufferPainter.setPen(QPen(QColorGroup::Background,3,SolidLine));
	//draw black border
	bufferPainter.drawRoundRect(0,0,width(),height(),1600/width(),1600/height());
	bufferPainter.setPen(QColor(55,97,155));
	bufferPainter.setBrush(QColor(55,97,155));
	bufferPainter.setBackgroundColor(QColor(55,97,155));
	//set title colour to blue
	_title->setPaletteBackgroundColor(QColor(55,97,155));
	_title->setPaletteForegroundColor(QColor(255,255,255));
	_closeButton->setPalette(QPalette(QColor(55,97,155),QColor(55,97,155)));
	//draw blue background behind title
	bufferPainter.drawRect(0,0,width(),_title->y() + _title->height()+3);

	//do rounded corners
	//create mask the size of the popup
	_mask.resize(width(), height());
	_mask.fill(Qt::black);
	QPainter maskPainter(&_mask);
	maskPainter.setBrush(Qt::white);
	maskPainter.setPen(Qt::white);
	//draw the rounded corners on the mask
	maskPainter.drawRoundRect(_mask.rect(), 1600 / _mask.rect().width(), 1600 / _mask.rect().height());
	
	//apply the mask to this popup.
	setMask(_mask);
}
