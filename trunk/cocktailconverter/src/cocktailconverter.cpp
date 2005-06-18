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


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

#include <qfile.h>
#include <qtextstream.h>

#include "cocktailconverter.h"

int main(int argc, char *argv[])
{
	QString db;
	dir=argv[0];
	int i=dir.findRev('/',-1);
	dir.remove(i+1, dir.length());
	db=dir+"cocktail.db";
	ImportUnits("Einheiten.txt", db);
	ReadIngredientslist("zutatenliste.txt");
	WriteIngredientslistToSQL(db);
	ReadCocktaillist("cocktailliste.txt");
	ReadWriteCocktailTypes("Cocktailart.txt", db);
	ReadWriteTastes("Cocktailgeschmack.txt", db);
	WriteCocktailsToSQL(db);
	qDebug("Copied "+QString::number(counter)+" cocktails to the database ("+db+").");
	return EXIT_SUCCESS;
}

bool ReadIngredientslist(QString file)
{
	ZutatNode TempZutat;
	Zutaten.clear();
	QString filename=dir+file;
	QFile f( filename );
	if ( !f.open( IO_ReadOnly ) )
	{
		qDebug("Could not open list of ingredients!");
		return false;
	}
	qDebug("Opened list of ingredients. Reading ...");
	QTextStream t( &f );
	t.readLine();
	t.readLine();
	t.readLine();
	while ( !t.atEnd() )
	{
		QString line( t.readLine() );
		QString Data;
		for(int k=0;k<6;k++)
		{
			int i = line.find( 0x09, 0 );
			Data=line.left(i); 
			switch (k)
			{
				case 0:	TempZutat.kennung=Data;
					break;
				case 1:	TempZutat.name=Data;
					break;
				case 2:	TempZutat.menge=Data.replace( QChar(','), "." ).toFloat();
					break;
				case 3:	TempZutat.preis=Data.replace( QChar(','), "." ).toFloat();
					break;
				case 4:	TempZutat.gehalt=Data.replace( QChar(','), "." ).toFloat();
					break;
				case 5:	TempZutat.bestand=Data.replace( QChar(','), "." ).toFloat();
			}
			line.remove(0,i+1);
		}
		//qDebug(TempZutat.kennung+" "+TempZutat.name+" "+QString::number(TempZutat.menge)+" "+QString::number(TempZutat.preis)+" "+QString::number(TempZutat.gehalt)+" "+QString::number(TempZutat.bestand));
		Zutaten.push_back(TempZutat);
	}
	f.close();
	return true;
}

bool WriteIngredientslistToSQL(QString dbfile)
{
	LZutaten::iterator zutat;
	char *zErrMsg = 0;
	int rc;
	rc = sqlite3_open(dbfile, &db);
	if( rc )
	{
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return false;
	}
	rc = sqlite3_exec(db, "DROP TABLE Ingredients;", 0, 0, &zErrMsg);
	rc = sqlite3_exec(db, "CREATE TABLE Ingredients (ID INTEGER PRIMARY KEY, name TEXT, amount NUMERIC, price NUMERIC, alcohol NUMERIC, stock NUMERIC, oldID TEXT);", 0, 0, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	}
	rc = sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, &zErrMsg);
	for(zutat=Zutaten.begin();zutat!=Zutaten.end();zutat++)
	{
		rc = sqlite3_exec(db, "INSERT INTO Ingredients VALUES(NULL,\""+zutat->name+"\","+QString::number(zutat->menge)+","+QString::number(zutat->preis)+","+QString::number(zutat->gehalt)+","+QString::number(zutat->bestand)+",\""+zutat->kennung+"\")", 0, 0, &zErrMsg);
		if( rc!=SQLITE_OK )
		{
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		}
	}
	rc = sqlite3_exec(db, "COMMIT TRANSACTION", 0, 0, &zErrMsg);
	sqlite3_close(db);
	return true;
}

