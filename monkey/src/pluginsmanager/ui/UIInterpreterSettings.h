/****************************************************************************
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
****************************************************************************/
#ifndef UIINTERPRETERSETTINGS_H
#define UIINTERPRETERSETTINGS_H

#include <fresh.h>
#include <pCommand.h>

#include "ui_UIInterpreterSettings.h"

#include <QWidget>

class InterpreterPlugin;

class Q_MONKEY_EXPORT UIInterpreterSettings : public QWidget, public Ui::UIInterpreterSettings
{
	Q_OBJECT
	
protected:
	pCommand mDefault;
	pCommand mReset;
	pCommand mCommand;
	InterpreterPlugin* mPlugin;

public:
	UIInterpreterSettings( InterpreterPlugin*, QWidget* = 0 );

	void updateCommand();
	void restoreDefault();
	void reset();
	void save();

protected slots:
	void on_tbBuildCommandCommand_clicked();
	void on_tbBuildCommandWorkingDirectory_clicked();
	void on_dbbButtons_clicked( QAbstractButton* );
};

#endif // UIINTERPRETERSETTINGS_H
