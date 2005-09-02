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
	newID = false;
	ID = loadID;
	openDB();
	loadCocktailBasics( loadID );
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
	newID = true;
	openDB();
	newCocktail();
}

void Cocktail::loadCocktailBasics( QString ID)
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
	FloatList::iterator iterator;
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
	StringList::iterator iterator;
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
	StringList::iterator iterator;
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

void Cocktail::calculateExtras( FloatList AmountList , StringList UnitList, StringList NameList )
{
	FloatList::iterator AmountIterator;
	StringList::iterator UnitIterator, NameIterator;
	AmountIterator=AmountList.begin();
	UnitIterator=UnitList.begin();
	Available=TRUE;
	Amount=0;
	Price=0;
	AbsolutAlc=0;
	RelativeAlc=0;
	for(NameIterator=NameList.begin();NameIterator!=NameList.end();NameIterator++)
	{
		QString UnitID=getID( "units", "unit", *UnitIterator );
		float UnitFactor=getitFromID( UnitID, "units", "mlfactor" ).toFloat();
		Amount+=*AmountIterator*UnitFactor;
		
		QString IngredientID=getID( "ingredients", "name", *NameIterator );
		float IngredientAmount=getitFromID( IngredientID, "ingredients", "amount" ).toFloat();
		float IngredientPrice=getitFromID( IngredientID, "ingredients", "price" ).toFloat();
		float IngredientAlcohol=getitFromID( IngredientID, "ingredients", "alcohol" ).toFloat();
		
		if(IngredientAmount!=0)
			Price+=(*AmountIterator*UnitFactor/IngredientAmount)*IngredientPrice;
			
		AbsolutAlc+=*AmountIterator*UnitFactor*IngredientAlcohol/100;
		
		bool IngredientAvailable=( IngredientID=="-1" || getitFromID( IngredientID, "ingredients", "stock" )!="0" ? true : false );
		if( IngredientAvailable == FALSE )
			Available=FALSE;
		
		UnitIterator++;
		AmountIterator++;
	}
	RelativeAlc=AbsolutAlc/Amount;
	/*qDebug("Cocktailmenge: "+QString::number(Amount)+"ml");
	qDebug("Alkohol: "+QString::number(RelativeAlc)+"%, "+QString::number(AbsolutAlc)+"ml");
	qDebug("Preis: "+QString::number(Price)+" EUR");
	QString machbar=(Available ? "ja" : "nein");
	qDebug("Machbar: "+machbar);*/
}

void Cocktail::setName( QString newName )
{
	Name=newName;
}

void Cocktail::setTaste1( QString newTaste )
{
	Taste1=newTaste;
}

void Cocktail::setTaste2( QString newTaste )
{
	Taste2=newTaste;
}

void Cocktail::setType( QString newType )
{
	Type=newType;
}

void Cocktail::setRating( int newRating )
{
	Rating=newRating;
}

void Cocktail::setDescription( QString newDescription )
{
	Description=newDescription;
}

void Cocktail::saveCocktail()
{
	saveCocktailBasics();
	saveCocktailIngredients();
	saveTMPCocktailExtras();
}

void Cocktail::saveCocktailBasics()
{
	QString Data;
	char *zErrMsg = 0;
	int rc;
	if( newID )
		rc = sqlite3_exec(db, "INSERT INTO Cocktails VALUES("+ID+",\""+Name+"\",\""+Description+"\","+QString::number(Rating)+","+getID("types", "type", Type)+","+getID("tastes", "taste", Taste1)+","+getID("tastes", "taste", Taste2)+")", 0, 0, &zErrMsg);
	else
	{
		rc = sqlite3_exec(db, "UPDATE Cocktails SET name=\""+Name+"\", description=\""+Description+"\", rating="+QString::number(Rating)+", typeID="+getID("types", "type", Type)+", taste1ID="+getID("tastes", "taste", Taste1)+", taste2ID="+getID("tastes", "taste", Taste2)+" WHERE ID="+ID, 0, 0, &zErrMsg);
		/*rc = sqlite3_exec(db, "UPDATE Cocktails SET description=\""+Description+"\" WHERE ID="+ID, 0, 0, &zErrMsg);
		rc = sqlite3_exec(db, "UPDATE Cocktails SET rating="+QString::number(Rating)+" WHERE ID="+ID, 0, 0, &zErrMsg);
		rc = sqlite3_exec(db, "UPDATE Cocktails SET typeID="+getID("types", "type", Type)+" WHERE ID="+ID, 0, 0, &zErrMsg);
		rc = sqlite3_exec(db, "UPDATE Cocktails SET taste1ID="+getID("tastes", "taste", Taste1)+" WHERE ID="+ID, 0, 0, &zErrMsg);
		rc = sqlite3_exec(db, "UPDATE Cocktails SET taste2ID="+getID("tastes", "taste", Taste2)+" WHERE ID="+ID, 0, 0, &zErrMsg);*/
	}
	if( rc!=SQLITE_OK )
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	}
}

void Cocktail::saveCocktailIngredients()
{
	FloatList::iterator AmountIterator;
	StringList::iterator UnitIterator, NameIterator;
	AmountIterator = IngredientAmounts.begin();
	UnitIterator = IngredientUnits.begin();
	NameIterator = IngredientNames.begin();
	char *zErrMsg = 0;
	int rc;
	rc = sqlite3_exec(db, "DELETE FROM CocktailIngredients WHERE CocktailID=\""+ID+"\"", 0, 0, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	}
	sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, &zErrMsg);
	while( AmountIterator!=IngredientAmounts.end() && NameIterator!=IngredientNames.end() )
	{
		QString Amount=QString::number( *AmountIterator );
		QString UnitID=getID( "units", "unit", *UnitIterator );
		QString IngredientID=getID( "Ingredients", "Name", *NameIterator );
		if( Amount!="0" && IngredientID!="-1" )
		{
			rc = sqlite3_exec(db, "INSERT INTO CocktailIngredients VALUES(NULL,"+ID+","+Amount+","+UnitID+","+IngredientID+")", 0, 0, &zErrMsg);
			if( rc!=SQLITE_OK )
			{
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
			}
		}
		AmountIterator++;
		UnitIterator++;
		NameIterator++;
	}
	sqlite3_exec(db, "COMMIT TRANSACTION", 0, 0, &zErrMsg);
}

void Cocktail::saveTMPCocktailExtras()
{
	char *zErrMsg = 0;
	int rc;
	rc = sqlite3_exec(db, "DELETE FROM TMPCocktailExtras WHERE CocktailID=\""+ID+"\"", 0, 0, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	}
	rc = sqlite3_exec(db, "INSERT INTO TMPCocktailExtras VALUES(NULL,"+ID+","+(Available ? "1" : "0")+","+QString::number(Amount)+","+QString::number(Price)+","+QString::number(AbsolutAlc)+","+QString::number(RelativeAlc)+")", 0, 0, &zErrMsg);
	//qDebug(ID+","+(Available ? "1" : "0")+","+QString::number(Amount)+","+QString::number(Price)+","+QString::number(AbsolutAlc)+","+QString::number(RelativeAlc));
	if( rc!=SQLITE_OK )
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	}
}

void Cocktail::newCocktail()
{
	char *zErrMsg = 0;
	int rc, nrow, ncolumn;
	char **Result;
	rc = sqlite3_get_table(db, "SELECT ID FROM Cocktails ORDER BY ID", &Result, &nrow, &ncolumn, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	}
	if(nrow>0)
		ID=( QString::number( QString( Result[nrow] ).toInt()+1 ) );
}

Cocktail::~Cocktail()
{
	sqlite3_close(db);
}
