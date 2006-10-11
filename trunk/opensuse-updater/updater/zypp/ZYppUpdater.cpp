/* 
   Copyright (C) 2006 Novell Inc

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/poll.h>
#include <sys/types.h>
#include <fcntl.h>

#include <qobject.h>
#include <qpopupmenu.h>

#include <kprocess.h>
#include <kdebug.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kprocess.h>
#include <kconfig.h>
#include <kapp.h>

#include "ZYppUpdater.h"
#include "MainWindow.h"
//#include "ZYppInstallWindow.h"
//#include "ZYppConfigWindow.h"
#include "UpdateListItem.h"

// <?xml version='1.0'?>
// <update-status op="success">
//  <update-sources>
//   <source url="http://ftp.gwdg.de/pub/suse/update/10.1" alias="http://ftp.gwdg.de/pub/suse/update/10.1"/>
//  </update-sources>
//  <update-list>
//  <update category="security">
//   <name>java-1_5_0-sun</name>
//   <edition>1438-0</edition>
//  </update>
//  <update category="recommended">
//   <name>ghostscript-library</name>
//   <edition>1686-0</edition>
//  </update>
//  </update-list>
//  <update-summary total="2" security="1"/>
// </update-status>


ZYppUpdater::ZYppUpdater() : Updater()
  , _process(0L)
  , _you_process(0L)
  , _current_patch(0L)
  , _current_source(0L)
  , _state(Unknown)
{
  doCheckForUpdates();
}

void ZYppUpdater::slotYOUProcessExited( KProcess * )
{
  delete _you_process;
  _you_process = 0L;
  doCheckForUpdates();
}
 
void ZYppUpdater::showLog()
{

}

void ZYppUpdater::slotProcessExited( KProcess *proc )
{
  kdDebug() << "done..." << endl;
  kdDebug() << _buffer << endl;
  delete _process;
  _process = 0L;
  // parse the xml
  
  QXmlInputSource xml_source;
  xml_source.setData(_buffer);
  
  // clear the buffer
  _buffer.truncate(0);
  
  QXmlSimpleReader reader;
  reader.setContentHandler(this);
  reader.parse(xml_source);
}

void ZYppUpdater::slotReceivedStdout(KProcess *proc, char *buffer, int buflen)
{
  kdDebug() << "got..." << endl;
  // add stdout to a buffer#
  // we can parse it when process finishes
  _buffer += QString::fromUtf8( buffer, buflen );
}

void ZYppUpdater::slotReceivedStderr(KProcess *proc, char *buffer, int buflen)
{
}

void ZYppUpdater::doCheckForUpdates()
{
  kdDebug() << "checking..." << endl;

  if ( _process ) {
    kdDebug() << "Check still running." << endl;
    return;
  }

  _process = new KProcess;

  *_process << "/usr/sbin/zypp-checkpatches-wrapper";
  
  QObject::connect( _process, SIGNAL( processExited( KProcess * ) ),
           SLOT( slotProcessExited( KProcess * ) ) );
  QObject::connect( _process, SIGNAL( receivedStdout(KProcess *, char *, int ) ),
           SLOT( slotReceivedStdout(KProcess *, char *, int ) ) );
  QObject::connect( _process, SIGNAL( receivedStderr(KProcess *, char *, int ) ),
           SLOT( slotReceivedStderr(KProcess *, char *, int ) ) );


  _process->start( KProcess::NotifyOnExit, KProcess::AllOutput );
  //mStatusLabel->setText( i18n("Checking...") );
}

void ZYppUpdater::populateUpdateList(QListView *updateList)
{
  //currentPackages.clear();
  //currentPatches.clear();
}

void ZYppUpdater::updateSelected(QListViewItem *item)
{

}

void ZYppUpdater::updateMenu(QListViewItem *item, const QPoint& point)
{
// 	if (item != NULL && tempList != NULL)
//   {
// 		QPopupMenu *menu = new QPopupMenu(tempList);
// 		if (item->text(COLUMN_LOCK) == "") {
// 			menu->insertItem(i18n("Hold Back Package"), this, SLOT(holdPackage()));
// 		} else {
// 			menu->insertItem(i18n("Don't Hold Back Package"), this, SLOT(removeHold()));
// 		}
// 		menu->popup(point);
// 	}
}


/*****************************************************************************
									These are all noops because we do not support
									package holds yet
*****************************************************************************/
// void ZYppUpdater::holdPackage()
// {
// }
// 
// void ZYppUpdater::removeHold()
// {
// }
/******************************************************************************
******************************************************************************/

void ZYppUpdater::startInstall()
{
  if ( _you_process != 0L )
  {
    kdDebug() << "yast already running" << endl;
    return;
  }
  
  _you_process = new KProcess;
  *_you_process << "kdesu" << "yast2" << "online_update";

  connect( _you_process, SIGNAL( processExited( KProcess * ) ),
           SLOT( slotYouProcessExited( KProcess * ) ) );
  _you_process->start( KProcess::NotifyOnExit );
}

// void ZYppUpdater::startRefresh()
// {
// 	//emit(refreshList());
// }

void ZYppUpdater::configureUpdater()
{
	//ZYppConfigWindow *win = new ZYppConfigWindow(core); //deletes itself
	//win->show();
}

// QXmlHandler stuff

bool ZYppUpdater::startDocument()
{

}

bool ZYppUpdater::startElement( const QString & namespaceURI, const QString & localName, const QString & qName, const QXmlAttributes & atts  )
{
//   Unknown,
//   UpdateSources,
//   UpdateList,
  
  // here we have to allocate a patch or source in the stack
  kdDebug() << "xml..." << qName << endl;
}

bool ZYppUpdater::endElement( const QString&, const QString&, const QString& )
{
  // add the source of patch to the list
}



