/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : pToolsManager.h
** Date      : 2008-01-14T00:37:18
** License   : GPL
** Comment   : This header has been automatically generated, if you are the original author, or co-author, fill free to replace/append with your informations.
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
#ifndef PTOOLSMANAGER_H
#define PTOOLSMANAGER_H

#include <fresh.h>

#include <QFileIconProvider>

class pSettings;
class pMenuBar;
class QAction;
	
struct Q_MONKEY_EXPORT pTool
{
	pTool( const QString& c, const QString& i, const QString& f, const QString& p, bool b = false, bool console = false )
	:	Caption( c ), FileIcon( i ), FilePath( f ), WorkingPath( p ), DesktopEntry( b ), UseConsoleManager( console ) {}
	
	pTool()
	{ DesktopEntry = false; UseConsoleManager = false; }
	
	QString toString() const
	{
		return QCoreApplication::translate( "pTool",
			"Caption: %1\n"
			"File Path: %2\n"
			"Working Path: %3\n"
			"Use Console Manager: %4\n" ).arg( Caption ).arg( FilePath ).arg( WorkingPath ).arg( QVariant( UseConsoleManager ).toString() );
	}

	QString Caption;
	QString FileIcon;
	QString FilePath;
	QString WorkingPath;
	bool DesktopEntry;
	bool UseConsoleManager;
};

class Q_MONKEY_EXPORT pToolsManager : public QObject
{
	Q_OBJECT
	friend class MonkeyCore;

public:
	enum ToolType { ttAll = -1, ttUserEntry, ttDesktopEntry };

	static const QFileIconProvider* iconProvider();
	static const QList<pTool> tools( ToolType = pToolsManager::ttAll );
	static void initializeTools();

protected:
	static QFileIconProvider* mIconProvider;

	pToolsManager( QObject* = 0 );
	~pToolsManager();

public slots:
	void editTools_triggered();
	void toolsMenu_triggered( QAction* );

};

Q_DECLARE_METATYPE( pTool )

#endif // PTOOLSMANAGER_H
