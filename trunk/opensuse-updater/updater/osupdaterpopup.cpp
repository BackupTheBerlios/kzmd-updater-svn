#include "osupdaterpopup.h"
#include "MainWindow.h"

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

	popupLayout = new QVBoxLayout(this, 11, 20, "popupLayout");

	titleLayout = new QHBoxLayout(popupLayout,KDialog::spacingHint(),"titleLayout");

	title = new QLabel("<b>openSUSE updater.</b>",this);
	title->setTextFormat(Qt::RichText);
	title->setAlignment(Qt::SingleLine);

	closeButton = new QPushButton("x",this,"closebutton");
	closeButton->setMinimumSize(QSize(20,20));
	closeButton->setMaximumSize(QSize(20,20));
	connect(closeButton, SIGNAL(clicked()),this, SLOT(hide()));

	titleLayout->addWidget(title);
	titleLayout->addStretch();
	titleLayout->addWidget(closeButton);
	
	description = new QLabel("New important updates are available.",this);

	KIconLoader *loader = KGlobal::iconLoader();
	QPixmap icon = loader->loadIcon("msg_warning",KIcon::Panel);
	iconLabel = new QLabel(this);
	iconLabel->setPixmap(icon);
	descriptionLayout = new QHBoxLayout(popupLayout,KDialog::spacingHint(),"descriptionlayout");

	descriptionLayout->addWidget(iconLabel);
	descriptionLayout->addWidget(description);
	
	buttonLayout = new QHBoxLayout(popupLayout,KDialog::spacingHint(),"buttonLayout");

	installButton = new QPushButton("Install now",this,"installbutton");
	cancelButton = new QPushButton("Ignore",this,"ignorebutton");
	connect(cancelButton, SIGNAL(clicked()),this, SLOT(hide()));
	connect(installButton, SIGNAL(clicked()),mainWindow, SIGNAL(startInstall()));
	connect(installButton, SIGNAL(clicked()),this, SLOT(hide()));

	buttonLayout->addStretch();
	buttonLayout->addWidget(installButton);
	buttonLayout->addWidget(cancelButton);
	buttonLayout->addStretch();
}

/**
 * Called when new updates are detected, with the new number of unapplied updates.
 */
void OSUpdaterPopup::setUpdates(int newUpdateValue)
{
	updates = newUpdateValue;
	if (updates > 0)
		show();
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
	bufferPainter.drawLine(0,title->y() + title->height() + 3,this->width(),title->y() + title->height() + 3);
	bufferPainter.setPen(QPen(QColorGroup::Background,3,SolidLine));
	//draw black border
	bufferPainter.drawRoundRect(0,0,width(),height(),1600/width(),1600/height());
	bufferPainter.setPen(QColor(55,97,155));
	bufferPainter.setBrush(QColor(55,97,155));
	bufferPainter.setBackgroundColor(QColor(55,97,155));
	//set title colour to blue
	title->setPaletteBackgroundColor(QColor(55,97,155));
	title->setPaletteForegroundColor(QColor(255,255,255));
	closeButton->setPalette(QPalette(QColor(55,97,155),QColor(55,97,155)));
	//draw blue background behind title
	bufferPainter.drawRect(0,0,width(),title->y() + title->height()+3);

	//do rounded corners
	//create mask the size of the popup
	mask.resize(width(), height());
	mask.fill(Qt::black);
	QPainter maskPainter(&mask);
	maskPainter.setBrush(Qt::white);
	maskPainter.setPen(Qt::white);
	//draw the rounded corners on the mask
	maskPainter.drawRoundRect(mask.rect(), 1600 / mask.rect().width(), 1600 / mask.rect().height());
	
	//apply the mask to this popup.
	setMask(mask);
}
