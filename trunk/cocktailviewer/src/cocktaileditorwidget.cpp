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
#include <sqlite3.h>
#include <qlabel.h>
#include <qcombobox.h>

#include "cocktaileditorwidget.h"

cocktaileditorwidget::cocktaileditorwidget( QWidget* parent, const char* name, WFlags fl )
 : cocktaileditorwidgetbase( parent,name,fl )
{
	int rc;
	QString dbfile;
	extern QString dir;
	textLabel2_2->setText("");
	//green=green_xpm;
	//red=red_xpm;
	//imageRed=QIconSet( red );
	//imageGreen=QIconSet( green );
	dbfile = dir + "cocktail.db";
	rc = sqlite3_open(dbfile, &db3);
	if( rc )
	{
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db3));
		sqlite3_close(db3);
		return;
	}
	init();
}

void cocktaileditorwidget::init()
{
	writeTastes();
	writeTypes();
	writeUnits();
	writeIngredients();
}

void cocktaileditorwidget::writeTastes()
{
	char *zErrMsg = 0;
	int rc, nrow, ncolumn;
	char **Result;
	comboBox7->clear();
	comboBox8->clear();
	comboBox7->insertItem("");
	comboBox8->insertItem("");
	rc = sqlite3_get_table(db3, "select taste from tastes", &Result, &nrow, &ncolumn, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	}
	for(int i=1;i<=nrow;i++)
	{
		comboBox7->insertItem(Result[i]);
		comboBox8->insertItem(Result[i]);
	}
}

void cocktaileditorwidget::writeTypes()
{
	char *zErrMsg = 0;
	int rc, nrow, ncolumn;
	char **Result;
	comboBox9->clear();
	comboBox9->insertItem("");
	rc = sqlite3_get_table(db3, "select type from types", &Result, &nrow, &ncolumn, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	}
	for(int i=1;i<=nrow;i++)
	{
		comboBox9->insertItem(Result[i]);
	}
}

void cocktaileditorwidget::writeUnits()
{
	char *zErrMsg = 0;
	int rc, nrow, ncolumn;
	char **Result;
	comboBox5->clear();
	comboBox5_2->clear();
	comboBox5_3->clear();
	comboBox5_4->clear();
	comboBox5_5->clear();
	comboBox5_6->clear();
	comboBox5_7->clear();
	rc = sqlite3_get_table(db3, "select unit from units", &Result, &nrow, &ncolumn, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	}
	for(int i=1;i<=nrow;i++)
	{
		comboBox5->insertItem(Result[i]);
		comboBox5_2->insertItem(Result[i]);
		comboBox5_3->insertItem(Result[i]);
		comboBox5_4->insertItem(Result[i]);
		comboBox5_5->insertItem(Result[i]);
		comboBox5_6->insertItem(Result[i]);
		comboBox5_7->insertItem(Result[i]);
	}
}

void cocktaileditorwidget::writeIngredients()
{
	char *zErrMsg = 0;
	int rc, nrow, ncolumn;
	char **Result;
	comboBox4->clear();
	comboBox4_2->clear();
	comboBox4_3->clear();
	comboBox4_4->clear();
	comboBox4_5->clear();
	comboBox4_6->clear();
	comboBox4_7->clear();
	comboBox4->insertItem("");
	comboBox4_2->insertItem("");
	comboBox4_3->insertItem("");
	comboBox4_4->insertItem("");
	comboBox4_5->insertItem("");
	comboBox4_6->insertItem("");
	comboBox4_7->insertItem("");
	rc = sqlite3_get_table(db3, "select name,stock from ingredients", &Result, &nrow, &ncolumn, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	}
	for(int i=1;i<=nrow;i++)
	{
		if(QString(Result[ncolumn*i+1])!="0")
		{
			comboBox4->insertItem(Result[ncolumn*i]);
			comboBox4_2->insertItem(Result[ncolumn*i]);
			comboBox4_3->insertItem(Result[ncolumn*i]);
			comboBox4_4->insertItem(Result[ncolumn*i]);
			comboBox4_5->insertItem(Result[ncolumn*i]);
			comboBox4_6->insertItem(Result[ncolumn*i]);
			comboBox4_7->insertItem(Result[ncolumn*i]);
		}
	}
}

cocktaileditorwidget::~cocktaileditorwidget()
{
}


