#include "XUPProjectManager.h"
#include "UIXUPFindFiles.h"
#include "pIconManager.h"
#include "XUPProjectModel.h"
#include "XUPFilteredProjectModel.h"
#include "MkSFileDialog.h"
#include "pMonkeyStudio.h"
#include "MonkeyCore.h"
#include "PluginsManager.h"
#include "pWorkspace.h"
#include "XUPPlugin.h"
#include "UITemplatesWizard.h"
#include "pRecentsManager.h"

#include <pDockWidgetTitleBar.h>
#include <pQueuedMessageToolBar.h>

#include <QDebug>
#include <QTextCodec>
#include <QMenu>
#include <QInputDialog>
#include <QMessageBox>
#include <QFileSystemWatcher>

XUPProjectManager::XUPProjectManager( QWidget* parent )
	: pDockWidget( parent )
{
	setupUi( this );
	cbProjects->setAttribute( Qt::WA_MacShowFocusRect, false );
	cbProjects->setAttribute( Qt::WA_MacSmallSize );
	tvNative->setAttribute( Qt::WA_MacShowFocusRect, false );
	tvNative->setAttribute( Qt::WA_MacSmallSize );
	tvFiltered->setAttribute( Qt::WA_MacShowFocusRect, false );
	tvFiltered->setAttribute( Qt::WA_MacSmallSize );
	
	setActionsManager( MonkeyCore::actionsManager() );

#ifdef QT_NO_DEBUG
	wDebug->setVisible( false );
	pteLog->setVisible( false );
#endif
	
	// register generic xup project format
	XUPProjectItem* pItem = new XUPProjectItem();
	pItem->registerProjectType();
	
	titleBar()->addAction( action( atNew ), 0 );
	titleBar()->addAction( action( atOpen ), 1 );
	titleBar()->addAction( action( atClose ), 2 );
	titleBar()->addAction( action( atCloseAll ), 3 );
	titleBar()->addAction( action( atEdit ), 4 );
	titleBar()->addAction( action( atAddFiles ), 5 );
	titleBar()->addAction( action( atRemoveFiles ), 6 );
	titleBar()->addAction( aDebug, 7 );
	titleBar()->addSeparator( 8 );
	
	mFilteredModel = new XUPFilteredProjectModel( tvFiltered );
	tvFiltered->setModel( mFilteredModel );
	
	mDebugMenu = new QMenu( this );
	aDebug->setMenu( mDebugMenu );
	
	mDebugMenu->addAction( "interpretValue" );
	mDebugMenu->addAction( "interpretVariable" );
	mDebugMenu->addAction( "project" );
	mDebugMenu->addAction( "topLevelProject" );
	mDebugMenu->addAction( "rootIncludeProject" );
	mDebugMenu->addAction( "editAttribute" );
	mDebugMenu->addAction( "removeItem" );
	mDebugMenu->addAction( "addItem" );
	mDebugMenu->addAction( "debugFilteredModel" );
	mDebugMenu->addAction( "projectSettingsDebug" );
	mDebugMenu->addAction( "saveProject" );
	
	connect( mDebugMenu, SIGNAL( triggered( QAction* ) ), this, SLOT( debugMenu_triggered( QAction* ) ) );
	
	connect( tvFiltered->selectionModel(), SIGNAL( currentChanged( const QModelIndex&, const QModelIndex& ) ), this, SLOT( tvFiltered_currentChanged( const QModelIndex&, const QModelIndex& ) ) );
	connect( tvFiltered, SIGNAL( customContextMenuRequested( const QPoint& ) ), this, SIGNAL( projectCustomContextMenuRequested( const QPoint& ) ) );
	
	setCurrentProject( 0, 0 );
}

XUPProjectManager::~XUPProjectManager()
{
	delete XUPProjectItem::projectInfos();
}

void XUPProjectManager::on_cbProjects_currentIndexChanged( int id )
{
	XUPProjectModel* model = cbProjects->itemData( id ).value<XUPProjectModel*>();
	XUPProjectItem* project = model ? model->mRootProject : 0;
	setCurrentProject( project, currentProject() );
}

