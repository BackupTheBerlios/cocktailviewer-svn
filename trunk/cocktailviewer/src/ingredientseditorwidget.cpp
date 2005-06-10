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

#include <sqlite3.h>
#include <qtable.h>
#include <qstring.h>
#include <qpushbutton.h>
#include <qmessagebox.h>

#include "ingredientseditorwidget.h"

ingredientseditorwidget::ingredientseditorwidget(QWidget* parent, const char* name, WFlags fl)
 : ingredientseditorwidgetbase(parent,name,fl)
{
	char *zErrMsg = 0;
	int rc, nrow, ncolumn;
	char **Result;
	QString dbfile;
	dbfile="cocktail.db";
	rc = sqlite3_open(dbfile, &db2);
	if( rc )
	{
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db2));
		sqlite3_close(db2);
		return;
	}
	rc = sqlite3_get_table(db2, "SELECT * FROM Ingredients ORDER BY Name", &Result, &nrow, &ncolumn, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	}
	table1->setNumRows(nrow);
	for(int i=1;i<=nrow;i++)
	{
		table1->setText(i-1, 1, Result[7*i+1] );
		QCheckTableItem *Item;
		Item=new QCheckTableItem( table1, "        " );
		if(QString(Result[7*i+5])!="0")
			Item->setChecked(true);
		else
			Item->setChecked(false);
		table1->setItem(i-1, 0, Item );
		table1->setText(i-1, 2, Result[7*i+2] );
		table1->setText(i-1, 3, Result[7*i+3] );
		table1->setText(i-1, 4, Result[7*i+4] );
		table1->setText(i-1, 5, Result[7*i] );
		table1->setRowStretchable ( i-1, true );
	}
	table1->setColumnReadOnly ( 5, true );
	table1->setColumnWidth ( 0, 50);
	table1->setColumnWidth ( 1, 250);
	table1->setColumnWidth ( 2, 80);
	table1->setColumnWidth ( 3, 70);
	table1->setColumnWidth ( 4, 80);
	table1->setColumnWidth ( 5, 40);
	countNew=0;
	pushButton8->setEnabled( false );
	dirty=false;
}

void ingredientseditorwidget::addIngredientClicked()
{
	int row=table1->numRows();
	table1->insertRows( row );
	table1->ensureCellVisible( row, 1 );
	QCheckTableItem *Item;
	Item=new QCheckTableItem( table1, "        " );
	Item->setChecked(true);
	table1->setItem( row, 0, Item );
	table1->setText( row, 2, "700" );
	table1->setText( row, 3, "5" );
	table1->setText( row, 4, "37.5" );
	table1->setText( row, 5, getNewIngredientID() );
	table1->clearSelection();
	table1->setCurrentCell( row, 1 );
}

QString ingredientseditorwidget::getNewIngredientID()
{
	countNew++;
	char *zErrMsg = 0;
	int rc, nrow, ncolumn;
	char **Result;
	rc = sqlite3_get_table(db2, "SELECT * FROM Ingredients ORDER BY ID", &Result, &nrow, &ncolumn, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	}
	return QString::number(QString(Result[7*nrow]).toInt()+countNew);
}

void ingredientseditorwidget::deleteIngredientClicked()
{
	char *zErrMsg = 0;
	int rc, nrow, ncolumn;
	char **Result;
	QString selectedIngredientID;
	selectedIngredientID = table1->text( table1->currentRow(), 5 );
	rc = sqlite3_get_table(db2, "SELECT ID FROM CocktailIngredients WHERE IngredientID="+selectedIngredientID, &Result, &nrow, &ncolumn, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	}
	if(nrow>=1)
		QMessageBox::information(
			this,
			tr("Delete Ingredient"),
			tr("You cannot delete this ingredient, because it is still in use!"),
			0, 0, 0 );
	else
		if( QMessageBox::question(
			this,
			tr("Delete Ingredient"),
			tr("Do you really want to delete this ingredient?"),
			tr("&Yes"), tr("&No"),
			QString::null, 0, 1 )==0 )
		{
			pushButton8->setEnabled( true );
			table1->removeRow(table1->currentRow());
		}
}

void ingredientseditorwidget::saveIngredientsToDB()
{
	int rows=table1->numRows();
	QString Ingredient;
	char *zErrMsg = 0;
	int rc;
	rc = sqlite3_exec(db2, "DROP TABLE Ingredients", 0, 0, &zErrMsg);
	rc = sqlite3_exec(db2, "CREATE TABLE Ingredients (ID INTEGER PRIMARY KEY, name TEXT, amount NUMERIC, price NUMERIC, alcohol NUMERIC, stock NUMERIC, oldID TEXT);", 0, 0, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	}
	rc = sqlite3_exec(db2, "BEGIN TRANSACTION", 0, 0, &zErrMsg);
	for(int i=0;i<rows;i++)
	{
		QCheckTableItem *Item;
		Ingredient=table1->text( i, 5);
		Ingredient+=",\""+table1->text( i, 1);
		Ingredient+="\","+table1->text( i, 2);
		Ingredient+=","+table1->text( i, 3);
		Ingredient+=","+table1->text( i, 4);
		Item=static_cast<QCheckTableItem *>(table1->item(i,0));
		if(Item->isChecked())
			Ingredient+=",1";
		else
			Ingredient+=",0";
		rc = sqlite3_exec(db2, "INSERT INTO Ingredients VALUES("+Ingredient+",\"\" )", 0, 0, &zErrMsg);
		if( rc!=SQLITE_OK )
		{
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		}
	}
	rc = sqlite3_exec(db2, "COMMIT TRANSACTION", 0, 0, &zErrMsg);
}

void ingredientseditorwidget::OkClicked()
{
	table1->setCurrentCell( 0, 5 );
	saveIngredientsToDB();
	dirty=true;
	close();
}

void ingredientseditorwidget::tableChanged()
{
	pushButton8->setEnabled( true );
}

bool ingredientseditorwidget::isDirty()
{
	return dirty;
}

ingredientseditorwidget::~ingredientseditorwidget()
{
	sqlite3_close(db2);
}
