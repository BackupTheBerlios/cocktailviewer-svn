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


#include <qlabel.h>
#include <qlistview.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qdatetime.h>
#include <qstringlist.h>
#include <qregexp.h>
#include <qmessagebox.h>

#include "cocktailviewerwidget.h"
#include "ingredientseditorwidget.h"
#include "cocktaileditorwidget.h"
#include "green.xpm"
#include "red.xpm"

cocktailviewerWidget::cocktailviewerWidget(QWidget* parent, const char* name, WFlags fl)
        : cocktailviewerWidgetBase(parent,name,fl)
{
	ColumnOfID=6;
	initialize();
	LoadData();
}

void cocktailviewerWidget::initialize()
{
	//QString dir=argv[0];
	//int i=dir.findRev('/',-1);
	//dir.remove(i+1, dir.length());
	green=(const char **) green_xpm;
	red=(const char **) red_xpm;
	//green.load( "./green.bmp" );
	//red.load( "./red.bmp" );
	openDB();
	nrowFilterResult1=-1;
	nrowFilterResult2=-1;
	nrowFilterResult3=-1;
	nrowFilterResult4=-1;
}

void cocktailviewerWidget::LoadData()
{
	makeIngredientsSearchList();
	if( newChanges() )
		createTMPCocktailExtras();
	UpdateListView1();
	writeIngredientsIntoComboBoxes();
	writeTastesIntoComboBoxes();
	writeTypesIntoComboBoxes();
	listView1->setFocus();
	listView1->setSelected( listView1->firstChild(), TRUE );
	ListView1Clicked(listView1->firstChild());
}

void cocktailviewerWidget::openDB()
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

void cocktailviewerWidget::makeIngredientsSearchList()
{
	char *zErrMsg = 0;
	int rc;
	rc = sqlite3_get_table(db, "SELECT amount,price,alcohol,stock,ID FROM Ingredients", &globalIngredientsResult, &globalIngredientsnrow, &globalIngredientsncolumn, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	}
	
}

void cocktailviewerWidget::searchIngredientsList(QString ID)
{
	for(int i=1;i<=globalIngredientsnrow;i++)
	{
		if(globalIngredientsResult[5*i+4]==ID)
		{
		globalResult1=globalIngredientsResult[5*i];
		globalResult2=globalIngredientsResult[5*i+1];
		globalResult3=globalIngredientsResult[5*i+2];
		globalResult4=globalIngredientsResult[5*i+3];
		break;
		}
	}
}