void XUPProjectManager::debugMenu_triggered( QAction* action )
{
	XUPItem* item = currentItem();
	
	if ( !item )
		return;
	
	addError( "------------------" );
	
	QString attribute;
	
	switch ( item->type() )
	{
		case XUPItem::Project:
			attribute = "name";
			break;
		case XUPItem::Comment:
			attribute = "value";
			break;
		case XUPItem::EmptyLine:
			attribute = "count";
			break;
		case XUPItem::Variable:
			attribute = "name";
			break;
		case XUPItem::Value:
			attribute = "content";
			break;
		case XUPItem::Function:
			attribute = "parameters";
			break;
		case XUPItem::Scope:
			attribute = "name";
			break;
		case XUPItem::DynamicFolder:
			attribute = "name";
			break;
		case XUPItem::Folder:
			attribute = "name";
			break;
		case XUPItem::File:
			attribute = "content";
			break;
		case XUPItem::Path:
			attribute = "content";
			break;
		case XUPItem::Unknow:
			break;
	}
	
	if ( action->text() == "interpretValue" )
	{
		if ( item->type() == XUPItem::Value )
		{
			addError( item->attribute( attribute ).prepend( "Interpret value '" ).append( "'" ) );
			addError( item->cacheValue( attribute ) );
		}
	}
	else if ( action->text() == "interpretVariable" )
	{
		if ( item->type() == XUPItem::Variable )
		{
			addError( item->attribute( attribute ).prepend( "Interpret variable '" ).append( "'" ) );
			addError( item->project()->rootIncludeProject()->variableCache().value( item->attribute( attribute ) ) );
		}
	}
	else if ( action->text() == "project" )
	{
		addError( item->project()->displayText().prepend( "Project: " ) );
	}
	else if ( action->text() == "topLevelProject" )
	{
		addError( item->project()->topLevelProject()->displayText().prepend( "Top level project: " ) );
	}
	else if ( action->text() == "rootIncludeProject" )
	{
		addError( item->project()->rootIncludeProject()->displayText().prepend( "Root include project: " ) );
	}
	else if ( action->text() == "editAttribute" )
	{
		bool ok;
		QString value = QInputDialog::getText( this, "title", "label", QLineEdit::Normal, item->attribute( attribute ), &ok );
		if ( ok && !value.isEmpty() )
		{
			item->setAttribute( attribute, value );
		}
	}
	else if ( action->text() == "removeItem" )
	{
		XUPItem* parentItem = item->parent();
		if ( parentItem )
		{
			parentItem->removeChild( item );
		}
	}
	else if ( action->text() == "addItem" )
	{
		int row = qrand() % item->childCount();
		XUPItem* child = item->addChild( XUPItem::File, row );
		if ( child )
		{
			child->setAttribute( "content", QString( "my new item %1" ).arg( qrand() % 256 ) );
		}
	}
	else if ( action->text() == "debugFilteredModel" )
	{
		mFilteredModel->debug( item->project() );
	}
	else if ( action->text() == "projectSettingsDebug" )
	{
		XUPProjectItem* project = item->project();
		
		project->setProjectSettingsValue( "EDITOR", "SimpleQMakeEditor" );
		project->setProjectSettingsValue( "DEBUG_BINARY", "myappd" );
		project->setProjectSettingsValue( "RELEASE_BINARY", "myapp" );
		project->setProjectSettingsValue( "TEST", "merde" );
		
		project->addProjectSettingsValue( "TEST", "youpi" );
		
		QStringList values;
		values << project->projectSettingsValue( "EDITOR" );
		values << project->projectSettingsValue( "DEBUG_BINARY" );
		values << project->projectSettingsValue( "RELEASE_BINARY" );
		values << project->projectSettingsValue( "TEST" );
		addError( values.join( " " ) );
		
		project->setProjectSettingsValue( "TEST", "okimichel" );
		addError( project->projectSettingsValue( "TEST" ) );
	}
	else if ( action->text() == "saveProject" )
	{
		XUPProjectItem* project = item->project();
		
		if ( project )
		{
			addError( project->toString() );
		}
	}
}

