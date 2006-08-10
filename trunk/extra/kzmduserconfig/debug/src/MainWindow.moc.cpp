/****************************************************************************
** MainWindow meta object code from reading C++ file 'MainWindow.h'
**
** Created: Thu Aug 10 06:19:45 2006
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../src/MainWindow.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *MainWindow::className() const
{
    return "MainWindow";
}

QMetaObject *MainWindow::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MainWindow( "MainWindow", &MainWindow::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MainWindow::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MainWindow", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MainWindow::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MainWindow", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MainWindow::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUMethod slot_0 = {"addButtonClicked", 0, 0 };
    static const QUMethod slot_1 = {"removeButtonClicked", 0, 0 };
    static const QUMethod slot_2 = {"modifyButtonClicked", 0, 0 };
    static const QUParameter param_slot_3[] = {
	{ 0, &static_QUType_ptr, "QValueList<Identity>", QUParameter::In }
    };
    static const QUMethod slot_3 = {"userData", 1, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In },
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"fault", 2, param_slot_4 };
    static const QMetaData slot_tbl[] = {
	{ "addButtonClicked()", &slot_0, QMetaData::Private },
	{ "removeButtonClicked()", &slot_1, QMetaData::Private },
	{ "modifyButtonClicked()", &slot_2, QMetaData::Private },
	{ "userData(QValueList<Identity>)", &slot_3, QMetaData::Private },
	{ "fault(QString,int)", &slot_4, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MainWindow", parentObject,
	slot_tbl, 5,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MainWindow.setMetaObject( metaObj );
    return metaObj;
}

void* MainWindow::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MainWindow" ) )
	return this;
    return QWidget::qt_cast( clname );
}

bool MainWindow::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: addButtonClicked(); break;
    case 1: removeButtonClicked(); break;
    case 2: modifyButtonClicked(); break;
    case 3: userData((QValueList<Identity>)(*((QValueList<Identity>*)static_QUType_ptr.get(_o+1)))); break;
    case 4: fault((QString)static_QUType_QString.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MainWindow::qt_emit( int _id, QUObject* _o )
{
    return QWidget::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MainWindow::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool MainWindow::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
