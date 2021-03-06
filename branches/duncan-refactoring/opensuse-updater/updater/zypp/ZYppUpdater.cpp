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

#include <kglobal.h>
#include <dcopclient.h>

#include <kgenericfactory.h>
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

K_EXPORT_COMPONENT_FACTORY( opensuseupdater_zypp, KGenericFactory<ZYppUpdater>( "opensuseupdater_zypp" ) )

ZYppUpdater::ZYppUpdater( QObject *parent, const char* name, const QStringList& ) : Updater( parent, name )
  , _process(0L)
  , _you_process(0L)
  , _state(Unknown)
  , _update_counter(0)
  , _list_view(0)
  , _found_update_status_tag(false)
  , _end_document_reached(false)
  , _error(false)
{
  //setInstance(KGenericFactory<ZYppUpdater>::instance());
  //setXMLFile("plugindemo_capitalizeui.rc");

  // For ease announce that we have been loaded.
  kdDebug() << "ZmdUpdater plugin loaded" << endl;

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
  
  QXmlSimpleReader reader;
  reader.setContentHandler(this);
  reader.parse(xml_source);
  
  if ( !_end_document_reached )
  {
    kdDebug() << "Houston, we have a problem." << endl;
    if ( _buffer.isEmpty() && !_stderr_buffer.isEmpty() )
      updateAppletError( i18n("helper program returned:\n%1").arg(_stderr_buffer) );
    else
      updateAppletError(i18n("helper program returned:\n%1").arg(_buffer) );
    
    // clear the buffer
    _stderr_buffer.truncate(0);
    _buffer.truncate(0);
    emit(populateDone());
    return;
  }  
  
  // clear the buffer
  _stderr_buffer.truncate(0);
  _buffer.truncate(0);
  
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
   
  if ( _patches.count() > 0 )
  {
    emit(updateApplet(APPLET_UPDATES, _patches.count()));
    emit(installAllowed(true));
  }
  else
  {
    emit(updateApplet(APPLET_NO_UPDATES, 0));
    emit(installAllowed(false));
  }
  
  if ( _error )
  {
    emit(updateAppletError(_error_message_buffer));
    _error_message_buffer.truncate(0);
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
  _stderr_buffer += QString::fromUtf8( buffer, buflen );
}

bool ZYppUpdater::networkOnline() const
{
  //enum EnumStatus { NoNetworks = 1, Unreachable, OfflineDisconnected,  OfflineFailed, ShuttingDown, Offline, Establishing, Online };
  
  int online = 1;
  if ( kapp->dcopClient()->isApplicationRegistered( "kded" ) )
  {
    QByteArray data, replyData;
    
    QDataStream arg(data, IO_WriteOnly);
    arg << QString("www.novell.com");
    
    QCString replyType;
    if ( !kapp->dcopClient()->call( "kded", "networkstatus", "status(QString)",
                                    data, replyType, replyData ) )
    {
      kdError() << "DCOP error." << endl;
    }
    else
    {
      QDataStream reply(replyData, IO_ReadOnly);
      if (replyType == "int") reply >> online;
      else kdDebug() << "DCOP: unexpected return type." << endl;
    }
  }
  
  return (online == 8) || ( online == 1 );
  
}

void ZYppUpdater::shutdown()
{

}

void ZYppUpdater::doCheckForUpdates()
{
  kdDebug() << "checking..." << endl;

  if ( networkOnline() )
  {
    kdDebug() << "Network available." << endl;
  }
  else
  {
    kdDebug() << "No network, skipping check..." << endl;
    return;
  }
  
  
  if ( _process ) {
    kdDebug() << "Check process still running. Will not run this time." << endl;
    emit(updateApplet(APPLET_CHECKING, 0));
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


  bool result = _process->start( KProcess::NotifyOnExit, KProcess::AllOutput );
  if ( ! result )
  {
    kdDebug() << "error launching zypp-checkpatches-wrapper..." << endl;
    emit(updateAppletError(i18n("Can't launch zypp-checkpatches-wrapper helper program. Make sure zypper package is installed and working.")));
    _list_view = 0L;
    delete _process;
    _process = 0L;
    emit(populateDone());
    return;
  }
  
  kdDebug() << "check process started.." << endl;
  emit(updateApplet(APPLET_CHECKING, 0));
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
  *_you_process << "opensuseupdater-su" << "-c" << "/sbin/yast2 online_update";

  connect( _you_process, SIGNAL( processExited( KProcess * ) ),
           SLOT( slotYOUProcessExited( KProcess * ) ) );
  _you_process->start( KProcess::NotifyOnExit );
}

// void ZYppUpdater::startRefresh()
// {
// 	//emit(refreshList());
// }

void ZYppUpdater::configureUpdater()
{
	KProcess *inst_source = new KProcess;
  *inst_source << "opensuseupdater-su" << "-c" << "/sbin/yast2 inst_source";

  connect( inst_source, SIGNAL( processExited( KProcess * ) ),
           SLOT( slotInstSourceProcessExited( KProcess * ) ) );
  inst_source->start( KProcess::NotifyOnExit );
}

// QXmlHandler stuff

bool ZYppUpdater::startDocument()
{
  kdDebug() << "start document..." << endl;
  _found_update_status_tag = false;
  _end_document_reached = false;
  _error = false;
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
  if ( _state == Error )
  {
    _error_message_buffer += ch;
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
  if ( qName == "update-status" )
  {
    _found_update_status_tag = true;
  }
  if ( qName == "errors")
  {
    _state = Errors;
  }
  if ( (qName == "error") && (_state == Errors ) )
  {
    _state = Error;
    _error = true;
  }
  if ( qName == "update" )
  {
    kdDebug() << "found patch..." << qName << endl;
    _current_patch = new ZYppPatch();
    _current_patch->name = atts.value("name");
    _current_patch->category = atts.value("category");
    _current_patch->edition = atts.value("edition");
    
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
      _current_patch->source = atts.value("alias");
    }
    else
    {
    
    }
    // ignore sources for now
    return true;
  }
  return true;
}

bool ZYppUpdater::endDocument()
{
  _end_document_reached = true;
  kdDebug() << "end document" << endl;
  if ( ! _found_update_status_tag )
  {
    kdDebug() << "Wrong xml output" << endl;
    emit(updateAppletError(_buffer));
    return false;
  }
  return true;
}

bool ZYppUpdater::endElement( const QString &uri , const QString &localname, const QString &qName )
{
  if ( qName == "source" )
  {
    //_state = Update;
  }
  if ( qName == "update-status" )
  {
      _state = Unknown;
  }
  if ( (qName == "error") && (_state == Error) )
  {
    _error_message_buffer += "\n";
    _state = Errors;
  }
  if ( (qName == "errors") )
  {
    _state = Unknown;
  }
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
    kdDebug() << _current_patch->name << " appended" << endl;
    _patches.append(_current_patch);
    // ignore sources for now
    
    kdDebug() << _current_patch->name << endl;
    kdDebug() << _current_patch->summary << endl;
    
    return true;
  }
  
  return true;
}



