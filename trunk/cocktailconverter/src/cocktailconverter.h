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
#include <list>
#include <qstring.h>

#define _AsciiEmp     "?§"
#define _AsciiSepCock "%§"
#define _AsciiSepZut  "$§"
#define _AsciiSepZutB "(§"



using namespace std;

struct CocktailZutatNode
{
float menge;
QString einheit;
QString kennung;
QString ExternerName;
float ExternPreis;
bool ExternMachbar;
};

typedef list<CocktailZutatNode> LCocktailZutaten;


struct CocktailNode
{
QString Name;
LCocktailZutaten CocktailZutaten;
QString Beschreibung;
QString Cocktailart;
QString Geschmack1;
QString Geschmack2;
float Benotung;
//diese Variblen werden berechnet oder aus andere Listen entnommen:
float ExternPreis;
bool ExternMachbar;
float ExternGehalt;
float ExternGehaltML;
float ExternMengeML;
bool ExternShow;
};


typedef list<CocktailNode> LCocktails;

typedef void (*CallbackFunktCount) (int count);
//-------------------------------------------------------------------
struct ZutatNode
{
//void Assign(ZutatNode src);
QString kennung;
QString name;
float menge;
float preis;
float gehalt;
float bestand;
//Extern;
bool ExternMachbar;
};
typedef list<ZutatNode> LZutaten;

QString dir;
sqlite3 *db;
int counter;

LCocktails Cocktails;
LZutaten   Zutaten;

bool ReadIngredientslist(QString file);
bool WriteIngredientslistToSQL(QString dbile);
bool ReadCocktaillist(QString file);
bool WriteCocktailsToSQL(QString dbfile);
QString getID(QString table, QString value, QString string);
bool ReadWriteCocktailTypes(QString file, QString dbfile);
bool ReadWriteTastes(QString file, QString dbfile);
bool ImportUnits(QString file, QString dbfile);
bool timeStamp(QString dbfile);