void cocktailviewerWidget::createTMPCocktailExtras()
{
	QDateTime d;
	QTime t;
	t.start();
	char *zErrMsg = 0;
	int rc, nrow, ncolumn, nrow2, ncolumn2;
	char **Result, **Result2;
	QString stock, Date;
	float CocktailPrice, AbsolutAlcohol, CocktailAmount, RelativeAlcohol;
	float amountInBottle=0, priceOfBottle=0, alcohol=0;
	fprintf(stderr, "Creating TMPCocktailExtras ");
	rc = sqlite3_get_table(db, "select ID from units where mlfactor=10", &Result, &nrow, &ncolumn, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	}
	QString CLunitID=Result[1];
	rc = sqlite3_exec(db, "DROP TABLE TMPCocktailExtras;", 0, 0, &zErrMsg);
	rc = sqlite3_exec(db, "CREATE TABLE TMPCocktailExtras (ID INTEGER PRIMARY KEY, cocktailID NUMERIC, available NUMERIC, amount NUMERIC, price NUMERIC, absolutAlc NUMERIC, relativeAlc NUMERIC);", 0, 0, &zErrMsg);
	rc = sqlite3_get_table(db, "SELECT CocktailID,amount,unitID,IngredientID FROM CocktailIngredients", &Result2, &nrow2, &ncolumn2, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	}
	sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, &zErrMsg);
	int i=1;
	while(i<=nrow2)
	{
		bool newCocktail=true;
		QString available="1";
		QString CocktailID=Result2[4*i];
		CocktailPrice=0;
		AbsolutAlcohol=0;
		CocktailAmount=0;
		RelativeAlcohol=0;
		while( i<=nrow2 && ( QString(Result2[4*i])==QString(Result2[4*(i-1)]) || newCocktail ) )
		{
			float amountInCocktail=QString(Result2[4*i+1]).toFloat();
			QString unitID=Result2[4*i+2];
			float mlfactor;
			if(unitID==CLunitID)
				mlfactor=10;
			else
				mlfactor=getitFromID(unitID, "units", "mlfactor").toFloat();
			QString IngredientID=Result2[4*i+3];
			newCocktail=false;
			searchIngredientsList(IngredientID);
			amountInBottle=globalResult1.toFloat();
			priceOfBottle=globalResult2.toFloat();
			alcohol=globalResult3.toFloat();
			stock=globalResult4;
			if(amountInBottle!=0)
				CocktailPrice+=amountInCocktail*mlfactor*(priceOfBottle/amountInBottle);
			AbsolutAlcohol+=amountInCocktail*mlfactor*(alcohol/100);
			CocktailAmount+=amountInCocktail*mlfactor;
			if(stock=="0")
				available="0";
			i++;
		}
		RelativeAlcohol=AbsolutAlcohol/CocktailAmount;
		rc = sqlite3_exec(db, "INSERT INTO TMPCocktailExtras VALUES(NULL,"+CocktailID+","+available+","+QString::number(CocktailAmount)+","+QString::number(CocktailPrice)+","+QString::number(AbsolutAlcohol)+","+QString::number(RelativeAlcohol)+")", 0, 0, &zErrMsg);
		if( rc!=SQLITE_OK )
		{
			qDebug("1");
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		}
		fprintf(stderr, ".");
	}
	sqlite3_free_table(Result2);
	fprintf(stderr, "\n");
	rc = sqlite3_exec(db, "DELETE FROM timestamps WHERE operation=\"TMPCocktailExtras_created\";", 0, 0, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	}
	Date=d.currentDateTime().toString("yyyyMMddhhmmss");
	rc = sqlite3_exec(db, "INSERT INTO timestamps VALUES( NULL,\"TMPCocktailExtras_created\","+Date+");", 0, 0, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	}
	rc = sqlite3_exec(db, "COMMIT TRANSACTION", 0, 0, &zErrMsg);
	qDebug(QString::number(t.elapsed())+" ms");
}

void cocktailviewerWidget::UpdateListView1()
{
	char *zErrMsg = 0;
	int rc, nrow, ncolumn, nrow2, ncolumn2;
	char **Result, **Result2;
	int counter=0;
	QTime t;
	t.start();
	QString Box1=comboBox1->currentText();
	QString Box2=comboBox2->currentText();
	QString Box3=comboBox3->currentText();
	QString Box4=comboBox4->currentText();
	QString Box5=comboBox5->currentText();
	QString Box6=comboBox6->currentText();
	bool checkBox1Checked=checkBox1->isChecked();
	
	fprintf(stderr, "Updating ListView ");
	rc = sqlite3_get_table(db, "SELECT ID,Name,rating,taste1ID,taste2ID,typeID FROM Cocktails WHERE Name LIKE \"%"+lineEdit1->text()+"%\"", &Result, &nrow, &ncolumn, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		qDebug("0");
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	}
	listView1->clear();
	for(int i=1;i<=nrow;i++)
	{
		QString ID=Result[ncolumn*i];
		QString name=Result[ncolumn*i+1];
		QString rating=Result[ncolumn*i+2];
		QString taste1ID=Result[ncolumn*i+3];
		QString taste2ID=Result[ncolumn*i+4];
		QString typeID=Result[ncolumn*i+5];
		rc = sqlite3_get_table(db, "SELECT * FROM TMPCocktailExtras WHERE ID="+ID, &Result2, &nrow2, &ncolumn2, &zErrMsg);
		if( rc!=SQLITE_OK )
		{
			qDebug("1");
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		}
		QString available=Result2[9];
		QString amount=Result2[10];
		QString price=QString::number(QString(Result2[11]).toFloat(),'f', 2);
		QString absolutAlcohol=QString::number(QString(Result2[12]).toFloat(),'f', 2);
		QString relativeAlcohol=QString::number(QString(Result2[13]).toFloat()*100,'f', 1);
		if(!checkBox1Checked || available=="1")
		{
			if( (Box1=="" || checkFilterlist( ID, 1 , nrowFilterResult1)) && (Box2=="" || checkFilterlist( ID, 2 , nrowFilterResult2)) && (Box3=="" || checkFilterlist( ID, 3, nrowFilterResult3)) && (Box4=="" || checkFilterlist( ID, 4, nrowFilterResult4 )) && (Box5=="" || (taste1ID!="-1" && taste1ID==getID("tastes", "taste", Box5)) || (taste2ID!="-1" && taste2ID==getID("tastes", "taste", Box5))) && (Box6=="" || (typeID!="-1" && typeID==getID("types", "type", Box6))) )
			{
				QListViewItem *item = new MyListViewItem( listView1 );
				item->setText( 0,  name);
				item->setText( 1,  rating);
				item->setText( 2,  price);
				item->setText( 3,  amount);
				item->setText( 4,  relativeAlcohol);
				item->setText( 5,  absolutAlcohol);
				item->setText( 6,  ID);
				if( available=="1")
					item->setPixmap( 0, green );
				else
					item->setPixmap( 0, red );
				counter++;
				fprintf(stderr, ".");
			}
		}
		sqlite3_free_table(Result2);
		textLabel1_5->setText(QString::number(counter)+" Cocktails");
	}
	fprintf(stderr, "\n");
	sqlite3_free_table(Result);
	qDebug(QString::number(t.elapsed())+" ms");
}

