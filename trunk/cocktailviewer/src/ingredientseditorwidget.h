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
#ifndef INGREDIENTSEDITORWIDGET_H
#define INGREDIENTSEDITORWIDGET_H

#include "cocktailviewerwidget.h"
#include "ingredientseditorwidgetbase.h"
#include <qiconset.h>


class ingredientseditorwidget : public ingredientseditorwidgetbase
{
    //Q_OBJECT

public:
    ingredientseditorwidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    void saveIngredientsToDB();
    QString getNewIngredientID();
    bool isDirty();

    ~ingredientseditorwidget();
    sqlite3 *db2;
    int countNew;
    bool dirty;

protected:
    //QPixmap green, red;
    QIconSet imageRed, imageGreen;

public slots:
    void addIngredientClicked();
    void deleteIngredientClicked();
    void OkClicked();
    void tableChanged();
    void tableClicked(int row);

};

#endif
