/****************************************************************************
** myclass1 meta object code from reading C++ file 'myClass1.h'
**
** Created: Mon Oct 22 10:32:54 2007
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "myClass1.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *myclass1::className() const
{
    return "myclass1";
}

QMetaObject *myclass1::metaObj = 0;
static QMetaObjectCleanUp cleanUp_myclass1( "myclass1", &myclass1::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString myclass1::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "myclass1", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString myclass1::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "myclass1", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* myclass1::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"displayDecimal", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"displayHex", 1, param_slot_1 };
    static const QMetaData slot_tbl[] = {
	{ "displayDecimal(int)", &slot_0, QMetaData::Public },
	{ "displayHex(int)", &slot_1, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"myclass1", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_myclass1.setMetaObject( metaObj );
    return metaObj;
}

void* myclass1::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "myclass1" ) )
	return this;
    return QObject::qt_cast( clname );
}

bool myclass1::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: displayDecimal((int)static_QUType_int.get(_o+1)); break;
    case 1: displayHex((int)static_QUType_int.get(_o+1)); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool myclass1::qt_emit( int _id, QUObject* _o )
{
    return QObject::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool myclass1::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool myclass1::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