void cocktailviewerWidget::ListView1Clicked(QListViewItem *Item)
{
	if(Item)
	{
		QString ID=Item->text(ColumnOfID);
		QString Name=Item->text(0);
		loadCocktail( ID, Name);
	}
}

void cocktailviewerWidget::loadCocktail(QString ID, QString Name)
{
	QString description, rating, type, taste1, taste2;
	char *zErrMsg = 0;
	int rc, nrow, ncolumn, nrow2, ncolumn2;
	char **Result, **Result2;
	QString Text="", Text2="";
	rc = sqlite3_get_table(db, "SELECT * FROM Cocktails WHERE ID="+ID, &Result, &nrow, &ncolumn, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		qDebug("2");
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	}
	rating=Result[10];
	description=Result[9];
	type=getitFromID(Result[11], "types", "type");
	taste1=getitFromID(Result[12], "tastes", "taste");
	taste2=getitFromID(Result[13], "tastes", "taste");
	sqlite3_free_table(Result);
	rc = sqlite3_get_table(db, "SELECT * FROM CocktailIngredients WHERE cocktailID="+ID, &Result, &nrow, &ncolumn, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		qDebug("3");
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	}
	for(int i=1;i<=nrow;i++)
	{
		QString amount, unit,ingredient;
		amount=Result[5*i+2];
		unit=getitFromID(Result[5*i+3], "units", "unit");
		ingredient=getitFromID(Result[5*i+4], "ingredients", "name");
		Text2+=amount+" "+unit+" "+ingredient+"<br>";
	}
	sqlite3_free_table(Result);
	rc = sqlite3_get_table(db, "SELECT * FROM TMPCocktailExtras WHERE cocktailID="+ID, &Result2, &nrow2, &ncolumn2, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		qDebug("4");
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	}
	if(description!="")
		Text2+="<br><font size=\"-1\">"+description+"</font>";
	textLabel1_3->setText(Text2);
	Text="<font size=\"+1\">"+Name+"</font>";
	QString Stars=printStars(rating.toInt());
	if(Stars!="")
		Text+="<br>"+Stars;
	if(type!="" || taste1!="" || taste2!="")
		Text+="<br><font size=\"-4\">"+type+" "+taste1+" "+taste2+"</font>";
	textLabel1_4->setText(Text);
	textLabel1_6->setText("<b>"+QString::number(QString(Result2[11]).toFloat(),'f', 2)+" EUR</b>");
	sqlite3_free_table(Result2);
}

