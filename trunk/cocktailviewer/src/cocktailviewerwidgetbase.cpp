/****************************************************************************
** Form implementation generated from reading ui file 'cocktailviewerwidgetbase.ui'
**
** Created: Di Mai 24 16:25:02 2005
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "cocktailviewerwidgetbase.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qheader.h>
#include <qlistview.h>
#include <qframe.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a cocktailviewerWidgetBase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
cocktailviewerWidgetBase::cocktailviewerWidgetBase( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "cocktailviewerwidgetbase" );
    setMinimumSize( QSize( 750, 590 ) );
    cocktailviewerwidgetbaseLayout = new QGridLayout( this, 1, 1, 2, 2, "cocktailviewerwidgetbaseLayout"); 

    layout23 = new QVBoxLayout( 0, 0, 2, "layout23"); 

    layout1 = new QHBoxLayout( 0, 0, 2, "layout1"); 

    textLabel1_2 = new QLabel( this, "textLabel1_2" );
    QFont textLabel1_2_font(  textLabel1_2->font() );
    textLabel1_2_font.setBold( TRUE );
    textLabel1_2->setFont( textLabel1_2_font ); 
    layout1->addWidget( textLabel1_2 );

    pushButton1 = new QPushButton( this, "pushButton1" );
    layout1->addWidget( pushButton1 );

    lineEdit1 = new QLineEdit( this, "lineEdit1" );
    layout1->addWidget( lineEdit1 );

    checkBox1 = new QCheckBox( this, "checkBox1" );
    layout1->addWidget( checkBox1 );
    layout23->addLayout( layout1 );

    listView1 = new QListView( this, "listView1" );
    listView1->addColumn( tr( "Cocktail" ) );
    listView1->addColumn( tr( "rating" ) );
    listView1->addColumn( tr( "price (EUR)" ) );
    listView1->addColumn( tr( "amount (ml)" ) );
    listView1->addColumn( tr( "alcohol (%)" ) );
    listView1->addColumn( tr( "alcohol (ml)" ) );
    listView1->setAllColumnsShowFocus( TRUE );
    listView1->setShowSortIndicator( TRUE );
    layout23->addWidget( listView1 );

    frame3 = new QFrame( this, "frame3" );
    frame3->setMinimumSize( QSize( 400, 320 ) );
    frame3->setFrameShape( QFrame::NoFrame );
    frame3->setFrameShadow( QFrame::Raised );
    frame3Layout = new QGridLayout( frame3, 1, 1, 2, 2, "frame3Layout"); 

    frame4 = new QFrame( frame3, "frame4" );
    frame4->setMaximumSize( QSize( 200, 32767 ) );
    frame4->setFrameShape( QFrame::StyledPanel );
    frame4->setFrameShadow( QFrame::Raised );

    textLabel1 = new QLabel( frame4, "textLabel1" );
    textLabel1->setGeometry( QRect( 6, 6, 190, 25 ) );
    QFont textLabel1_font(  textLabel1->font() );
    textLabel1_font.setPointSize( 14 );
    textLabel1->setFont( textLabel1_font ); 
    textLabel1->setAlignment( int( QLabel::AlignCenter ) );

    comboBox2 = new QComboBox( FALSE, frame4, "comboBox2" );
    comboBox2->setGeometry( QRect( 15, 65, 170, 20 ) );
    comboBox2->setEditable( TRUE );

    comboBox3 = new QComboBox( FALSE, frame4, "comboBox3" );
    comboBox3->setGeometry( QRect( 15, 95, 170, 20 ) );
    comboBox3->setEditable( TRUE );

    comboBox4 = new QComboBox( FALSE, frame4, "comboBox4" );
    comboBox4->setGeometry( QRect( 15, 125, 170, 20 ) );
    comboBox4->setEditable( TRUE );

    comboBox1 = new QComboBox( FALSE, frame4, "comboBox1" );
    comboBox1->setGeometry( QRect( 15, 35, 170, 20 ) );
    comboBox1->setEditable( TRUE );

    frame3Layout->addWidget( frame4, 0, 0 );

    layout5 = new QVBoxLayout( 0, 0, 2, "layout5"); 

    textLabel1_4 = new QLabel( frame3, "textLabel1_4" );
    textLabel1_4->setMinimumSize( QSize( 0, 63 ) );
    textLabel1_4->setMaximumSize( QSize( 32767, 63 ) );
    QFont textLabel1_4_font(  textLabel1_4->font() );
    textLabel1_4_font.setPointSize( 18 );
    textLabel1_4_font.setBold( TRUE );
    textLabel1_4->setFont( textLabel1_4_font ); 
    textLabel1_4->setFrameShape( QLabel::StyledPanel );
    textLabel1_4->setFrameShadow( QLabel::Raised );
    textLabel1_4->setAlignment( int( QLabel::WordBreak | QLabel::AlignTop | QLabel::AlignHCenter ) );
    layout5->addWidget( textLabel1_4 );

    textLabel1_3 = new QLabel( frame3, "textLabel1_3" );
    textLabel1_3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, textLabel1_3->sizePolicy().hasHeightForWidth() ) );
    QFont textLabel1_3_font(  textLabel1_3->font() );
    textLabel1_3_font.setPointSize( 16 );
    textLabel1_3_font.setBold( TRUE );
    textLabel1_3->setFont( textLabel1_3_font ); 
    textLabel1_3->setFrameShape( QLabel::StyledPanel );
    textLabel1_3->setFrameShadow( QLabel::Raised );
    textLabel1_3->setAlignment( int( QLabel::AlignTop | QLabel::AlignHCenter ) );
    layout5->addWidget( textLabel1_3 );

    frame3Layout->addLayout( layout5, 0, 1 );
    layout23->addWidget( frame3 );

    cocktailviewerwidgetbaseLayout->addLayout( layout23, 0, 0 );
    languageChange();
    resize( QSize(797, 590).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( listView1, SIGNAL( currentChanged(QListViewItem*) ), this, SLOT( ListView1Clicked() ) );
    connect( lineEdit1, SIGNAL( textChanged(const QString&) ), this, SLOT( UpdateListView1() ) );
    connect( checkBox1, SIGNAL( clicked() ), this, SLOT( UpdateListView1() ) );
    connect( pushButton1, SIGNAL( clicked() ), lineEdit1, SLOT( clear() ) );
    connect( listView1, SIGNAL( clicked(QListViewItem*) ), this, SLOT( ListView1Clicked() ) );
    connect( comboBox1, SIGNAL( textChanged(const QString&) ), this, SLOT( ComboBox1Changed() ) );
    connect( comboBox2, SIGNAL( textChanged(const QString&) ), this, SLOT( ComboBox2Changed() ) );
    connect( comboBox3, SIGNAL( textChanged(const QString&) ), this, SLOT( ComboBox3Changed() ) );
    connect( comboBox4, SIGNAL( textChanged(const QString&) ), this, SLOT( ComboBox4Changed() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
cocktailviewerWidgetBase::~cocktailviewerWidgetBase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void cocktailviewerWidgetBase::languageChange()
{
    setCaption( tr( "cocktailviewerwidgetbase" ) );
    textLabel1_2->setText( tr( "Search:" ) );
    pushButton1->setText( tr( "X" ) );
    checkBox1->setText( tr( "&show only available cocktails" ) );
    checkBox1->setAccel( QKeySequence( tr( "Alt+S" ) ) );
    listView1->header()->setLabel( 0, tr( "Cocktail" ) );
    listView1->header()->setLabel( 1, tr( "rating" ) );
    listView1->header()->setLabel( 2, tr( "price (EUR)" ) );
    listView1->header()->setLabel( 3, tr( "amount (ml)" ) );
    listView1->header()->setLabel( 4, tr( "alcohol (%)" ) );
    listView1->header()->setLabel( 5, tr( "alcohol (ml)" ) );
    textLabel1->setText( tr( "Filter" ) );
    textLabel1_4->setText( QString::null );
    textLabel1_3->setText( QString::null );
}

void cocktailviewerWidgetBase::ListView1Clicked()
{
    qWarning( "cocktailviewerWidgetBase::ListView1Clicked(): Not implemented yet" );
}

void cocktailviewerWidgetBase::UpdateListView1()
{
    qWarning( "cocktailviewerWidgetBase::UpdateListView1(): Not implemented yet" );
}

void cocktailviewerWidgetBase::ComboBox1Changed()
{
    qWarning( "cocktailviewerWidgetBase::ComboBox1Changed(): Not implemented yet" );
}

void cocktailviewerWidgetBase::ComboBox2Changed()
{
    qWarning( "cocktailviewerWidgetBase::ComboBox2Changed(): Not implemented yet" );
}

void cocktailviewerWidgetBase::ComboBox3Changed()
{
    qWarning( "cocktailviewerWidgetBase::ComboBox3Changed(): Not implemented yet" );
}

void cocktailviewerWidgetBase::ComboBox4Changed()
{
    qWarning( "cocktailviewerWidgetBase::ComboBox4Changed(): Not implemented yet" );
}

