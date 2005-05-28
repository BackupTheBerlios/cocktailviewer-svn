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


#include <qlabel.h>
#include <qlistview.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qdatetime.h>
#include <qstringlist.h>
#include <qregexp.h>

#include "cocktailviewerwidget.h"

cocktailviewerWidget::cocktailviewerWidget(QWidget* parent, const char* name, WFlags fl)
        : cocktailviewerWidgetBase(parent,name,fl)
{
	//QString dir=argv[0];
	//int i=dir.findRev('/',-1);
	//dir.remove(i+1, dir.length());
	QString dbfile;
	dbfile="cocktail.db";
	int rc;
	char *zErrMsg = 0;
	rc = sqlite3_open(dbfile, &db);
	if( rc )
	{
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return;
	}
	sqlite3_exec(db, "DROP TABLE TMPFilter1;", 0, 0, &zErrMsg);
	sqlite3_exec(db, "DROP TABLE TMPFilter2;", 0, 0, &zErrMsg);
	sqlite3_exec(db, "DROP TABLE TMPFilter3;", 0, 0, &zErrMsg);
	sqlite3_exec(db, "DROP TABLE TMPFilter4;", 0, 0, &zErrMsg);
	nrowFilterResult1=-1;
	nrowFilterResult2=-1;
	nrowFilterResult3=-1;
	nrowFilterResult4=-1;
	createTMPCocktailExtras();
	UpdateListView1();
	writeIngredientsIntoComboBoxes();
}

void cocktailviewerWidget::createTMPCocktailExtras()
{
	char *zErrMsg = 0;
	int rc, nrow, ncolumn, nrow2, ncolumn2, nrow3, ncolumn3;
	char **Result, **Result2, **Result3;
	QString stock;
	float CocktailPrice, AbsolutAlcohol, CocktailAmount, RelativeAlcohol;
	float amountInBottle=0, priceOfBottle=0, alcohol=0;
	rc = sqlite3_exec(db, "DROP TABLE TMPCocktailExtras;", 0, 0, &zErrMsg);
	rc = sqlite3_exec(db, "CREATE TABLE TMPCocktailExtras (ID INTEGER PRIMARY KEY, cocktailID NUMERIC, available NUMERIC, amount NUMERIC, price NUMERIC, absolutAlc NUMERIC, relativeAlc NUMERIC);", 0, 0, &zErrMsg);
	rc = sqlite3_get_table(db, "SELECT ID FROM Cocktails", &Result, &nrow, &ncolumn, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	}
	sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, &zErrMsg);
	for(int i=1;i<=nrow;i++)
	{
		CocktailPrice=0;
		AbsolutAlcohol=0;
		CocktailAmount=0;
		RelativeAlcohol=0;
		QString CocktailID=Result[i];
		rc = sqlite3_get_table(db, "SELECT amount,unitID,IngredientID FROM CocktailIngredients WHERE CocktailID="+CocktailID, &Result2, &nrow2, &ncolumn2, &zErrMsg);
		if( rc!=SQLITE_OK )
		{
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		}
		QString available="1";
		for(int k=1;k<=nrow2;k++)
		{
			float amountInCocktail=QString(Result2[k*3]).toFloat();
			QString unitID=Result2[k*3+1];
			float mlfactor=getitFromID(unitID, "units", "mlfactor").toFloat();
			QString IngredientID=Result2[k*3+2];
			rc = sqlite3_get_table(db, "SELECT amount,price,alcohol,stock FROM Ingredients WHERE ID="+IngredientID, &Result3, &nrow3, &ncolumn3, &zErrMsg);
			if( rc!=SQLITE_OK )
			{
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
			}
			if(nrow3>0)
			{
				amountInBottle=QString(Result3[4]).toFloat();
				priceOfBottle=QString(Result3[5]).toFloat();
				alcohol=QString(Result3[6]).toFloat();
				stock=Result3[7];
			}
			if(amountInBottle!=0)
				CocktailPrice+=amountInCocktail*mlfactor*(priceOfBottle/amountInBottle);
			AbsolutAlcohol+=amountInCocktail*mlfactor*(alcohol/100);
			CocktailAmount+=amountInCocktail*mlfactor;
			if(stock=="0")
				available="0";
		}
		RelativeAlcohol=AbsolutAlcohol/CocktailAmount;
		rc = sqlite3_exec(db, "INSERT INTO TMPCocktailExtras VALUES(NULL,"+CocktailID+","+available+","+QString::number(CocktailAmount)+","+QString::number(CocktailPrice)+","+QString::number(AbsolutAlcohol)+","+QString::number(RelativeAlcohol)+")", 0, 0, &zErrMsg);
		if( rc!=SQLITE_OK )
		{
			qDebug("1");
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		}
	}
	rc = sqlite3_exec(db, "COMMIT TRANSACTION", 0, 0, &zErrMsg);
}