void XUPProjectManager::on_pbFiltered_toggled( bool checked )
{
	pbNative->setChecked( !checked );
	
	if ( checked )
	{
		swPages->setCurrentIndex( 0 );
	}
}

void XUPProjectManager::on_pbNative_toggled( bool checked )
{
	pbFiltered->setChecked( !checked );
	
	if ( checked )
	{
		swPages->setCurrentIndex( 1 );
	}
}

void XUPProjectManager::tvFiltered_currentChanged( const QModelIndex& current, const QModelIndex& previous )
{
	XUPItem* curItem = mFilteredModel->mapToSource( current );
	XUPItem* preItem = mFilteredModel->mapToSource( previous );
	XUPProjectItem* curProject = curItem ? curItem->project() : 0;
	XUPProjectItem* preProject = preItem ? preItem->project() : 0;
	
	if ( !previous.isValid() )
	{
		preProject = curProject->topLevelProject();
	}
	
	setCurrentProject( curProject, preProject );
}

void XUPProjectManager::on_tvFiltered_activated( const QModelIndex& index )
{
	XUPItem* item = mFilteredModel->mapToSource( index );
	if ( item )
	{
		if ( item->type() == XUPItem::Project )
		{
			emit projectDoubleClicked( item->project() );
		}
		else if ( item->type() == XUPItem::File )
		{
			XUPProjectItem* project = item->project();
			XUPProjectItem* rootIncludeProject = project->rootIncludeProject();
			QString fn = rootIncludeProject->filePath( item->cacheValue( "content" ) );
			
			if ( !QFile::exists( fn ) )
			{
				QString findFile = item->attribute( "content" ).remove( '"' );
				QFileInfoList files = rootIncludeProject->findFile( findFile );
				switch ( files.count() )
				{
					case 0:
						fn.clear();
						break;
					case 1:
						fn = files.at( 0 ).absoluteFilePath();
						break;
					default:
					{
						UIXUPFindFiles dlg( findFile, this );
						dlg.setFiles( files, rootIncludeProject->path() );
						fn.clear();
						if ( dlg.exec() == QDialog::Accepted )
						{
							fn = dlg.selectedFile();
						}
						break;
					}
				}
			}
			
			if ( QFile::exists( fn ) )
			{
				const QString codec = project->temporaryValue( "codec" ).toString();
				emit fileDoubleClicked( project, fn, codec );
				emit fileDoubleClicked( fn, codec );
			}
		}
	}
}

void XUPProjectManager::on_tvFiltered_customContextMenuRequested( const QPoint& pos )
{
	if ( currentProject() )
	{
		// get menubar
		pMenuBar* mb = MonkeyCore::menuBar();
		QMenu menu;
		
		// add menu commands
		menu.addActions( mb->menu( "mProject" )->actions() );
		menu.addSeparator();
		menu.addActions( mb->menu( "mBuilder" )->actions() );
		menu.addSeparator();
		menu.addActions( mb->menu( "mDebugger" )->actions() );
		menu.addSeparator();
		menu.addActions( mb->menu( "mInterpreter" )->actions() );
		
		// show menu
		menu.exec( tvFiltered->mapToGlobal( pos ) );
	}
}