QString cocktailviewerWidget::getitFromID(QString ID, QString table, QString value)
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

QString cocktailviewerWidget::getID(QString table, QString value, QString string)
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

QString cocktailviewerWidget::printStars(int rating)
{
	QString stars;
	if(rating<1 || rating>5)
		stars="";
	else
		stars="*"+printStars(rating-1);
	return stars;
}

int cocktailviewerWidget::createFilteredCocktaillist(QComboBox *box, int FilterNo )
{
	switch(FilterNo)
	{
		case 1: FilterList1.clear();
			break;
		case 2: FilterList2.clear();
			break;
		case 3: FilterList3.clear();
			break;
		case 4: FilterList4.clear();
			break;
	}
	int nrow;
	QString ID=getID("Ingredients", "name", box->currentText());
	if( ID!="-1" )
	{
		char *zErrMsg = 0;
		int rc, ncolumn;
		char **result;
		rc = sqlite3_get_table(db, "SELECT CocktailID FROM CocktailIngredients WHERE IngredientID="+ID, &result, &nrow, &ncolumn, &zErrMsg);
		if( rc!=SQLITE_OK )
		{
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		}
		for(int i=1;i<=nrow;i++)
		{
			switch(FilterNo)
			{
				case 1: FilterList1.append(result[i]);
					break;
				case 2: FilterList2.append(result[i]);
					break;
				case 3: FilterList3.append(result[i]);
					break;
				case 4: FilterList4.append(result[i]);
					break;
			}
		}
		sqlite3_free_table(result);
	}
	else
		nrow=-1;
	return nrow;
}

void cocktailviewerWidget::ComboBox1Changed()
{
	nrowFilterResult1 = createFilteredCocktaillist( comboBox1, 1 );
	UpdateListView1();
}

void cocktailviewerWidget::ComboBox2Changed()
{
	nrowFilterResult2 = createFilteredCocktaillist( comboBox2, 2 );
	UpdateListView1();
}

void cocktailviewerWidget::ComboBox3Changed()
{
	nrowFilterResult3 = createFilteredCocktaillist( comboBox3, 3 );
	UpdateListView1();
}

void cocktailviewerWidget::ComboBox4Changed()
{
	nrowFilterResult4 = createFilteredCocktaillist( comboBox4, 4 );
	UpdateListView1();
}

bool cocktailviewerWidget::checkFilterlist(QString ID, int Filter, int nrowFilterResult )
{
	if(nrowFilterResult==-1)
		return false;
	QStringList result;
	switch(Filter)
	{
		case 1: result=FilterList1.grep(QRegExp("^"+ID+"$"));
			break;
		case 2: result=FilterList2.grep(QRegExp("^"+ID+"$"));
			break;
		case 3: result=FilterList3.grep(QRegExp("^"+ID+"$"));
			break;
		case 4: result=FilterList4.grep(QRegExp("^"+ID+"$"));
			break;
	}
	if(result.empty())
		return false;
	else
		return true;
}

void cocktailviewerWidget::writeIngredientsIntoComboBoxes()
{
	comboBox1->disconnect(this);
	comboBox2->disconnect(this);
	comboBox3->disconnect(this);
	comboBox4->disconnect(this);
	comboBox1->clear();
	comboBox2->clear();
	comboBox3->clear();
	comboBox4->clear();
	char *zErrMsg = 0;
	int rc, ncolumn, nrow;
	char **result;
	rc = sqlite3_get_table(db, "SELECT name,stock FROM Ingredients ORDER BY name", &result, &nrow, &ncolumn, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	}
	comboBox1->insertItem("");
	comboBox2->insertItem("");
	comboBox3->insertItem("");
	comboBox4->insertItem("");
	for(int i=1;i<=nrow;i++)
	{
		if(QString(result[2*i+1])!="0")
		{
			comboBox1->insertItem( green, result[2*i] );
			comboBox2->insertItem( green, result[2*i] );
			comboBox3->insertItem( green, result[2*i] );
			comboBox4->insertItem( green, result[2*i] );
		}
		else
		{
			if( !checkBox2->isChecked() )
			{
				comboBox1->insertItem( red, result[2*i] );
				comboBox2->insertItem( red, result[2*i] );
				comboBox3->insertItem( red, result[2*i] );
				comboBox4->insertItem( red, result[2*i] );
			}
		}

	}
	sqlite3_free_table(result);
	connect( comboBox1, SIGNAL(textChanged(const QString&)), this, SLOT(ComboBox1Changed()) );
	connect( comboBox2, SIGNAL(textChanged(const QString&)), this, SLOT(ComboBox2Changed()) );
	connect( comboBox3, SIGNAL(textChanged(const QString&)), this, SLOT(ComboBox3Changed()) );
	connect( comboBox4, SIGNAL(textChanged(const QString&)), this, SLOT(ComboBox4Changed()) );
}

