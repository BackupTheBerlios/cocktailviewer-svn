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
#include <list>
#include <sqlite3.h>

using namespace std;

class Cocktail
{
public:
	Cocktail( QString loadID );
	Cocktail(  );
	
	QString getName() { return Name; };
	void setName( QString newName );
	
	QString getTaste1() { return Taste1; };
	//QString getTaste1ID() { return Taste1ID; };
	void setTaste1( QString newTaste );
	
	QString getTaste2() { return Taste2; };
	//QString getTaste2ID() { return Taste2ID; };
	void setTaste2( QString newTaste );
	
	QString getType() { return Type; };
	//QString getTypeID() { return TypeID; };
	void setType( QString newType );
	
	int getRating() { return Rating; };
	void setRating( int newRating );
	
	QString getDescription() { return Description; };
	void setDescription( QString newDescription );
	
	int getNumberOfIngredients() { return NumberOfIngredients; };
	
	float getIngredientAmount( int index );
	void setIngredientAmounts( list<float> AmountList );
	
	QString getIngredientUnit( int index );
	void setIngredientUnits( list<QString> UnitList );
	
	QString getIngredientName( int index );
	void setIngredientNames( list<QString> NameList );
	
	bool getAvailable() { return Available; };
	float getAmount() { return Amount; };
	float getPrice() { return Price; };
	float getAbsolutAlc() { return AbsolutAlc; };
	float getRelativeAlc() { return RelativeAlc; };
	
	void calculateExtras( list<float> AmountList , list<QString> UnitList, list<QString> NameList );
	
	void saveCocktail();
	
	~Cocktail();

private:
	void openDB();
	void loadCocktailBasics( QString ID );
	void loadCocktailIngredients( QString ID );
	void loadTMPCocktailExtras( QString ID );
	void newCocktail();
	QString getitFromID(QString ID, QString table, QString value);
	QString getID(QString table, QString value, QString string);
	void saveCocktailBasics();
	void saveCocktailIngredients();
	void saveTMPCocktailExtras();
	
	sqlite3 *db;
	QString ID, Name, Taste1, Taste1ID, Taste2, Taste2ID, Type, TypeID, Description;
	int Rating, NumberOfIngredients;
	bool Available;
	float Amount, Price, AbsolutAlc, RelativeAlc;
	
	typedef list<QString> StringList;
	typedef list<float> FloatList;
	StringList IngredientUnits, IngredientNames;
	FloatList IngredientAmounts;
};

#endif
