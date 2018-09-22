# Monkey Studio 2 project file - 2005 - 2009

# include qscintilla framework
include( ../qscintilla/qscintilla.pri )

# include fresh framework
include( ../fresh/fresh.pri )

# include ctags framework
include( ../ctags/ctags.pri )

# include qCtagsSense framework
include( ../qCtagsSense/qCtagsSense.pri )

# include monkey framework
include( monkey.pri )

# include config file
include( ../config.pri )

TEMPLATE	= app
TARGET	= $$PACKAGE_TARGET
DESTDIR	= $$PACKAGE_DESTDIR

mac:ICON	= src/resources/icons/application/monkey2.icns

win32:RC_FILE	*= monkey.rc
RESOURCES	*= src/resources/resources.qrc

DEFINES	*= MONKEY_CORE_BUILD

LIBS	*= -L$${PACKAGE_BUILD_PATH}
mac:*-g++:LIBS	*= -Wl,-all_load # import all symbols as the not used ones too
else:*-g++:LIBS	*= -Wl,--whole-archive # import all symbols as the not used ones too
mac:*-g++:LIBS	*= -dynamic
else:unix:*-g++:LIBS	*= -rdynamic

isEqual( SYSTEM_QSCINTILLA, 1 ):PRE_TARGETDEPS	*= ../qscintilla
PRE_TARGETDEPS	*= ../fresh ../ctags ../qCtagsSense

CONFIG( debug, debug|release ) {
	#Debug
	!isEqual( SYSTEM_QSCINTILLA, 1 ) {
		unix:	LIBS	*= -lqscintilla2_debug -lfresh_debug -lctags_debug -lqCtagsSense_debug
		else:	LIBS	*= -lqscintilla2d -lfreshd -lctagsd -lqCtagsSensed
	} else {
		unix:	LIBS	*= -lqscintilla2 -lfresh_debug -lctags_debug -lqCtagsSense_debug
		else:	LIBS	*= -lqscintilla2 -lfreshd -lctagsd -lqCtagsSensed
	}
	win32-*g++:LIBS	*= -Wl,--out-implib,$${PACKAGE_BUILD_PATH}/lib$${TARGET}.a
	win32-msvc*:LIBS	*= /IMPLIB:$${PACKAGE_BUILD_PATH}/$${TARGET}.lib -lshell32
} else {
	#Release
	LIBS	*= -lqscintilla2 -lctags -lfresh -lqCtagsSense
	win32-*g++:LIBS	*= -Wl,--out-implib,$${PACKAGE_BUILD_PATH}/lib$${TARGET}.a
	win32-msvc*:LIBS	*= /IMPLIB:$${PACKAGE_BUILD_PATH}/$${TARGET}.lib -lshell32
}

mac:*-g++:LIBS	*= -Wl,-noall_load # stop importing all symbols
else:*-g++:LIBS	*= -Wl,--no-whole-archive # stop importing all symbols

FORMS	*= src/maininterface/ui/UITranslator.ui \
	src/maininterface/ui/UIAbout.ui \
	src/maininterface/ui/UISettings.ui \
	src/abbreviationsmanager/ui/UIAddAbbreviation.ui \
	src/templatesmanager/ui/UITemplatesWizard.ui \
	src/toolsmanager/ui/UIDesktopTools.ui \
	src/toolsmanager/ui/UIToolsEdit.ui \
	src/pluginsmanager/ui/UIPluginsSettings.ui \
	src/pluginsmanager/ui/UICLIToolSettings.ui \
	src/pluginsmanager/ui/UIBuilderSettings.ui \
	src/pluginsmanager/ui/UICompilerSettings.ui \
	src/pluginsmanager/ui/UIPluginsSettingsElement.ui \
	src/pluginsmanager/ui/UIPluginsSettingsAbout.ui \
	src/xupmanager/gui/UIXUPFindFiles.ui \
	src/xupmanager/gui/XUPProjectManager.ui \
	src/xupmanager/gui/XUPAddFiles.ui \
	src/pluginsmanager/ui/UIInterpreterSettings.ui \
	src/xupmanager/gui/CommandsEditor.ui \
	src/xupmanager/gui/VariablesEditor.ui \
	src/workspace/pOpenedFileExplorer.ui

