/****************************************************************************
** myclass2 meta object code from reading C++ file 'myClass2.h'
**
** Created: Mon Oct 22 10:32:55 2007
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "myClass2.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *myclass2::className() const
{
    return "myclass2";
}

QMetaObject *myclass2::metaObj = 0;
static QMetaObjectCleanUp cleanUp_myclass2( "myclass2", &myclass2::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString myclass2::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "myclass2", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString myclass2::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "myclass2", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* myclass2::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"passValue", 1, param_signal_0 };
    static const QMetaData signal_tbl[] = {
	{ "passValue(int)", &signal_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"myclass2", parentObject,
	0, 0,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_myclass2.setMetaObject( metaObj );
    return metaObj;
}

void* myclass2::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "myclass2" ) )
	return this;
    return QObject::qt_cast( clname );
}

// SIGNAL passValue
void myclass2::passValue( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 0, t0 );
}

bool myclass2::qt_invoke( int _id, QUObject* _o )
{
    return QObject::qt_invoke(_id,_o);
}

bool myclass2::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: passValue((int)static_QUType_int.get(_o+1)); break;
    default:
	return QObject::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool myclass2::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool myclass2::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