void cocktailviewerWidget::UpdateListView1()
{
	char *zErrMsg = 0;
	int rc, nrow, ncolumn, nrow2, ncolumn2;
	char **Result, **Result2;
	int counter=0;
	QString Box1=comboBox1->currentText();
	QString Box2=comboBox2->currentText();
	QString Box3=comboBox3->currentText();
	QString Box4=comboBox4->currentText();
	rc = sqlite3_get_table(db, "SELECT ID,Name,rating FROM Cocktails WHERE Name LIKE \"%"+lineEdit1->text()+"%\"", &Result, &nrow, &ncolumn, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		qDebug("0");
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	}
	listView1->clear();
	for(int i=1;i<=nrow;i++)
	{
		QString ID=Result[3*i];
		QString name=Result[3*i+1];
		QString rating=Result[3*i+2];
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
		if(!checkBox1->isChecked() || available=="0")
		{
			if((Box1=="" || checkFilterlist( ID, 1 , nrowFilterResult1)) && (Box2=="" || checkFilterlist( ID, 2 , nrowFilterResult2)) && (Box3=="" || checkFilterlist( ID, 3, nrowFilterResult3)) && (Box4=="" || checkFilterlist( ID, 4, nrowFilterResult4 )))
			{
				QListViewItem *item = new MyListViewItem( listView1 );
				item->setText( 0,  name);
				item->setText( 1,  rating);
				item->setText( 2,  price);
				item->setText( 3,  amount);
				item->setText( 4,  relativeAlcohol);
				item->setText( 5,  absolutAlcohol);
				item->setText( 6,  ID);
				counter++;
			}
		}
	textLabel1_5->setText(QString::number(counter)+" Cocktails");
	}
}

void cocktailviewerWidget::ListView1Clicked()
{
	QListViewItem *item;
	QString ID, Name, description, rating, type, taste1, taste2;
	item=listView1->selectedItem();
	ID=item->text(6);
	Name=item->text(0);
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
	rc = sqlite3_get_table(db, "SELECT * FROM TMPCocktailExtras WHERE cocktailID="+ID, &Result2, &nrow2, &ncolumn2, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		qDebug("4");
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	}
	Text2+=description+"<br><br><b>"+QString::number(QString(Result2[11]).toFloat(),'f', 2)+" EUR</b>";
	textLabel1_3->setText(Text2);
	Text="<font size=\"+2\">"+Name+" "+printStars(rating.toInt())+"</font><br><font size=\"-4\"><b>"+type+" "+taste1+" "+taste2+"<b></font>";
	textLabel1_4->setText(Text);
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
		QTime t;
		t.start();
		char *zErrMsg = 0;
		int rc, ncolumn;
		char **result;
		//sqlite3_exec(db, "DROP TABLE TMPFilter"+QString::number(FilterNo)+";", 0, 0, &zErrMsg);
		rc = sqlite3_get_table(db, "SELECT CocktailID FROM CocktailIngredients WHERE IngredientID="+ID, &result, &nrow, &ncolumn, &zErrMsg);
		if( rc!=SQLITE_OK )
		{
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		}
		/*rc = sqlite3_exec(db, "CREATE TABLE TMPFilter"+QString::number(FilterNo)+" (CocktailID NUMERIC);", 0, 0, &zErrMsg);
		if( rc!=SQLITE_OK )
		{
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		}*/
		//sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, &zErrMsg);
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
			/*rc = sqlite3_exec(db, "INSERT INTO TMPFilter"+QString::number(FilterNo)+" VALUES ("+result[i]+");", 0, 0, &zErrMsg);
			if( rc!=SQLITE_OK )
			{
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
			}*/
		}
		//sqlite3_exec(db, "COMMIT TRANSACTION", 0, 0, &zErrMsg);
		qDebug( "%d ms", t.elapsed() );
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
	/*char *zErrMsg = 0;
	int rc, nrow, ncolumn;
	char **Result;
	rc = sqlite3_get_table(db, "SELECT CocktailID FROM TMPFilter"+QString::number(Filter)+" WHERE CocktailID="+ID, &Result, &nrow, &ncolumn, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	}
	if(nrow>0)
		return true;
	else
		return false;*/
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
	char *zErrMsg = 0;
	int rc, ncolumn, nrow;
	char **result;
	rc = sqlite3_get_table(db, "SELECT name FROM Ingredients ORDER BY name", &result, &nrow, &ncolumn, &zErrMsg);
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
		comboBox1->insertItem(result[i]);
		comboBox2->insertItem(result[i]);
		comboBox3->insertItem(result[i]);
		comboBox4->insertItem(result[i]);
	}
}

cocktailviewerWidget::~cocktailviewerWidget()
{
	sqlite3_close(db);
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

//#include "cocktailviewerwidget.moc"
