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
#ifndef COCKTAIL_H
#define COCKTAIL_H

#include <qstring.h>
#include <qstringlist.h>
#include <sqlite3.h>

using namespace std;

class Cocktail
{
public:
	Cocktail( QString loadID );
	Cocktail(  );
	
	QString getName() { return Name; };
	void setName( QString Name );
	
	QString getTaste1() { return Taste1; };
	QString getTaste1ID() { return Taste1ID; };
	void setTaste1( QString Taste );
	
	QString getTaste2() { return Taste2; };
	QString getTaste2ID() { return Taste2ID; };
	void setTaste2( QString Taste );
	
	QString getType() { return Type; };
	QString getTypeID() { return TypeID; };
	void setType( QString Type );
	
	int getRating() { return Rating; };
	void setRating( int Rating );
	
	QString getDescription() { return Description; };
	void setDescription( QString Description );
	
	int getNumberOfIngredients() { return NumberOfIngredients; };
	
	float getIngredientAmount( int index );
	void setIngredientAmount( int index, QString Amount );
	
	QString getIngredientUnit( int index );
	void setIngredientUnit( int index, QString Unit );
	
	QString getIngredientName( int index );
	void setIngredientName( int index, QString Name );
	
	bool getAvailable() { return Available; };
	float getAmount() { return Amount; };
	float getPrice() { return Price; };
	float getAbsolutAlc() { return AbsolutAlc; };
	float getRelativeAlc() { return RelativeAlc; };
	
	~Cocktail();

private:
	void openDB();
	void loadCocktail( QString ID );
	void loadCocktailIngredients( QString ID );
	void loadTMPCocktailExtras( QString ID );
	void newCocktail();
	QString getitFromID(QString ID, QString table, QString value);
	QString getID(QString table, QString value, QString string);
	
	sqlite3 *db;
	QString Name, Taste1, Taste1ID, Taste2, Taste2ID, Type, TypeID, Description;
	int Rating, NumberOfIngredients;
	bool Available;
	float Amount, Price, AbsolutAlc, RelativeAlc;
	
	typedef list<QString> UnitList, NameList;
	typedef list<float> AmountList;
	UnitList IngredientUnits;
	NameList IngredientNames;
	AmountList IngredientAmounts;
	
	//QStringList IngredientAmounts, IngredientUnits, IngredientNames;
};

#endif
