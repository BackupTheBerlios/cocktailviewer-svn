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


#include <qapplication.h>
#include <qsettings.h>
#include "cocktailviewerwidget.h"

QString dir;

int main( int argc, char ** argv ) {
    QApplication a( argc, argv );
    int returnvalue;
    dir=argv[0];
    int i=dir.findRev('/',-1);
    dir.remove(i+1, dir.length());
    QSettings settings;
    settings.setPath("Benni", "CocktailViewer");
    QPoint p( settings.readNumEntry( "/cocktailviewer/xPos", 0 ), settings.readNumEntry( "/cocktailviewer/yPos", 0 ) );
    QSize s( settings.readNumEntry( "/cocktailviewer/width", 750 ), settings.readNumEntry( "/cocktailviewer/height", 530 ) );
    cocktailviewerWidget * mw = new cocktailviewerWidget();
    mw->setCaption( "CocktailViewer" );
    mw->resize( s );
    mw->move( p ); 
    mw->show();
    a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
    returnvalue=a.exec();
    settings.writeEntry( "/cocktailviewer/width", mw->width() );
    settings.writeEntry( "/cocktailviewer/height", mw->height() );
    settings.writeEntry( "/cocktailviewer/xPos", mw->x() );
    settings.writeEntry( "/cocktailviewer/yPos", mw->y() );
    return returnvalue;
}
