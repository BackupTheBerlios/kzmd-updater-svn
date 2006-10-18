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

#include <qptrlist.h>
#include <qlistview.h>
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
#include "ZYppListItem.h"
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
  , _state(Unknown)
  , _update_counter(0)
  , _list_view(0)
{
  kdDebug() << "Initializing zypp backend" << endl;
  doCheckForUpdates();
}

UpdaterCapabilities ZYppUpdater::capabilities()
{
  UpdaterCapabilities caps;
  caps.canSelectIndividualUpdates = false;
  return caps;
}

void ZYppUpdater::slotYOUProcessExited( KProcess *p )
{
  delete p;
  _you_process = 0L;
  doCheckForUpdates();
}

void ZYppUpdater::slotInstSourceProcessExited( KProcess *p )
{
  delete p;
}
 
void ZYppUpdater::showLog()
{

}

void ZYppUpdater::slotProcessExited( KProcess *proc )
{
  kdDebug() << "check process finished..." << endl;
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
  
  if ( ! _list_view )
    return;
  
  kdDebug() << "populating... " << _patches.count() << " patches" << endl;
  for ( QPtrList<ZYppPatch>::const_iterator it = _patches.begin(); it != _patches.end(); ++it )
  {
    QListViewItem *newItem;
    newItem = new ZYppListItem( (*it), _list_view, (*it)->summary );

		newItem->setText(COLUMN_TYPE, ((*it)->category == "security") ? i18n("Security") : i18n("Patch") );
		newItem->setText(COLUMN_NEW_VERSION,(*it)->edition);
		//newItem->setText(COLUMN_ID, (*iter).id);
		newItem->setText(COLUMN_CATALOG, (*it)->source );
  }
  _list_view = 0L;
  emit(populateDone());
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
    kdDebug() << "Check process still running. Will not run this time." << endl;
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
  kdDebug() << "check process started.." << endl;
  //mStatusLabel->setText( i18n("Checking...") );
}

void ZYppUpdater::populateUpdateList(QListView *updateList)
{
  kdDebug() << "got populate UI request..." << endl;
  _list_view = updateList;
  emit(updateApplet(APPLET_NO_UPDATES, 0));
  doCheckForUpdates();
}

void ZYppUpdater::updateSelected(QListViewItem *item)
{
  kdDebug() << "updating item" << endl;
  ZYppListItem *zi = dynamic_cast<ZYppListItem *>(item);
  if ( zi )
  {
    ZYppPatch *patch = zi->patch();
    emit(returnDescription(patch->description));
  }
  
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
	KProcess *inst_source = new KProcess;
  *inst_source << "kdesu" << "yast2" << "inst_source";

  connect( inst_source, SIGNAL( processExited( KProcess * ) ),
           SLOT( slotInstSourceProcessExited( KProcess * ) ) );
  inst_source->start( KProcess::NotifyOnExit );
}

// QXmlHandler stuff

bool ZYppUpdater::startDocument()
{
  kdDebug() << "start document..." << endl;

  _patches.setAutoDelete(true);
  _patches.clear();
  return true;
}

bool ZYppUpdater::characters ( const QString & ch )
{
  if ( _state == UpdateDescription )
  {
    _current_patch->description += ch;
  }
  if ( _state == UpdateSummary )
  {
    _current_patch->summary += ch;
  }
  return true;
}

bool ZYppUpdater::startElement( const QString & namespaceURI, const QString & localName, const QString & qName, const QXmlAttributes & atts  )
{
//   Unknown,
//   UpdateSources,
//   UpdateList,
 
  // here we have to allocate a patch or source in the stack
  kdDebug() << "xml..." << qName << endl;
  
  if ( qName == "update" )
  {
    kdDebug() << "found patch..." << qName << endl;
    _current_patch = new ZYppPatch();
    _current_patch->name = atts.value("name");
    _current_patch->category = atts.value("category");
    _current_patch->edition = atts.value("edition");
    _current_patch->source = atts.value("source");
    
    _state = Update;
    // ignore sources for now
    return true;
  }
  if ( qName == "description" )
  {
    _state = UpdateDescription;
    return true;
  }
  if ( qName == "summary" )
  {
    _state = UpdateSummary;
    // ignore sources for now
    return true;
  }
  if ( qName == "source" )
  {
    if ( _state == Update )
    {
    
    }
    else
    {
    
    }
    // ignore sources for now
    return true;
  }
  return true;
}

bool ZYppUpdater::endElement( const QString &uri , const QString &localname, const QString &qName )
{
  if ( qName == "description" )
  {
    _state = Update;
    return true;
  }
  if ( qName == "summary" )
  {
    _state = Update;
    // ignore sources for now
    return true;
  }
  if ( qName == "update" )
  {
    _state = Unknown;
    _patches.append(_current_patch);
    // ignore sources for now
    
    kdDebug() << _current_patch->name << endl;
    kdDebug() << _current_patch->summary << endl;
    
    return true;
  }
  
  return true;
}



