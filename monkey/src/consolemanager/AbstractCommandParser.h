/****************************************************************************
**
** Authors   : Andrei Kopats aka hlamer <hlamer@tut.by>
** Project   : Monkey Studio IDE
** FileName  : AbstractCommandParser.h
** Date      : 2009-10-20T15:51:50
** License   : GPL
** Comment   : 
** Home Page : http://www.monkeystudio.org
**
	Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
**
****************************************************************************/
/*!
	\file AbstractCommandParser.h
	\author Andrei Kopats
	\brief Header for AbstractCommandParser class
*/

#ifndef ABSTRACTCOMMANDPARSER_H
#define ABSTRACTCOMMANDPARSER_H

#include "pConsoleManager.h"

#include <QObject>

/*!
	Parent class for parsers of console output.
	
	Inherit this class for create own parser. There is some description of it 
	in a wiki documentation of project
*/
class Q_MONKEY_EXPORT AbstractCommandParser : public QObject
{
	Q_OBJECT	
	
public:

	virtual QString name() const = 0;

public :
	AbstractCommandParser(QObject* parent): QObject(parent) {};
	
	
	/**
	 *\bfief Parse text
	 *\param text Text to parse
	 *\retval Count of line sucessfully parsed (text recognised, line can be discarded
	 */
	virtual int processParsing(QString* text) = 0;

signals:
	void newStepAvailable( const pConsoleManager::Step& );
};

typedef QList<AbstractCommandParser*> AbstractCommandParserList;

Q_DECLARE_METATYPE( AbstractCommandParserList );

#endif // ABSTRACTCOMMANDPARSER