bool ReadCocktaillist(QString file)
{
	CocktailNode tempcocktail;
	CocktailZutatNode tempzutat;
	QString filename=dir+file;
	QFile f( filename );
	if ( !f.open( IO_ReadOnly ) )
	{
		qDebug("Could not open cocktail list!");
		return false;
	}
	qDebug("Opened cocktail list. Reading ...");
	QTextStream t( &f );
	t.readLine();
	t.readLine();
	t.readLine();
	while ( !t.atEnd() )
	{
		QString line( t.readLine() );
		QString Data;
		for(int k=0;k<7;k++)
		{
			int i = line.find( "%§", 0 );
			Data=line.left(i);
			if(Data=="?§")
				Data="";
			switch (k)
			{
				case 0:	tempcocktail.Name=Data;
					break;
				case 1:	tempcocktail.Beschreibung=Data;
					break;
				case 2:	tempcocktail.Benotung=Data.toInt();
					if(tempcocktail.Benotung<1 || tempcocktail.Benotung>5)
						tempcocktail.Benotung=0;
					break;
				case 3:	tempcocktail.CocktailZutaten.clear();
					while ( Data.contains("$§")!=0 )
					{
						for(int l=1;l<=3;l++)
						{
							int j=Data.find( "(§", 0);
							QString Data2=Data.left(j);
							switch (l)
							{
								case 1: tempzutat.menge=Data2.toFloat();
									break;
								case 2: tempzutat.einheit=Data2;
									break;
								case 3: tempzutat.kennung=Data2;
									break;
							}
							Data.remove(0,j+2);
						}
						Data.remove(0,2);
						tempcocktail.CocktailZutaten.push_back(tempzutat);
					}
					break;
				case 4:	tempcocktail.Cocktailart=Data;
					break;
				case 5:	tempcocktail.Geschmack1=Data;
					break;
				case 6:	tempcocktail.Geschmack2=Data;
					break;
			}
			line.remove(0,i+2);
		}
		Cocktails.push_back(tempcocktail);
		/*qDebug(tempcocktail.Name+" "+QString::number(tempcocktail.Benotung)+" "+tempcocktail.Cocktailart+" "+ tempcocktail.Geschmack1+" "+tempcocktail.Geschmack2+"\n"+tempcocktail.Beschreibung);
		LCocktailZutaten::iterator temp;
		for(temp=tempcocktail.CocktailZutaten.begin();temp!=tempcocktail.CocktailZutaten.end();temp++)
		{
			qDebug(QString::number(temp->menge)+" "+temp->einheit+" "+temp->kennung);
		}*/
	}
	return true;
}

bool WriteCocktailsToSQL(QString dbfile)
{
	LCocktails::iterator cocktail;
	LCocktailZutaten::iterator zutat;
	char *zErrMsg = 0;
	int rc;
	counter=0;
	rc = sqlite3_open(dbfile, &db);
	if( rc )
	{
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return false;
	}
	qDebug("Writing cocktails to database ...");
	rc = sqlite3_exec(db, "DROP TABLE Cocktails;", 0, 0, &zErrMsg);
	rc = sqlite3_exec(db, "DROP TABLE CocktailIngredients;", 0, 0, &zErrMsg);
	rc = sqlite3_exec(db, "CREATE TABLE Cocktails (ID INTEGER PRIMARY KEY, name TEXT, description TEXT, rating NUMERIC, typeID NUMERIC, taste1ID NUMERIC, taste2ID NUMERIC);", 0, 0, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	}
	rc = sqlite3_exec(db, "CREATE TABLE CocktailIngredients (ID INTEGER PRIMARY KEY, cocktailID NUMERIC, amount NUMERIC, unitID NUMERIC, IngredientID NUMERIC);", 0, 0, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	}
	rc = sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, &zErrMsg);
	for(cocktail=Cocktails.begin();cocktail!=Cocktails.end();cocktail++)
	{
		rc = sqlite3_exec(db, "INSERT INTO Cocktails VALUES(NULL,\""+cocktail->Name+"\",\""+cocktail->Beschreibung+"\","+QString::number(cocktail->Benotung)+","+getID("types", "type", cocktail->Cocktailart)+","+getID("tastes", "taste", cocktail->Geschmack1)+","+getID("tastes", "taste", cocktail->Geschmack2)+")", 0, 0, &zErrMsg);
		if( rc!=SQLITE_OK )
		{
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		}
		QString CocktailID=QString::number(sqlite3_last_insert_rowid(db));
		for(zutat=cocktail->CocktailZutaten.begin();zutat!=cocktail->CocktailZutaten.end();zutat++)
		{
			if(zutat->einheit=="af")
				zutat->einheit="cl";
			rc = sqlite3_exec(db, "INSERT INTO CocktailIngredients VALUES(NULL,"+CocktailID+","+QString::number(zutat->menge)+","+getID("units", "oldID", zutat->einheit)+","+getID("Ingredients", "oldID", zutat->kennung)+")", 0, 0, &zErrMsg);
			if( rc!=SQLITE_OK )
			{
				qDebug(cocktail->Name);
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
			}
		}
		counter++;
	}
	rc = sqlite3_exec(db, "COMMIT TRANSACTION", 0, 0, &zErrMsg);
	sqlite3_close(db);
	return true;
}