QAction* XUPProjectManager::action( XUPProjectManager::ActionType type )
{
	if ( mActions.contains( type ) )
	{
		return mActions[ type ];
	}
	
	QAction* action = 0;
	switch ( type )
	{
		case XUPProjectManager::atNew:
			action = new QAction( pIconManager::icon( "new.png", ":/project" ), tr( "New project..." ), this );
			mActionsManager->setDefaultShortcut( action, tr( "Ctrl+Shift+N" ) );
			connect( action, SIGNAL( triggered() ), this, SLOT( newProject() ) );
			break;
		case XUPProjectManager::atOpen:
			action = new QAction( pIconManager::icon( "open.png", ":/project" ), tr( "Open project..." ), this );
			mActionsManager->setDefaultShortcut( action, tr( "Ctrl+Shift+O" ) );
			connect( action, SIGNAL( triggered() ), this, SLOT( openProject() ) );
			break;
		case XUPProjectManager::atClose:
			action = new QAction( pIconManager::icon( "close.png", ":/project" ), tr( "Close current project" ), this );
			connect( action, SIGNAL( triggered() ), this, SLOT( closeProject() ) );
			break;
		case XUPProjectManager::atCloseAll:
			action = new QAction( pIconManager::icon( "closeall.png", ":/project" ), tr( "Close all projects" ), this );
			connect( action, SIGNAL( triggered() ), this, SLOT( closeAllProjects() ) );
			break;
		case XUPProjectManager::atEdit:
			action = new QAction( pIconManager::icon( "settings.png", ":/project" ), tr( "Edit current project..." ), this );
			connect( action, SIGNAL( triggered() ), this, SLOT( editProject() ) );
			break;
		case XUPProjectManager::atAddFiles:
			action = new QAction( pIconManager::icon( "add.png", ":/project" ), tr( "Add existing files to current project..." ), this );
			connect( action, SIGNAL( triggered() ), this, SLOT( addFiles() ) );
			break;
		case XUPProjectManager::atRemoveFiles:
			action = new QAction( pIconManager::icon( "remove.png", ":/project" ), tr( "Remove files from current project..." ), this );
			connect( action, SIGNAL( triggered() ), this, SLOT( removeFiles() ) );
			break;
	}
	
	if ( action )
	{
		action->setToolTip( action->text() );
		action->setStatusTip( action->text() );
		mActions[ type ] = action;
	}
	
	return action;
}

void XUPProjectManager::addError( const QString& error )
{
	pteLog->appendPlainText( error );
}

void XUPProjectManager::newProject()
{
	UITemplatesWizard wizard( this );
	wizard.setType( "Projects" );
	wizard.exec();
}

bool XUPProjectManager::openProject( const QString& fileName, const QString& codec )
{
	// check that project is not yet open
	foreach ( XUPProjectItem* project, topLevelProjects() )
	{
		if ( pMonkeyStudio::isSameFile( project->fileName(), fileName ) )
		{
			setCurrentProject( project, currentProject() );
			return true;
		}
	}
	
	QFileInfo fi( fileName );
	
	if ( fi.exists() && fi.isFile() )
	{
		XUPProjectModel* model = new XUPProjectModel( this );
		if ( model->open( fileName, codec ) )
		{
			// append file to recents project
			MonkeyCore::recentsManager()->addRecentProject( fileName );
			
			model->registerWithFileWatcher( MonkeyCore::workspace()->fileWatcher() );
			
			int id = cbProjects->count();
			cbProjects->addItem( model->headerData( 0, Qt::Horizontal, Qt::DisplayRole ).toString(), QVariant::fromValue<XUPProjectModel*>( model ) );
			cbProjects->setItemIcon( id, model->headerData( 0, Qt::Horizontal, Qt::DecorationRole ).value<QIcon>() );
			setCurrentProject( model->rootProject(), currentProject() );
			emit projectOpened( currentProject() );
			
			return true;
		}
		else
		{
			// remove from recents
			MonkeyCore::recentsManager()->removeRecentProject( fileName );
			
			// inform user about error
			MonkeyCore::messageManager()->appendMessage( tr( "An error occur while opening project '%1': %2" ).arg( fileName ).arg( model->lastError() ) );
			
			pteLog->appendPlainText( model->lastError() );
			delete model;
		}
	}
	
	return false;
}

bool XUPProjectManager::openProject()
{
	pFileDialogResult result = MkSFileDialog::getOpenProjects( window() );
	
	const QStringList files = result[ "filenames" ].toStringList();
	if ( !files.isEmpty() )
	{
		const QString codec = result[ "codec" ].toString();
		foreach ( const QString file, files )
		{
			if ( !openProject( file, codec ) )
			{
				return false;
			}
		}
		
		return true;
	}
	
	return false;
}