void cocktailviewerWidget::writeTastesIntoComboBoxes()
{
	comboBox5->disconnect(this);
	comboBox5->clear();
	char *zErrMsg = 0;
	int rc, ncolumn, nrow;
	char **result;
	rc = sqlite3_get_table(db, "SELECT taste FROM tastes ORDER BY taste", &result, &nrow, &ncolumn, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	}
	comboBox5->insertItem("");
	for(int i=1;i<=nrow;i++)
	{
		comboBox5->insertItem( result[i] );
	}
	sqlite3_free_table(result);
	connect( comboBox5, SIGNAL(textChanged(const QString&)), this, SLOT(UpdateListView1()) );
}

void cocktailviewerWidget::writeTypesIntoComboBoxes()
{
	comboBox6->disconnect(this);
	comboBox6->clear();
	char *zErrMsg = 0;
	int rc, ncolumn, nrow;
	char **result;
	rc = sqlite3_get_table(db, "SELECT type FROM types ORDER BY type", &result, &nrow, &ncolumn, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	}
	comboBox6->insertItem("");
	for(int i=1;i<=nrow;i++)
	{
		comboBox6->insertItem( result[i] );
	}
	sqlite3_free_table(result);
	connect( comboBox6, SIGNAL(textChanged(const QString&)), this, SLOT(UpdateListView1()) );
}

void cocktailviewerWidget::checkBox1Clicked()
{
	UpdateListView1();
}

void cocktailviewerWidget::pushButton2Clicked()
{
	comboBox1->setCurrentItem( 0 );
}

void cocktailviewerWidget::pushButton3Clicked()
{
	comboBox2->setCurrentItem( 0 );
}

void cocktailviewerWidget::pushButton4Clicked()
{
	comboBox3->setCurrentItem( 0 );
}

void cocktailviewerWidget::pushButton5Clicked()
{
	comboBox4->setCurrentItem( 0 );
}

void cocktailviewerWidget::editIngredientsClicked()
{
	bool dirty;
	sqlite3_close(db);
	ingredientseditorwidget *IngredientEditor;
	IngredientEditor=new ingredientseditorwidget(this, "IngredientEditor");
	IngredientEditor->setCaption( "IngredientEditor" );
	IngredientEditor->exec();
	dirty=IngredientEditor->isDirty();
	delete IngredientEditor;
	openDB();
	if( dirty )
		LoadData();
}

void cocktailviewerWidget::aboutQt()
{
	QMessageBox::aboutQt( this, tr("About Qt") );
}

void cocktailviewerWidget::about()
{
	extern QString version;
	QMessageBox::about( this, tr("About "+ version),
				tr("This program is free software; you can redistribute it and/or"
				"\nmodify it under the terms of the GNU General Public License."
				"\nSee COPYING for more details."
				"\n\nThis programm uses some icons of the Nuvola icon theme."
				"\nSee http://www.icon-king.com."
				"\n\nBenni"));
}