QString getID(QString table, QString value, QString string)
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
	return ID;
}

bool ReadWriteCocktailTypes(QString file, QString dbfile)
{
	QString filename=dir+file;
	QFile f( filename );
	char *zErrMsg = 0;
	int rc;
	rc = sqlite3_open(dbfile, &db);
	if( rc )
	{
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return false;
	}
	rc = sqlite3_exec(db, "DROP TABLE types;", 0, 0, &zErrMsg);
	rc = sqlite3_exec(db, "CREATE TABLE types (ID INTEGER PRIMARY KEY, type TEXT);", 0, 0, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	}
	if ( !f.open( IO_ReadOnly ) )
	{
		qDebug("Could not open list of cocktail types.");
		return false;
	}
	qDebug("Opened list of cocktail types. Reading ...");
	QTextStream t( &f );
	rc = sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, &zErrMsg);
	while ( !t.atEnd() )
	{
		QString line( t.readLine() );
		rc = sqlite3_exec(db, "INSERT INTO types VALUES(NULL,\""+line+"\");", 0, 0, &zErrMsg);
		if( rc!=SQLITE_OK )
		{
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		}
	}
	rc = sqlite3_exec(db, "COMMIT TRANSACTION", 0, 0, &zErrMsg);
	sqlite3_close(db);
	f.close();
	return true;
}

bool ReadWriteTastes(QString file, QString dbfile)
{
	QString filename=dir+file;
	QFile f( filename );
	char *zErrMsg = 0;
	int rc;
	rc = sqlite3_open(dbfile, &db);
	if( rc )
	{
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return false;
	}
	rc = sqlite3_exec(db, "DROP TABLE tastes;", 0, 0, &zErrMsg);
	rc = sqlite3_exec(db, "CREATE TABLE tastes (ID INTEGER PRIMARY KEY, taste TEXT);", 0, 0, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	}
	if ( !f.open( IO_ReadOnly ) )
	{
		qDebug("Could not open list of tastes.");
		return false;
	}
	qDebug("Opened list of tastes. Reading ...");
	QTextStream t( &f );
	rc = sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, &zErrMsg);
	while ( !t.atEnd() )
	{
		QString line( t.readLine() );
		rc = sqlite3_exec(db, "INSERT INTO tastes VALUES(NULL,\""+line+"\");", 0, 0, &zErrMsg);
		if( rc!=SQLITE_OK )
		{
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		}
	}
	rc = sqlite3_exec(db, "COMMIT TRANSACTION", 0, 0, &zErrMsg);
	sqlite3_close(db);
	f.close();
	return true;
}

bool ImportUnits(QString file, QString dbfile)
{
	QString filename=dir+file;
	QFile f( filename );
	char *zErrMsg = 0;
	int rc;
	rc = sqlite3_open(dbfile, &db);
	if( rc )
	{
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return false;
	}
	rc = sqlite3_exec(db, "DROP TABLE units;", 0, 0, &zErrMsg);
	rc = sqlite3_exec(db, "CREATE TABLE units (ID INTEGER PRIMARY KEY, unit TEXT, mlfactor NUMERIC, oldID TEXT);", 0, 0, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	}
	if ( !f.open( IO_ReadOnly ) )
	{
		qDebug("Could not open list of units.");
		return false;
	}
	qDebug("Opened list of units. Reading ...");
	QTextStream t( &f );
	rc = sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, &zErrMsg);
	while ( !t.atEnd() )
	{
		QString line( t.readLine() );
		QString Data, line2;
		for(int k=0;k<4;k++)
		{
			int i = line.find( ";", 0 );
			Data=line.left(i);
			switch (k)
			{
				case 0:	line2=Data;
					break;
				case 1:	line2+=",\""+Data+"\"";
					break;
				case 2:	line2+=","+Data;
					break;
				case 3:	line2+=",\""+Data+"\"";
					break;
			}
			line.remove(0,i+1);
		}
		//qDebug(line2);
		rc = sqlite3_exec(db, "INSERT INTO units VALUES("+line2+");", 0, 0, &zErrMsg);
		if( rc!=SQLITE_OK )
		{
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		}
	}
	rc = sqlite3_exec(db, "COMMIT TRANSACTION", 0, 0, &zErrMsg);
	sqlite3_close(db);
	f.close();
	return true;
}
