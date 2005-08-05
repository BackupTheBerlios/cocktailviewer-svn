/***************************************************************************
 *   Copyright (C) 2005 by Benni                                           *
 *   benjamin.loewe@freenet.de                                             *
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
#ifndef COCKTAILEDITORWIDGET_H
#define COCKTAILEDITORWIDGET_H

#include "cocktailviewerwidget.h"
#include <cocktaileditorwidgetbase.h>

/**
@author Benni
*/
class cocktaileditorwidget : public cocktaileditorwidgetbase
{
public:
    cocktaileditorwidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    void init();
    void writeTastes();
    void writeTypes();
    void writeUnits();
    
    ~cocktaileditorwidget();
    
    sqlite3 *db3;
    
public slots:
    void writeIngredients();

};

#endif