void XUPProjectManager::closeProject()
{
	XUPProjectModel* curModel = currentProjectModel();
	
	if ( curModel )
	{
		XUPProjectItem* preProject = currentProject();
		
		bool blocked = cbProjects->blockSignals( true );
		int id = cbProjects->findData( QVariant::fromValue<XUPProjectModel*>( curModel ) );
		cbProjects->removeItem( id );
		XUPProjectModel* model = cbProjects->itemData( cbProjects->currentIndex() ).value<XUPProjectModel*>();
		setCurrentProjectModel( model );
		cbProjects->blockSignals( blocked );
		
		XUPProjectItem* curProject = currentProject();
		
		setCurrentProject( curProject, preProject );
		
		emit projectAboutToClose( preProject );
		
		curModel->unregisterWithFileWatcher( MonkeyCore::workspace()->fileWatcher() );
		curModel->close();
		delete curModel;
	}
}

void XUPProjectManager::closeAllProjects()
{
	while ( cbProjects->count() > 0 )
	{
		closeProject();
	}
}

void XUPProjectManager::editProject()
{
	XUPProjectItem* project = currentProject();
	
	if ( !project )
	{
		return;
	}
	
	XUPProjectItem* topLevelProject = project->topLevelProject();
	
	// get plugin name that can manage this project
	const QString pluginName = topLevelProject->projectSettingsValue( "EDITOR" );
	
	if ( pluginName.isEmpty() || !MonkeyCore::pluginsManager()->plugins<XUPPlugin*>( PluginsManager::stAll, pluginName ).value( 0 ) )
	{
		// get xup plugins
		QHash<QString, XUPPlugin*> plugins;
		
		foreach ( XUPPlugin* plugin, MonkeyCore::pluginsManager()->plugins<XUPPlugin*>( PluginsManager::stAll ) )
		{
			plugins[ plugin->infos().Caption ] = plugin;
		}
	
		bool ok;
		const QString caption = QInputDialog::getItem( window(), tr( "Choose an editor plugin..." ), tr( "Your project is not yet editable, please select a correct project settings plugin" ), plugins.keys(), 0, false, &ok );
		
		if ( ok && !caption.isEmpty() )
		{
			topLevelProject->setProjectSettingsValue( "EDITOR", plugins[ caption ]->infos().Name );
		}
	}
	
	// edit project settings
	if ( topLevelProject->projectSettingsValue( "EDITOR" ).isEmpty() )
	{
		QMessageBox::warning( QApplication::activeWindow(), tr( "Warning..." ), tr( "The project can't be edited because there is no associate project settings plugin." ) );
		return;
	}
	
	XUPPlugin* plugin = MonkeyCore::pluginsManager()->plugins<XUPPlugin*>( PluginsManager::stAll, topLevelProject->projectSettingsValue( "EDITOR" ) ).value( 0 );
	
	if ( plugin )
	{
		XUPProjectModel* model = currentProjectModel();
		QFileSystemWatcher* watcher = MonkeyCore::workspace()->fileWatcher();
		
		model->unregisterWithFileWatcher( watcher, project );
		
		// edit project and save it if needed
		if ( plugin->editProject( project ) )
		{
			if ( project->save() )
			{
				// need save topLevelProject ( for XUPProejctSettings scope  )
				if ( !topLevelProject->save() )
				{
					addError( topLevelProject->lastError() );
				}
			}
			else
			{
				addError( project->lastError() );
			}
			
			// rebuild cache
			project->rebuildCache();
			
			// update menu actions
			project->uninstallCommands();
			project->installCommands();
		}
		
		model->registerWithFileWatcher( watcher, project );
	}
}

