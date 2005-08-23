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
#include "cocktail.h"

Cocktail::Cocktail( QString loadID )
{
	openDB();
	loadCocktail( loadID );
	loadCocktailIngredients( loadID );
	loadTMPCocktailExtras( loadID );
}

void Cocktail::openDB()
{
	extern QString dir;
	QString dbfile;
	dbfile = dir + "cocktail.db";
	int rc;
	rc = sqlite3_open(dbfile, &db);
	if( rc )
	{
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return;
	}
}

Cocktail::Cocktail()
{
	openDB();
	newCocktail();
}

void Cocktail::loadCocktail( QString ID)
{
	char *zErrMsg = 0;
	int rc, nrow, ncolumn;
	char **Result;
	rc = sqlite3_get_table(db, "SELECT * FROM Cocktails WHERE ID="+ID, &Result, &nrow, &ncolumn, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		qDebug("2");
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	}
	Name=Result[8];
	Description=Result[9];
	Rating=QString(Result[10]).toInt();
	TypeID=Result[11];
	Taste1ID=Result[12];
	Taste2ID=Result[13];
	Type=getitFromID(Result[11], "types", "type");
	Taste1=getitFromID(Result[12], "tastes", "taste");
	Taste2=getitFromID(Result[13], "tastes", "taste");
	sqlite3_free_table(Result);
}

void Cocktail::loadCocktailIngredients( QString ID)
{
	char *zErrMsg = 0;
	int rc, nrow, ncolumn;
	char **Result;
	rc = sqlite3_get_table(db, "SELECT * FROM CocktailIngredients WHERE cocktailID="+ID, &Result, &nrow, &ncolumn, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		qDebug("3");
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	}
	IngredientAmounts.clear();
	IngredientUnits.clear();
	IngredientNames.clear();
	NumberOfIngredients = nrow;
	for(int i=1;i<=nrow;i++)
	{
		IngredientAmounts.push_back( QString( Result[5*i+2] ).toFloat() );
		IngredientUnits.push_back( getitFromID( Result[5*i+3], "units", "unit" ) );
		IngredientNames.push_back( getitFromID( Result[5*i+4], "ingredients", "name" ) );
	}
	sqlite3_free_table(Result);
}

float Cocktail::getIngredientAmount( int index )
{
	if( index > NumberOfIngredients )
		return 0;
	AmountList::iterator iterator;
	iterator=IngredientAmounts.begin();
	for(int i=1;i<index;i++)
	{
		iterator++;
	}
	return *iterator;
}

void Cocktail::setIngredientAmounts( list<float> AmountList )
{
	IngredientAmounts=AmountList;
}

QString Cocktail::getIngredientUnit( int index )
{
	if( index > NumberOfIngredients )
		return "";
	UnitList::iterator iterator;
	iterator=IngredientUnits.begin();
	for(int i=1;i<index;i++)
	{
		iterator++;
	}
	return *iterator;
}

void Cocktail::setIngredientUnits( list<QString> UnitList )
{
	IngredientUnits=UnitList;
}

QString Cocktail::getIngredientName( int index )
{
	if( index > NumberOfIngredients )
		return "";
	NameList::iterator iterator;
	iterator=IngredientNames.begin();
	for(int i=1;i<index;i++)
	{
		iterator++;
	}
	return *iterator;
}

void Cocktail::setIngredientNames( list<QString> NameList)
{
	IngredientNames=NameList;
}

void Cocktail::loadTMPCocktailExtras( QString ID)
{
	char *zErrMsg = 0;
	int rc, nrow, ncolumn;
	char **Result;
	rc = sqlite3_get_table(db, "SELECT * FROM TMPCocktailExtras WHERE cocktailID="+ID, &Result, &nrow, &ncolumn, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		qDebug("4");
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	}
	Available = ( QString(Result[9])=="0" ? false : true );
	Amount = QString(Result[10]).toFloat();
	Price = QString(Result[11]).toFloat();
	AbsolutAlc = QString(Result[12]).toFloat();
	RelativeAlc = QString(Result[13]).toFloat();
	sqlite3_free_table(Result);
}

QString Cocktail::getitFromID(QString ID, QString table, QString value)
{
	char *zErrMsg = 0;
	int rc, nrow, ncolumn;
	char **Result;
	QString ResultString;
	if(ID=="-1")
		ResultString="";
	rc = sqlite3_get_table(db, "SELECT "+value+" FROM "+table+" WHERE ID="+ID, &Result, &nrow, &ncolumn, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		qDebug("6");
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	}
	if(nrow>0)
		ResultString=Result[1];
	sqlite3_free_table(Result);
	return ResultString;
}

QString Cocktail::getID(QString table, QString value, QString string)
{
	QString ID="-1";
	char *zErrMsg = 0;
	int rc, nrow, ncolumn;
	char **Result;
	rc = sqlite3_get_table(db, "SELECT ID FROM "+table+" WHERE "+value+"=\""+string+"\"", &Result, &nrow, &ncolumn, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	}
	if(nrow>0)
		ID=Result[1];
	sqlite3_free_table(Result);
	return ID;
}

void Cocktail::recalculateExtras()
{
	AmountList::iterator AmountIterator;
	UnitList::iterator UnitIterator;
	NameList::iterator NameIterator;
	AmountIterator=IngredientAmounts.begin();
	UnitIterator=IngredientUnits.begin();
	NameIterator=IngredientNames.begin();


}

void Cocktail::saveCocktail()
{

}

void Cocktail::newCocktail()
{

}



Cocktail::~Cocktail()
{

}
