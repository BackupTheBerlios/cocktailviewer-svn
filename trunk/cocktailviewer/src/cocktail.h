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
#include <qstring.h>
#include <sqlite3.h>

class Cocktail
{
public:
    Cocktail( QString loadID );
    Cocktail(  );

    QString getName();
    void setName( QString Name );

    QString getTaste1();
    void setTaste1( QString Taste );

    QString getTaste2();
    void setTaste2( QString Taste );

    QString getType();
    void setType( QString Type );

    int getRating();
    void setRating( int Rating );

    QString getDescription();
    void setDescription( QString Description );


    //void loadCocktail( QString ID );
    

    ~Cocktail();

    sqlite3 *db3;
    
private:

};