void XUPProjectManager::addFilesToScope( XUPItem* scope, const QStringList& allFiles, const QString& ope )
{
	QStringList files = allFiles;
	XUPProjectItem* project = scope->project();
	XUPProjectItem* rootIncludeProject = project->rootIncludeProject();
	const StringStringListList mVariableSuffixes = project->projectInfos()->variableSuffixes( project->projectType() );
	QMap<QString, QString> variablesOperator; // variableName, operator
	QMap<QString, QStringList> suffixeVariables; // suffixe, variable
	
	// this map allow to know if a suffixe can be handled by more than one variable
	foreach ( const PairStringStringList& pair, mVariableSuffixes )
	{
		foreach ( QString suffixe, pair.second )
		{
			suffixe = suffixe.toLower();
			
			if ( !suffixeVariables[ suffixe ].contains( pair.first ) )
			{
				suffixeVariables[ suffixe ] << pair.first;
			}
		}
	}
	
	foreach ( const QString& file, files )
	{
		foreach ( const QString& suffixe, suffixeVariables.keys() )
		{
			if ( QDir::match( suffixe, file ) )
			{
				const QStringList variablesName = suffixeVariables[ suffixe ];
			
				QString variableName;
				if ( variablesName.count() > 1 )
				{
					bool ok;
					variableName = QInputDialog::getItem( QApplication::activeWindow(), tr( "Choose variable..." ), tr( "More than one variable can handle this file,\nplease select the variable you want to use for this file :\n%1" ).arg( QFileInfo( file ).fileName() ), variablesName, 0, false, &ok );
					
					if ( !ok )
					{
						variableName.clear();
					}
				}
				else
				{
					variableName = variablesName.at( 0 );
				}
				
				if ( variableName.isEmpty() )
				{
					variableName = "OTHER_FILES";
				}
				
				XUPItemList variables = project->getVariables( scope, variableName, 0, false );
				
				bool foundFile = false;
				XUPItem* usedVariable = 0;
				QString op = ope.isEmpty() ? variablesOperator[ variableName ] : ope;
				
				if ( op.isEmpty() )
				{
					op = checkForBestAddOperator( variables );
				}
				
				foreach ( XUPItem* variable, variables )
				{
					if ( variable->attribute( "operator", "=" ) != op )
					{
						continue;
					}
					
					usedVariable = variable;
					
					foreach ( XUPItem* child, variable->childrenList() )
					{
						if ( child->type() != XUPItem::File )
						{
							continue;
						}
						
						const QString fn = rootIncludeProject->filePath( child->cacheValue( "content" ) );
						
						if ( fn == file )
						{
							foundFile = true;
							break;
						}
					}
					
					if ( foundFile )
					{
						break;
					}
				}
				
				if ( foundFile )
				{
					break;
				}
				
				if ( !usedVariable )
				{
					usedVariable = scope->addChild( XUPItem::Variable );
					usedVariable->setAttribute( "name", variableName );
					usedVariable->setAttribute( "operator", op );
					variables << usedVariable;
				}
				
				usedVariable->setAttribute( "multiline", "true" );
				
				XUPItem* value = usedVariable->addChild( XUPItem::File );
				value->setAttribute( "content", project->relativeFilePath( file ) );
				
				break;
			}
		}
	}
	
	// rebuild cache
	project->rebuildCache();
	
	// save project
	if ( !project->save() )
	{
		addError( project->lastError() );
	}
}

QString XUPProjectManager::checkForBestAddOperator( const XUPItemList& variables ) const
{
	bool haveSet = false;
	bool haveAdd = false;
	bool haveAddIfNotExists = false;
	
	foreach ( const XUPItem* variable, variables )
	{
		const QString op = variable->attribute( "operator", "=" );
		
		if ( op == "*=" )
		{
			haveAddIfNotExists = true;
		}
		else if ( op == "+=" )
		{
			haveAdd = true;
		}
		else if ( op == "=" )
		{
			haveSet = true;
		}
	}
	
	if ( haveAddIfNotExists )
	{
		return "*=";
	}
	
	if ( haveAdd )
	{
		return "+=";
	}
	
	if ( haveSet )
	{
		return "=";
	}
	
	return "+=";
}

void XUPProjectManager::addFiles()
{
	pFileDialogResult result = MkSFileDialog::getProjectAddFiles( window() );
	
	if ( !result.isEmpty() )
	{
		QStringList files = result[ "filenames" ].toStringList();
		XUPItem* scope = result[ "scope" ].value<XUPItem*>();
		
		// import files if needed
		if ( result[ "import" ].toBool() )
		{
			const QString projectPath = scope->project()->path();
			const QString importPath = result[ "importpath" ].toString();
			const QString importRootPath = result[ "directory" ].toString();
			QDir dir( importRootPath );
			
			for ( int i = 0; i < files.count(); i++ )
			{
				if ( !files.at( i ).startsWith( projectPath ) )
				{
					QString fn = QString( files.at( i ) ).remove( importRootPath ).replace( "\\", "/" );
					fn = QDir::cleanPath( QString( "%1/%2/%3" ).arg( projectPath ).arg( importPath ).arg( fn ) );
					
					if ( dir.mkpath( QFileInfo( fn ).absolutePath() ) && QFile::copy( files.at( i ), fn ) )
					{
						files[ i ] = fn;
					}
				}
			}
		}
		
		// add files to scope
		addFilesToScope( scope, files );
	}
}

