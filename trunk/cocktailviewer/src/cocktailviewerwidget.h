/***************************************************************************
 *   Copyright (C) 2005 by Benni   *
 *   benjamin.loewe@freenet.de   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#ifndef _COCKTAILVIEWERWIDGET_H_
#define _COCKTAILVIEWERWIDGET_H_

#include "cocktailviewerwidgetbase.h"
#include <sqlite3.h>
#include <qlistview.h>

class cocktailviewerWidget : public cocktailviewerWidgetBase
{
    Q_OBJECT

public:
    cocktailviewerWidget(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~cocktailviewerWidget();
    /*$PUBLIC_FUNCTIONS$*/

    char **FilterResult1, **FilterResult2, **FilterResult3, **FilterResult4;
    int nrowFilterResult1, nrowFilterResult2, nrowFilterResult3, nrowFilterResult4;

public slots:
    /*$PUBLIC_SLOTS$*/
    void UpdateListView1();
    void ListView1Clicked();
    void ComboBox1Changed();
    void ComboBox2Changed();
    void ComboBox3Changed();
    void ComboBox4Changed();

protected:
    /*$PROTECTED_FUNCTIONS$*/
    void createTMPCocktailExtras();
    QString getitFromID(QString ID, QString table, QString value);
    QString getID(QString table, QString value, QString string);
    QString printStars(int rating);
    void createFilteredCocktaillist(QComboBox *box, char **result, int &nrow );
    bool checkFilterlist(QString ID, char **result, int &nrow);

    sqlite3 *db;

protected slots:
    /*$PROTECTED_SLOTS$*/

};

class MyListViewItem : public QListViewItem
    {
    public:
	MyListViewItem( QListView* parent )
		: QListViewItem(parent) {}
	int compare ( QListViewItem * i, int col, bool ascending ) const;
    };

#endif

