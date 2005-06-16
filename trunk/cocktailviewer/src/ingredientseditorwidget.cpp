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
#include <qpixmap.h>
#include <qimage.h>
#include <qlineedit.h>

#include "ingredientseditorwidget.h"
#include "green.xpm"
#include "red.xpm"

ingredientseditorwidget::ingredientseditorwidget(QWidget* parent, const char* name, WFlags fl)
 : ingredientseditorwidgetbase(parent,name,fl)
{
	char *zErrMsg = 0;
	int rc, nrow, ncolumn;
	char **Result;
	QString dbfile;
	green=green_xpm;
	red=red_xpm;
	//green.load( "./green.bmp" );
	//red.load( "./red.bmp" );
	imageRed=QIconSet( red );
	imageGreen=QIconSet( green );
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
		table1->setText(i-1, 0, Result[7*i+1] );
		/*QCheckTableItem *Item;
		Item=new QCheckTableItem( table1, "        " );*/
		if(QString(Result[7*i+5])!="0")
		{
			//Item->setChecked(true);
			table1->verticalHeader()->setLabel( i-1, imageGreen, "" );
		}
		else
		{
			//Item->setChecked(false);
			table1->verticalHeader()->setLabel( i-1, imageRed, "" );
		}
		//table1->setItem(i-1, 0, Item );
		table1->setText(i-1, 1, Result[7*i+2] );
		table1->setText(i-1, 2, Result[7*i+3] );
		table1->setText(i-1, 3, Result[7*i+4] );
		table1->setText(i-1, 4, Result[7*i] );
		table1->verticalHeader()->setResizeEnabled( FALSE, i-1 );
		table1->verticalHeader()->setClickEnabled( TRUE , i-1 );
	}
	table1->setColumnReadOnly ( 4, true );
	//table1->setColumnWidth ( 0, 70);
	table1->setColumnWidth ( 0, 250);
	table1->setColumnWidth ( 1, 80);
	table1->setColumnWidth ( 2, 70);
	table1->setColumnWidth ( 3, 80);
	table1->setColumnWidth ( 4, 40);
	countNew=0;
	pushButton8->setEnabled( false );
	dirty=false;
	connect( table1->verticalHeader(), SIGNAL(clicked ( int )), this, SLOT(tableClicked(int)) );
	sqlite3_free_table(Result);
	oldrow=1;
	oldcol=1;
}

void ingredientseditorwidget::lineEdit2Changed()
{
	int rows=table1->numRows();
	for(int i=0;i<=rows;i++)
	{
		if( table1->text(i,0).contains(lineEdit2->text(), FALSE)==0 )
			table1->setRowHeight( i, 0 );
		else
			table1->setRowHeight( i, 20 );
	}
}

void ingredientseditorwidget::table1SelectionChanged(int row, int col)
{
	bool moveon=false;
	if( table1->text(oldrow,0)=="" )
	{
		table1->editCell( oldrow,0 );
		QMessageBox::information(
		this,
		tr("Empty Name"),
		tr("You did not enter a name."),
		0, 0, 0 );
		table1->editCell( oldrow,0 );
	}
	else
		moveon=true;
	table1->setText( oldrow, 1, QString::number( table1->text( oldrow, 1).toFloat() ) );
	table1->setText( oldrow, 2, QString::number( table1->text( oldrow, 2).toFloat() ) );
	table1->setText( oldrow, 3, QString::number( table1->text( oldrow, 3).toFloat() ) );
	if(moveon)
	{
		oldrow=row;
		oldcol=col;
	}
}

void ingredientseditorwidget::addIngredientClicked()
{
	//int row=table1->numRows();
	int row=0;
	table1->insertRows( row );
	table1->ensureCellVisible( row, 1 );
	/*QCheckTableItem *Item;
	Item=new QCheckTableItem( table1, "        " );
	Item->setChecked(true);
	table1->setItem( row, 0, Item );*/
	table1->verticalHeader()->setLabel( row, imageGreen, "" );
	table1->setText( row, 1, "700" );
	table1->setText( row, 2, "5" );
	table1->setText( row, 3, "37.5" );
	table1->setText( row, 4, getNewIngredientID() );
	table1->clearSelection();
	table1->setFocus();
	table1->setCurrentCell( row, 0 );
}

QString ingredientseditorwidget::getNewIngredientID()
{
	countNew++;
	char *zErrMsg = 0;
	int rc, nrow, ncolumn;
	char **Result;
	QString ID;
	rc = sqlite3_get_table(db2, "SELECT * FROM Ingredients ORDER BY ID", &Result, &nrow, &ncolumn, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	}
	ID=QString::number(QString(Result[7*nrow]).toInt()+countNew);
	sqlite3_free_table(Result);
	return ID;
}

void ingredientseditorwidget::deleteIngredientClicked()
{
	char *zErrMsg = 0;
	int rc, nrow, ncolumn;
	char **Result;
	QString selectedIngredientID;
	selectedIngredientID = table1->text( table1->currentRow(), 4 );
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
	sqlite3_free_table(Result);
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
		QImage image1, image2;
		//QCheckTableItem *Item;
		Ingredient=table1->text( i, 4);
		Ingredient+=",\""+table1->text( i, 0);
		Ingredient+="\","+table1->text( i, 1);
		Ingredient+=","+table1->text( i, 2);
		Ingredient+=","+table1->text( i, 3);
		image1=table1->verticalHeader()->iconSet(i)->pixmap();
		image2=red_xpm;
		if(image1==image2)
			Ingredient+=",0";
		else
			Ingredient+=",1";
		/*Item=static_cast<QCheckTableItem *>(table1->item(i,0));
		if(Item->isChecked())
			Ingredient+=",1";
		else
			Ingredient+=",0";*/
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

void ingredientseditorwidget::tableClicked(int row)
{
	QImage image1, image2;
	image1=table1->verticalHeader()->iconSet(row)->pixmap();
	image2=red_xpm;
	if(image1==image2)
		table1->verticalHeader()->setLabel( row, imageGreen, "" );
	else
		table1->verticalHeader()->setLabel( row, imageRed, "" );
	pushButton8->setEnabled( true );
}

ingredientseditorwidget::~ingredientseditorwidget()
{
	sqlite3_close(db2);
}