void XUPProjectManager::removeFiles()
{
	XUPItem* curItem = currentItem();
	
	
	if ( !curItem || !( curItem->type() == XUPItem::Value || curItem->type() == XUPItem::File || curItem->type() == XUPItem::Path ) )
	{
		return;
	}
	
	if ( QMessageBox::question( window(), tr( "Remove Value..." ), tr( "Are you sur you want to remove this value ?" ), QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
	{
		XUPProjectItem* project = curItem->project();
		
		// if file item
		if ( curItem->type() == XUPItem::File )
		{
			XUPProjectItem* rootIncludeProject = project->rootIncludeProject();
			const QString fp = rootIncludeProject->filePath( curItem->cacheValue( "content" ) );
			
			// ask removing file
			if ( QFile::exists( fp ) && QMessageBox::question( window(), tr( "Delete associations..." ), tr( "Do you want to delete the associate file ?" ), QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
			{
				if ( !QFile::remove( fp ) )
				{
					QMessageBox::warning( window(), tr( "Error..." ), tr( "Can't delete file: %1" ).arg( fp ) );
				}
			}
		}
		
		// remove value & variable if needed
		XUPItem* variable = curItem->parent();
		XUPItem* variableParent = variable->parent();
		
		variable->removeChild( curItem );
		
		if ( variable->childCount() == 0 )
		{
			variableParent->removeChild( variable );
		}
		
		// rebuild cache
		project->rebuildCache();
		
		// save project
		if ( !project->save() )
		{
			addError( project->lastError() );
		}
	}
}

XUPProjectModel* XUPProjectManager::currentProjectModel() const
{
	return mFilteredModel->sourceModel();
}

XUPProjectItem* XUPProjectManager::currentProject() const
{
	XUPProjectModel* curModel = currentProjectModel();
	XUPItem* curItem = currentItem();
	
	if ( curItem )
	{
		return curItem->project();
	}
	
	if ( !curItem && curModel )
	{
		return curModel->mRootProject;
	}
	
	return 0;
}

XUPItem* XUPProjectManager::currentItem() const
{
	const QModelIndex index = tvFiltered->currentIndex();
	return mFilteredModel->mapToSource( index );
}

XUPProjectItemList XUPProjectManager::topLevelProjects() const
{
	XUPProjectItemList projects;
	
	for ( int i = 0; i < cbProjects->count(); i++ )
	{
		XUPProjectItem* project = cbProjects->itemData( i ).value<XUPProjectModel*>()->mRootProject;
		projects << project;
	}
	
	return projects;
}

void XUPProjectManager::setCurrentProjectModel( XUPProjectModel* model )
{
	mFilteredModel->setSourceModel( model );
	tvNative->setModel( model );
	
	int id = cbProjects->findData( QVariant::fromValue<XUPProjectModel*>( model ) );
	cbProjects->setCurrentIndex( id );
}

void XUPProjectManager::setCurrentProject( XUPProjectItem* curProject, XUPProjectItem* preProject )
{
	// update project actions
	action( atClose )->setEnabled( curProject );
	action( atCloseAll )->setEnabled( curProject );
	action( atEdit )->setEnabled( curProject );
	action( atAddFiles )->setEnabled( curProject );
	action( atRemoveFiles )->setEnabled( curProject );
	
	if ( !curProject )
	{
		setCurrentProjectModel( 0 );
	}
	else if ( curProject->model() != currentProjectModel() )
	{
		setCurrentProjectModel( curProject->model() );
	}
	
	// if new project != old update gui
	if ( curProject != preProject )
	{
		emit currentProjectChanged( curProject, preProject );
		emit currentProjectChanged( curProject );
	}
}
