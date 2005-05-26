/****************************************************************************
** Form interface generated from reading ui file 'cocktailviewerwidgetbase.ui'
**
** Created: Di Mai 24 16:24:53 2005
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef COCKTAILVIEWERWIDGETBASE_H
#define COCKTAILVIEWERWIDGETBASE_H

#include <qvariant.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QPushButton;
class QLineEdit;
class QCheckBox;
class QListView;
class QListViewItem;
class QFrame;
class QComboBox;

class cocktailviewerWidgetBase : public QWidget
{
    Q_OBJECT

public:
    cocktailviewerWidgetBase( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~cocktailviewerWidgetBase();

    QLabel* textLabel1_2;
    QPushButton* pushButton1;
    QLineEdit* lineEdit1;
    QCheckBox* checkBox1;
    QListView* listView1;
    QFrame* frame3;
    QFrame* frame4;
    QLabel* textLabel1;
    QComboBox* comboBox2;
    QComboBox* comboBox3;
    QComboBox* comboBox4;
    QComboBox* comboBox1;
    QLabel* textLabel1_4;
    QLabel* textLabel1_3;

public slots:
    virtual void ListView1Clicked();
    virtual void UpdateListView1();
    virtual void ComboBox1Changed();
    virtual void ComboBox2Changed();
    virtual void ComboBox3Changed();
    virtual void ComboBox4Changed();

protected:
    QGridLayout* cocktailviewerwidgetbaseLayout;
    QVBoxLayout* layout23;
    QHBoxLayout* layout1;
    QGridLayout* frame3Layout;
    QVBoxLayout* layout5;

protected slots:
    virtual void languageChange();

};

#endif // COCKTAILVIEWERWIDGETBASE_H