HEADERS	*= src/main.h \
	src/maininterface/ui/UITranslator.h \
	src/maininterface/ui/UIAbout.h \
	src/maininterface/ui/UISettings.h \
	src/recentsmanager/pRecentsManager.h \
	src/workspace/pAbstractChild.h \
	src/qscintillamanager/pEditor.h \
	src/qscintillamanager/qSciShortcutsManager.h \
	src/workspace/pChild.h \
	src/workspace/UISaveFiles.h \
	src/workspace/pFileManager.h \
	src/workspace/pWorkspace.h \
	src/maininterface/UIMain.h \
	src/abbreviationsmanager/pAbbreviationsManager.h \
	src/abbreviationsmanager/ui/UIAddAbbreviation.h \
	src/variablesmanager/VariablesManager.h \
	src/templatesmanager/pTemplatesManager.h \
	src/templatesmanager/ui/UITemplatesWizard.h \
	src/pMonkeyStudio.h \
	src/toolsmanager/pDesktopApplications.h \
	src/toolsmanager/ui/UIDesktopTools.h \
	src/toolsmanager/ui/UIToolsEdit.h \
	src/consolemanager/pConsoleManager.h \
	src/consolemanager/AbstractCommandParser.h \
	src/consolemanager/CommandParser.h \
	src/consolemanager/pCommand.h \
	src/toolsmanager/pToolsManager.h \
	src/pluginsmanager/BasePlugin.h \
	src/pluginsmanager/XUPPlugin.h \
	src/pluginsmanager/ChildPlugin.h \
	src/pluginsmanager/CLIToolPlugin.h \
	src/pluginsmanager/BuilderPlugin.h \
	src/pluginsmanager/CompilerPlugin.h \
	src/pluginsmanager/PluginsManager.h \
	src/pluginsmanager/ui/UIPluginsSettings.h \
	src/pluginsmanager/ui/UICLIToolSettings.h \
	src/pluginsmanager/ui/UIBuilderSettings.h \
	src/pluginsmanager/ui/UICompilerSettings.h \
	src/settingsmanager/Settings.h \
	src/coremanager/MonkeyCore.h \
	src/statusbar/StatusBar.h \
	src/pluginsmanager/ui/UIPluginsSettingsElement.h \
	src/pluginsmanager/ui/UIPluginsSettingsAbout.h \
	src/xupmanager/core/XUPFilteredProjectModel.h \
	src/xupmanager/core/XUPItem.h \
	src/xupmanager/core/XUPProjectItem.h \
	src/xupmanager/core/XUPProjectItemInfos.h \
	src/xupmanager/core/XUPProjectModel.h \
	src/xupmanager/gui/UIXUPFindFiles.h \
	src/xupmanager/gui/XUPProjectManager.h \
	src/xupmanager/core/XUPProjectModelProxy.h \
	src/xupmanager/gui/XUPAddFiles.h \
	src/shared/MkSFileDialog.h \
	src/pluginsmanager/ui/UIInterpreterSettings.h \
	src/commandlinemanager/CommandLineManager.h \
	src/shellmanager/MkSShellConsole.h \
	src/shellmanager/MkSShellInterpreter.h \
	src/pluginsmanager/PluginsMenu.h \
	src/xupmanager/gui/CommandsEditor.h \
	src/xupmanager/core/XUPProjectItemHelper.h \
	src/xupmanager/gui/VariablesEditor.h \
	src/workspace/pOpenedFileExplorer.h \
	src/workspace/pOpenedFileModel.h