bool cocktailviewerWidget::newChanges()
{
	char *zErrMsg = 0;
	int rc, nrow, ncolumn, nrow2, ncolumn2;
	char **Result, **Result2;
	rc = sqlite3_get_table(db, "SELECT date FROM timestamps Where operation=\"LastChanges\"", &Result, &nrow, &ncolumn, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	}
	rc = sqlite3_get_table(db, "SELECT date FROM timestamps Where operation=\"TMPCocktailExtras_created\"", &Result2, &nrow2, &ncolumn2, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	}
	if(nrow<1 || nrow2<1)
		return true;
	if(QString(Result[1]).toDouble()<=QString(Result2[1]).toDouble())
		return false;
	else
		return true;
}

void cocktailviewerWidget::deleteCocktailClicked()
{
	QListViewItem *Item=listView1->currentItem();
	if(Item)
	{
		QString ID=Item->text(ColumnOfID);
		QString Name=Item->text(0);
		deleteCocktail( ID, Name );
	}

}

void cocktailviewerWidget::deleteCocktail( QString ID, QString Name )
{
	if( QMessageBox::question(
		this,
		tr("Delete Cocktail?"),
		tr("<qt><p align=\"center\">Do you really want to delete the following Cocktail?<br><br><b>"+Name+"</b></p></qt>"),
		tr("&Yes"), tr("&No"),
		QString::null, 0, 1 )==0 )
	{
		char *zErrMsg = 0;
		int rc;
		rc = sqlite3_exec(db, "DELETE FROM Cocktails WHERE ID=\""+ID+"\";", 0, 0, &zErrMsg);
		if( rc!=SQLITE_OK )
		{
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		}
		rc = sqlite3_exec(db, "DELETE FROM CocktailIngredients WHERE CocktailID=\""+ID+"\";", 0, 0, &zErrMsg);
		if( rc!=SQLITE_OK )
		{
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		}
		rc = sqlite3_exec(db, "DELETE FROM TMPCocktailExtras WHERE CocktailID=\""+ID+"\";", 0, 0, &zErrMsg);
		if( rc!=SQLITE_OK )
		{
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		}
		UpdateListView1();
	}
}

void cocktailviewerWidget::addCocktailClicked()
{
	bool dirty;
	sqlite3_close(db);
	cocktaileditorwidget *CocktailEditor;
	CocktailEditor=new cocktaileditorwidget(this, "Cocktail Editor", "");
	CocktailEditor->setCaption( "Cocktail Editor" );
	CocktailEditor->exec();
	//dirty=CocktailEditor->isDirty();
	delete CocktailEditor;
	openDB();
	/*if( dirty )
		LoadData();*/
}

void cocktailviewerWidget::editCocktailClicked()
{
	bool dirty;
	QListViewItem *Item=listView1->currentItem();
	if(Item)
	{
		QString ID=Item->text(ColumnOfID);
		sqlite3_close(db);
		cocktaileditorwidget *CocktailEditor;
		CocktailEditor=new cocktaileditorwidget(this, "Cocktail Editor", ID);
		CocktailEditor->setCaption( "Cocktail Editor" );
		CocktailEditor->exec();
		delete CocktailEditor;
		openDB();
	}
}

cocktailviewerWidget::~cocktailviewerWidget()
{
	sqlite3_close(db);
}

MyListViewItem::MyListViewItem( QListView* parent ) : QListViewItem(parent)
{
}

int MyListViewItem::compare ( QListViewItem * i, int col, bool ascending ) const
{
	float val1, val2;
	if(col>0)
	{
		val1=text(col).toFloat();
		val2=i->text(col).toFloat();
		if(val1==val2)
			return 0;
		else if(val1<val2)
			return 1;
		else return -1;
	}
        return QListViewItem::compare(i, col, ascending);
}

/*void MyListViewItem::paintCell( QPainter *p, const QColorGroup &cg, int column, int width, int alignment )
{
	QColorGroup _cg( cg );
	QColor c = _cg.text();
	if( available=="0" )
		_cg.setColor( QColorGroup::Text, Qt::red.dark(100) );
	else
		_cg.setColor( QColorGroup::Text, Qt::green.dark(200) );
	QListViewItem::paintCell( p, _cg, column, width, alignment );
	_cg.setColor( QColorGroup::Text, c );
}*/

//#include "cocktailviewerwidget.moc"
