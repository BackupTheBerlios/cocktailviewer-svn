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


#ifndef _COCKTAILVIEWER_H_
#define _COCKTAILVIEWER_H_

#include <qmainwindow.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/**
 * @short Application Main Window
 * @author Benni <benjamin.loewe@freenet.de>
 * @version 0.1
 */
class cocktailviewer : public QMainWindow
{
    Q_OBJECT
public:
    /**
     * Default Constructor
     */
    cocktailviewer();

    /**
     * Default Destructor
     */
    virtual ~cocktailviewer();
};

#endif // _COCKTAILVIEWER_H_