SOURCES	*= src/maininterface/ui/UITranslator.cpp \
	src/maininterface/ui/UIAbout.cpp \
	src/maininterface/ui/UISettings.cpp \
	src/recentsmanager/pRecentsManager.cpp \
	src/qscintillamanager/pEditor.cpp \
	src/qscintillamanager/qSciShortcutsManager.cpp \
	src/workspace/pChild.cpp \
	src/workspace/UISaveFiles.cpp \
	src/workspace/pFileManager.cpp \
	src/workspace/pWorkspace.cpp \
	src/maininterface/UIMain.cpp \
	src/abbreviationsmanager/pAbbreviationsManager.cpp \
	src/abbreviationsmanager/ui/UIAddAbbreviation.cpp \
	src/variablesmanager/VariablesManager.cpp \
	src/templatesmanager/pTemplatesManager.cpp \
	src/templatesmanager/ui/UITemplatesWizard.cpp \
	src/pMonkeyStudio.cpp \
	src/toolsmanager/pDesktopApplications.cpp \
	src/toolsmanager/ui/UIDesktopTools.cpp \
	src/toolsmanager/ui/UIToolsEdit.cpp \
	src/toolsmanager/pToolsManager.cpp \
	src/consolemanager/pConsoleManager.cpp \
	src/consolemanager/CommandParser.cpp \
	src/pluginsmanager/PluginsManager.cpp \
	src/pluginsmanager/BasePlugin.cpp \
	src/pluginsmanager/ui/UIPluginsSettings.cpp \
	src/pluginsmanager/ui/UICLIToolSettings.cpp \
	src/pluginsmanager/ui/UIBuilderSettings.cpp \
	src/main.cpp \
	src/pluginsmanager/ui/UICompilerSettings.cpp \
	src/settingsmanager/Settings.cpp \
	src/coremanager/MonkeyCore.cpp \
	src/statusbar/StatusBar.cpp \
	src/pluginsmanager/ui/UIPluginsSettingsElement.cpp \
	src/pluginsmanager/ui/UIPluginsSettingsAbout.cpp \
	src/xupmanager/core/XUPFilteredProjectModel.cpp \
	src/xupmanager/core/XUPItem.cpp \
	src/xupmanager/core/XUPProjectItem.cpp \
	src/xupmanager/core/XUPProjectItemInfos.cpp \
	src/xupmanager/core/XUPProjectModel.cpp \
	src/xupmanager/gui/UIXUPFindFiles.cpp \
	src/xupmanager/gui/XUPProjectManager.cpp \
	src/xupmanager/core/XUPProjectModelProxy.cpp \
	src/xupmanager/gui/XUPAddFiles.cpp \
	src/shared/MkSFileDialog.cpp \
	src/pluginsmanager/ui/UIInterpreterSettings.cpp \
	src/commandlinemanager/CommandLineManager.cpp \
	src/shellmanager/MkSShellInterpreter.cpp \
	src/shellmanager/MkSShellConsole.cpp \
	src/pluginsmanager/PluginsMenu.cpp \
	src/xupmanager/gui/CommandsEditor.cpp \
	src/xupmanager/core/XUPProjectItemHelper.cpp \
	src/xupmanager/gui/VariablesEditor.cpp \
	src/workspace/pOpenedFileExplorer.cpp \
	src/workspace/pOpenedFileModel.cpp \
	src/pluginsmanager/BuilderPlugin.cpp \
	src/pluginsmanager/CLIToolPlugin.cpp \
	src/pluginsmanager/CompilerPlugin.cpp \
	src/pluginsmanager/InterpreterPlugin.cpp

mac:SOURCES	*= src/toolsmanager/pDesktopApplications_mac.cpp
else:unix:SOURCES	*= src/toolsmanager/pDesktopApplications_unix.cpp
win32:SOURCES	*= src/toolsmanager/pDesktopApplications_win32.cpp

TRANSLATIONS	*= ../datas/translations/monkey_french.ts \
	../datas/translations/monkey_belarusian.ts \
	../datas/translations/monkey_italian.ts \
	../datas/translations/monkey_arabic.ts \
	../datas/translations/monkey_spanish.ts \
	../datas/translations/monkey_catalan.ts