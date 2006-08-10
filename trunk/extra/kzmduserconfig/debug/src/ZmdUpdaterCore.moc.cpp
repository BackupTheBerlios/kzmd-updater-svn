/****************************************************************************
** ZmdUpdaterCore meta object code from reading C++ file 'ZmdUpdaterCore.h'
**
** Created: Thu Aug 10 06:19:42 2006
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../src/ZmdUpdaterCore.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ZmdUpdaterCore::className() const
{
    return "ZmdUpdaterCore";
}

QMetaObject *ZmdUpdaterCore::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ZmdUpdaterCore( "ZmdUpdaterCore", &ZmdUpdaterCore::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ZmdUpdaterCore::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ZmdUpdaterCore", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ZmdUpdaterCore::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ZmdUpdaterCore", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ZmdUpdaterCore::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ 0, &static_QUType_varptr, "\x02", QUParameter::In },
	{ 0, &static_QUType_QVariant, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"catalogData", 2, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ 0, &static_QUType_varptr, "\x02", QUParameter::In },
	{ 0, &static_QUType_QVariant, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"identityData", 2, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ 0, &static_QUType_varptr, "\x02", QUParameter::In },
	{ 0, &static_QUType_QVariant, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"catalogSubData", 2, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ 0, &static_QUType_varptr, "\x02", QUParameter::In },
	{ 0, &static_QUType_QVariant, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"serviceData", 2, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ 0, &static_QUType_varptr, "\x02", QUParameter::In },
	{ 0, &static_QUType_QVariant, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"updateData", 2, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ 0, &static_QUType_varptr, "\x02", QUParameter::In },
	{ 0, &static_QUType_QVariant, 0, QUParameter::In }
    };
    static const QUMethod slot_5 = {"patchData", 2, param_slot_5 };
    static const QUParameter param_slot_6[] = {
	{ 0, &static_QUType_varptr, "\x02", QUParameter::In },
	{ 0, &static_QUType_QVariant, 0, QUParameter::In }
    };
    static const QUMethod slot_6 = {"lockData", 2, param_slot_6 };
    static const QUParameter param_slot_7[] = {
	{ 0, &static_QUType_varptr, "\x02", QUParameter::In },
	{ 0, &static_QUType_QVariant, 0, QUParameter::In }
    };
    static const QUMethod slot_7 = {"infoData", 2, param_slot_7 };
    static const QUParameter param_slot_8[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In },
	{ 0, &static_QUType_QString, 0, QUParameter::In },
	{ 0, &static_QUType_QVariant, 0, QUParameter::In }
    };
    static const QUMethod slot_8 = {"faultData", 3, param_slot_8 };
    static const QUParameter param_slot_9[] = {
	{ 0, &static_QUType_varptr, "\x02", QUParameter::In },
	{ 0, &static_QUType_QVariant, 0, QUParameter::In }
    };
    static const QUMethod slot_9 = {"transactData", 2, param_slot_9 };
    static const QUMethod slot_10 = {"timerSlot", 0, 0 };
    static const QUParameter param_slot_11[] = {
	{ 0, &static_QUType_varptr, "\x02", QUParameter::In },
	{ 0, &static_QUType_QVariant, 0, QUParameter::In }
    };
    static const QUMethod slot_11 = {"timerData", 2, param_slot_11 };
    static const QUParameter param_slot_12[] = {
	{ 0, &static_QUType_varptr, "\x02", QUParameter::In },
	{ 0, &static_QUType_QVariant, 0, QUParameter::In }
    };
    static const QUMethod slot_12 = {"abortData", 2, param_slot_12 };
    static const QMetaData slot_tbl[] = {
	{ "catalogData(const QValueList<QVariant>&,const QVariant&)", &slot_0, QMetaData::Private },
	{ "identityData(const QValueList<QVariant>&,const QVariant&)", &slot_1, QMetaData::Private },
	{ "catalogSubData(const QValueList<QVariant>&,const QVariant&)", &slot_2, QMetaData::Private },
	{ "serviceData(const QValueList<QVariant>&,const QVariant&)", &slot_3, QMetaData::Private },
	{ "updateData(const QValueList<QVariant>&,const QVariant&)", &slot_4, QMetaData::Private },
	{ "patchData(const QValueList<QVariant>&,const QVariant&)", &slot_5, QMetaData::Private },
	{ "lockData(const QValueList<QVariant>&,const QVariant&)", &slot_6, QMetaData::Private },
	{ "infoData(const QValueList<QVariant>&,const QVariant&)", &slot_7, QMetaData::Private },
	{ "faultData(int,const QString&,const QVariant&)", &slot_8, QMetaData::Private },
	{ "transactData(const QValueList<QVariant>&,const QVariant&)", &slot_9, QMetaData::Private },
	{ "timerSlot()", &slot_10, QMetaData::Private },
	{ "timerData(const QValueList<QVariant>&,const QVariant&)", &slot_11, QMetaData::Private },
	{ "abortData(const QValueList<QVariant>&,const QVariant&)", &slot_12, QMetaData::Private }
    };
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_ptr, "QValueList<Identity>", QUParameter::In }
    };
    static const QUMethod signal_0 = {"userListing", 1, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ 0, &static_QUType_ptr, "QValueList<Service>", QUParameter::In }
    };
    static const QUMethod signal_1 = {"serviceListing", 1, param_signal_1 };
    static const QUParameter param_signal_2[] = {
	{ 0, &static_QUType_ptr, "QValueList<Catalog>", QUParameter::In }
    };
    static const QUMethod signal_2 = {"catalogListing", 1, param_signal_2 };
    static const QUParameter param_signal_3[] = {
	{ 0, &static_QUType_ptr, "QValueList<Patch>", QUParameter::In }
    };
    static const QUMethod signal_3 = {"patchListing", 1, param_signal_3 };
    static const QUParameter param_signal_4[] = {
	{ 0, &static_QUType_ptr, "QValueList<Package>", QUParameter::In }
    };
    static const QUMethod signal_4 = {"updateListing", 1, param_signal_4 };
    static const QUParameter param_signal_5[] = {
	{ 0, &static_QUType_ptr, "QValueList<PackageLock>", QUParameter::In }
    };
    static const QUMethod signal_5 = {"lockListing", 1, param_signal_5 };
    static const QUParameter param_signal_6[] = {
	{ 0, &static_QUType_ptr, "Package", QUParameter::In }
    };
    static const QUMethod signal_6 = {"packageInfo", 1, param_signal_6 };
    static const QUParameter param_signal_7[] = {
	{ 0, &static_QUType_ptr, "PackageDetails", QUParameter::In }
    };
    static const QUMethod signal_7 = {"packageDetails", 1, param_signal_7 };
    static const QUParameter param_signal_8[] = {
	{ "installs", &static_QUType_ptr, "QValueList<Package>", QUParameter::In },
	{ "removals", &static_QUType_ptr, "QValueList<Package>", QUParameter::In },
	{ "updates", &static_QUType_ptr, "QValueList<Package>", QUParameter::In }
    };
    static const QUMethod signal_8 = {"realPackages", 3, param_signal_8 };
    static const QUParameter param_signal_9[] = {
	{ "flags", &static_QUType_int, 0, QUParameter::In },
	{ "errorMessage", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_9 = {"transactionFinished", 2, param_signal_9 };
    static const QUParameter param_signal_10[] = {
	{ "serviceName", &static_QUType_QString, 0, QUParameter::In },
	{ "flags", &static_QUType_int, 0, QUParameter::In },
	{ "errorMessage", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_10 = {"serviceAdded", 3, param_signal_10 };
    static const QUParameter param_signal_11[] = {
	{ "faultMessage", &static_QUType_QString, 0, QUParameter::In },
	{ "errorCode", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_11 = {"generalFault", 2, param_signal_11 };
    static const QUParameter param_signal_12[] = {
	{ 0, &static_QUType_ptr, "Progress", QUParameter::In }
    };
    static const QUMethod signal_12 = {"progress", 1, param_signal_12 };
    static const QUParameter param_signal_13[] = {
	{ 0, &static_QUType_ptr, "Progress", QUParameter::In }
    };
    static const QUMethod signal_13 = {"downloadProgress", 1, param_signal_13 };
    static const QMetaData signal_tbl[] = {
	{ "userListing(QValueList<Identity>)", &signal_0, QMetaData::Public },
	{ "serviceListing(QValueList<Service>)", &signal_1, QMetaData::Public },
	{ "catalogListing(QValueList<Catalog>)", &signal_2, QMetaData::Public },
	{ "patchListing(QValueList<Patch>)", &signal_3, QMetaData::Public },
	{ "updateListing(QValueList<Package>)", &signal_4, QMetaData::Public },
	{ "lockListing(QValueList<PackageLock>)", &signal_5, QMetaData::Public },
	{ "packageInfo(Package)", &signal_6, QMetaData::Public },
	{ "packageDetails(PackageDetails)", &signal_7, QMetaData::Public },
	{ "realPackages(QValueList<Package>,QValueList<Package>,QValueList<Package>)", &signal_8, QMetaData::Public },
	{ "transactionFinished(int,QString)", &signal_9, QMetaData::Public },
	{ "serviceAdded(QString,int,QString)", &signal_10, QMetaData::Public },
	{ "generalFault(QString,int)", &signal_11, QMetaData::Public },
	{ "progress(Progress)", &signal_12, QMetaData::Public },
	{ "downloadProgress(Progress)", &signal_13, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"ZmdUpdaterCore", parentObject,
	slot_tbl, 13,
	signal_tbl, 14,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ZmdUpdaterCore.setMetaObject( metaObj );
    return metaObj;
}

void* ZmdUpdaterCore::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ZmdUpdaterCore" ) )
	return this;
    return QObject::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL userListing
void ZmdUpdaterCore::userListing( QValueList<Identity> t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 0 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL serviceListing
void ZmdUpdaterCore::serviceListing( QValueList<Service> t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 1 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL catalogListing
void ZmdUpdaterCore::catalogListing( QValueList<Catalog> t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 2 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL patchListing
void ZmdUpdaterCore::patchListing( QValueList<Patch> t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 3 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL updateListing
void ZmdUpdaterCore::updateListing( QValueList<Package> t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 4 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL lockListing
void ZmdUpdaterCore::lockListing( QValueList<PackageLock> t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 5 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL packageInfo
void ZmdUpdaterCore::packageInfo( Package t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 6 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL packageDetails
void ZmdUpdaterCore::packageDetails( PackageDetails t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 7 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL realPackages
void ZmdUpdaterCore::realPackages( QValueList<Package> t0, QValueList<Package> t1, QValueList<Package> t2 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 8 );
    if ( !clist )
	return;
    QUObject o[4];
    static_QUType_ptr.set(o+1,&t0);
    static_QUType_ptr.set(o+2,&t1);
    static_QUType_ptr.set(o+3,&t2);
    activate_signal( clist, o );
}

// SIGNAL transactionFinished
void ZmdUpdaterCore::transactionFinished( int t0, QString t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 9 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_int.set(o+1,t0);
    static_QUType_QString.set(o+2,t1);
    activate_signal( clist, o );
}

// SIGNAL serviceAdded
void ZmdUpdaterCore::serviceAdded( QString t0, int t1, QString t2 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 10 );
    if ( !clist )
	return;
    QUObject o[4];
    static_QUType_QString.set(o+1,t0);
    static_QUType_int.set(o+2,t1);
    static_QUType_QString.set(o+3,t2);
    activate_signal( clist, o );
}

// SIGNAL generalFault
void ZmdUpdaterCore::generalFault( QString t0, int t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 11 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_QString.set(o+1,t0);
    static_QUType_int.set(o+2,t1);
    activate_signal( clist, o );
}

// SIGNAL progress
void ZmdUpdaterCore::progress( Progress t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 12 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL downloadProgress
void ZmdUpdaterCore::downloadProgress( Progress t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 13 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

bool ZmdUpdaterCore::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: catalogData((const QValueList<QVariant>&)*((const QValueList<QVariant>*)static_QUType_ptr.get(_o+1)),(const QVariant&)static_QUType_QVariant.get(_o+2)); break;
    case 1: identityData((const QValueList<QVariant>&)*((const QValueList<QVariant>*)static_QUType_ptr.get(_o+1)),(const QVariant&)static_QUType_QVariant.get(_o+2)); break;
    case 2: catalogSubData((const QValueList<QVariant>&)*((const QValueList<QVariant>*)static_QUType_ptr.get(_o+1)),(const QVariant&)static_QUType_QVariant.get(_o+2)); break;
    case 3: serviceData((const QValueList<QVariant>&)*((const QValueList<QVariant>*)static_QUType_ptr.get(_o+1)),(const QVariant&)static_QUType_QVariant.get(_o+2)); break;
    case 4: updateData((const QValueList<QVariant>&)*((const QValueList<QVariant>*)static_QUType_ptr.get(_o+1)),(const QVariant&)static_QUType_QVariant.get(_o+2)); break;
    case 5: patchData((const QValueList<QVariant>&)*((const QValueList<QVariant>*)static_QUType_ptr.get(_o+1)),(const QVariant&)static_QUType_QVariant.get(_o+2)); break;
    case 6: lockData((const QValueList<QVariant>&)*((const QValueList<QVariant>*)static_QUType_ptr.get(_o+1)),(const QVariant&)static_QUType_QVariant.get(_o+2)); break;
    case 7: infoData((const QValueList<QVariant>&)*((const QValueList<QVariant>*)static_QUType_ptr.get(_o+1)),(const QVariant&)static_QUType_QVariant.get(_o+2)); break;
    case 8: faultData((int)static_QUType_int.get(_o+1),(const QString&)static_QUType_QString.get(_o+2),(const QVariant&)static_QUType_QVariant.get(_o+3)); break;
    case 9: transactData((const QValueList<QVariant>&)*((const QValueList<QVariant>*)static_QUType_ptr.get(_o+1)),(const QVariant&)static_QUType_QVariant.get(_o+2)); break;
    case 10: timerSlot(); break;
    case 11: timerData((const QValueList<QVariant>&)*((const QValueList<QVariant>*)static_QUType_ptr.get(_o+1)),(const QVariant&)static_QUType_QVariant.get(_o+2)); break;
    case 12: abortData((const QValueList<QVariant>&)*((const QValueList<QVariant>*)static_QUType_ptr.get(_o+1)),(const QVariant&)static_QUType_QVariant.get(_o+2)); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ZmdUpdaterCore::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: userListing((QValueList<Identity>)(*((QValueList<Identity>*)static_QUType_ptr.get(_o+1)))); break;
    case 1: serviceListing((QValueList<Service>)(*((QValueList<Service>*)static_QUType_ptr.get(_o+1)))); break;
    case 2: catalogListing((QValueList<Catalog>)(*((QValueList<Catalog>*)static_QUType_ptr.get(_o+1)))); break;
    case 3: patchListing((QValueList<Patch>)(*((QValueList<Patch>*)static_QUType_ptr.get(_o+1)))); break;
    case 4: updateListing((QValueList<Package>)(*((QValueList<Package>*)static_QUType_ptr.get(_o+1)))); break;
    case 5: lockListing((QValueList<PackageLock>)(*((QValueList<PackageLock>*)static_QUType_ptr.get(_o+1)))); break;
    case 6: packageInfo((Package)(*((Package*)static_QUType_ptr.get(_o+1)))); break;
    case 7: packageDetails((PackageDetails)(*((PackageDetails*)static_QUType_ptr.get(_o+1)))); break;
    case 8: realPackages((QValueList<Package>)(*((QValueList<Package>*)static_QUType_ptr.get(_o+1))),(QValueList<Package>)(*((QValueList<Package>*)static_QUType_ptr.get(_o+2))),(QValueList<Package>)(*((QValueList<Package>*)static_QUType_ptr.get(_o+3)))); break;
    case 9: transactionFinished((int)static_QUType_int.get(_o+1),(QString)static_QUType_QString.get(_o+2)); break;
    case 10: serviceAdded((QString)static_QUType_QString.get(_o+1),(int)static_QUType_int.get(_o+2),(QString)static_QUType_QString.get(_o+3)); break;
    case 11: generalFault((QString)static_QUType_QString.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 12: progress((Progress)(*((Progress*)static_QUType_ptr.get(_o+1)))); break;
    case 13: downloadProgress((Progress)(*((Progress*)static_QUType_ptr.get(_o+1)))); break;
    default:
	return QObject::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool ZmdUpdaterCore::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool ZmdUpdaterCore::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
