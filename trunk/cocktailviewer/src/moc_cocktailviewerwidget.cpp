/****************************************************************************
** cocktailviewerWidget meta object code from reading C++ file 'cocktailviewerwidget.h'
**
** Created: Tue May 24 16:32:41 2005
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.4   edited Jan 21 18:14 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "cocktailviewerwidget.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *cocktailviewerWidget::className() const
{
    return "cocktailviewerWidget";
}

QMetaObject *cocktailviewerWidget::metaObj = 0;
static QMetaObjectCleanUp cleanUp_cocktailviewerWidget( "cocktailviewerWidget", &cocktailviewerWidget::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString cocktailviewerWidget::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "cocktailviewerWidget", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString cocktailviewerWidget::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "cocktailviewerWidget", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* cocktailviewerWidget::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = cocktailviewerWidgetBase::staticMetaObject();
    static const QUMethod slot_0 = {"UpdateListView1", 0, 0 };
    static const QUMethod slot_1 = {"ListView1Clicked", 0, 0 };
    static const QUMethod slot_2 = {"ComboBox1Changed", 0, 0 };
    static const QUMethod slot_3 = {"ComboBox2Changed", 0, 0 };
    static const QUMethod slot_4 = {"ComboBox3Changed", 0, 0 };
    static const QUMethod slot_5 = {"ComboBox4Changed", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "UpdateListView1()", &slot_0, QMetaData::Public },
	{ "ListView1Clicked()", &slot_1, QMetaData::Public },
	{ "ComboBox1Changed()", &slot_2, QMetaData::Public },
	{ "ComboBox2Changed()", &slot_3, QMetaData::Public },
	{ "ComboBox3Changed()", &slot_4, QMetaData::Public },
	{ "ComboBox4Changed()", &slot_5, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"cocktailviewerWidget", parentObject,
	slot_tbl, 6,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_cocktailviewerWidget.setMetaObject( metaObj );
    return metaObj;
}

void* cocktailviewerWidget::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "cocktailviewerWidget" ) )
	return this;
    return cocktailviewerWidgetBase::qt_cast( clname );
}

bool cocktailviewerWidget::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: UpdateListView1(); break;
    case 1: ListView1Clicked(); break;
    case 2: ComboBox1Changed(); break;
    case 3: ComboBox2Changed(); break;
    case 4: ComboBox3Changed(); break;
    case 5: ComboBox4Changed(); break;
    default:
	return cocktailviewerWidgetBase::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool cocktailviewerWidget::qt_emit( int _id, QUObject* _o )
{
    return cocktailviewerWidgetBase::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool cocktailviewerWidget::qt_property( int id, int f, QVariant* v)
{
    return cocktailviewerWidgetBase::qt_property( id, f, v);
}

bool cocktailviewerWidget::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
