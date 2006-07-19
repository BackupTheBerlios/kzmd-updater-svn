/****************************************************************************
** UDSTest meta object code from reading C++ file 'udstest.h'
**
** Created: Tue Jul 18 16:09:32 2006
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "udstest.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *UDSTest::className() const
{
    return "UDSTest";
}

QMetaObject *UDSTest::metaObj = 0;
static QMetaObjectCleanUp cleanUp_UDSTest( "UDSTest", &UDSTest::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString UDSTest::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "UDSTest", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString UDSTest::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "UDSTest", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* UDSTest::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ 0, &static_QUType_ptr, "KIO::Job", QUParameter::In },
	{ 0, &static_QUType_varptr, "\x1d", QUParameter::In }
    };
    static const QUMethod slot_0 = {"slotData", 2, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ 0, &static_QUType_ptr, "KIO::Job", QUParameter::In }
    };
    static const QUMethod slot_1 = {"slotResult", 1, param_slot_1 };
    static const QMetaData slot_tbl[] = {
	{ "slotData(KIO::Job*,const QByteArray&)", &slot_0, QMetaData::Private },
	{ "slotResult(KIO::Job*)", &slot_1, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"UDSTest", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_UDSTest.setMetaObject( metaObj );
    return metaObj;
}

void* UDSTest::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "UDSTest" ) )
	return this;
    return QObject::qt_cast( clname );
}

bool UDSTest::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: slotData((KIO::Job*)static_QUType_ptr.get(_o+1),(const QByteArray&)*((const QByteArray*)static_QUType_ptr.get(_o+2))); break;
    case 1: slotResult((KIO::Job*)static_QUType_ptr.get(_o+1)); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool UDSTest::qt_emit( int _id, QUObject* _o )
{
    return QObject::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool UDSTest::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool UDSTest::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
