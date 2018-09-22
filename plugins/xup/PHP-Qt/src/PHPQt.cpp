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
#include "PHPQt.h"
#include "PHPQtProjectItem.h"
//#include "UISettingsPHPQt.h"
#include "../XUP/src/gui/UIXUPEditor.h"

#include <MonkeyCore.h>
#include <UIMain.h>

#include <QDir>

void PHPQt::fillPluginInfos()
{
	mPluginInfos.Caption = tr( "PHP-Qt Project" );
	mPluginInfos.Description = tr( "PHP-Qt Project support for XUPManager" );
	mPluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>";
	mPluginInfos.Type = BasePlugin::iXUP;
	mPluginInfos.Name = PLUGIN_NAME;
	mPluginInfos.Version = "0.1.0";
	mPluginInfos.FirstStartEnabled = true;
	mPluginInfos.HaveSettingsWidget = false;
}


bool PHPQt::install()
{
	// register phpqt item
	mItem = new PHPQtProjectItem;
	mItem->registerProjectType();
	return true;
}

bool PHPQt::uninstall()
{
	// unregister qmake item, unregistering auto delete the item
	mItem->unRegisterProjectType();
	delete mItem;
	return true;
}

bool PHPQt::editProject( XUPProjectItem* project )
{
		if ( !project )
		{
		return false;
	}

	return UIXUPEditor( project, MonkeyCore::mainWindow() ).exec() == QDialog::Accepted;
}

Q_EXPORT_PLUGIN2( ProjectPHPQt, PHPQt )