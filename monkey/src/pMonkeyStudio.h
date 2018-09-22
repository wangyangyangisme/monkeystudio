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
/*!
	\file pMonkeyStudio.cpp
	\date 2008-01-14T00:37:22
	\author Filipe AZEVEDO aka Nox P\@sNox <pasnox@gmail.com>
	\brief A global namespace for various options and usefull functions
*/
#ifndef PMONKEYSTUDIO_H
#define PMONKEYSTUDIO_H

#include <qscintilla.h>

#include "workspace/pWorkspace.h"
#include "workspace/pOpenedFileModel.h"

#include <QApplication>
#include <QDir>

class QTranslator;
class pEditor;

namespace pMonkeyStudio
{	
	Q_MONKEY_EXPORT void addTranslator( QTranslator* translator );
	Q_MONKEY_EXPORT void loadTranslations();

	Q_MONKEY_EXPORT bool isSameFile( const QString& left, const QString& right );

	Q_MONKEY_EXPORT QStringList availableTextCodecs();
	Q_MONKEY_EXPORT QStringList availableImageFormats();
	Q_MONKEY_EXPORT QStringList availableLanguages();

	Q_MONKEY_EXPORT QFileInfoList getFolders( QDir fromDir, const QStringList& filters, bool recursive = true );
	Q_MONKEY_EXPORT QFileInfoList getFiles( QDir fromDir, const QStringList& filters, bool recursive = true );
	Q_MONKEY_EXPORT QFileInfoList getFiles( QDir fromDir, const QString& filters = QString::null, bool recursive = true );

	Q_MONKEY_EXPORT QStringList getImageFileNames( const QString& title, const QString& filename, QWidget* parent = QApplication::activeWindow() );
	Q_MONKEY_EXPORT QString getImageFileName( const QString& title, const QString& filename, QWidget* parent = QApplication::activeWindow() );

	Q_MONKEY_EXPORT QStringList getOpenFileNames( const QString& title, const QString& filename, const QString& filters = QString(), QWidget* parent = QApplication::activeWindow() );
	Q_MONKEY_EXPORT QString getOpenFileName( const QString& title, const QString& filename, const QString& filters = QString(), QWidget* parent = QApplication::activeWindow() );
	
	Q_MONKEY_EXPORT QString getSaveFileName( const QString& title, const QString& filename, const QString& filters = QString(), QWidget* parent = QApplication::activeWindow() );

	Q_MONKEY_EXPORT QString getExistingDirectory( const QString& title, const QString& path, QWidget* parent = QApplication::activeWindow() );

	Q_MONKEY_EXPORT QString tokenizeHome( const QString& string );
	Q_MONKEY_EXPORT QString unTokenizeHome( const QString& string );
	
	Q_MONKEY_EXPORT QMap<QString, QStringList> availableLanguagesSuffixes();
	Q_MONKEY_EXPORT QMap<QString, QStringList> availableFilesSuffixes();
	Q_MONKEY_EXPORT QString availableLanguagesFilters();
	Q_MONKEY_EXPORT QString availableFilesFilters();

	Q_MONKEY_EXPORT QString settingsPath();
	Q_MONKEY_EXPORT QString scintillaSettingsPath();
	Q_MONKEY_EXPORT void prepareAPIs();
	Q_MONKEY_EXPORT QsciAPIs* apisForLexer( QsciLexer* lexer );
	Q_MONKEY_EXPORT QString languageForFileName( const QString& fileName );
	Q_MONKEY_EXPORT QsciLexer* lexerForFileName( const QString& fileName );
	Q_MONKEY_EXPORT QsciLexer* lexerForLanguage( const QString& language );
	Q_MONKEY_EXPORT bool setLexerProperty( const QString& property, QsciLexer* lexer, const QVariant& value );
	Q_MONKEY_EXPORT QVariant lexerProperty( const QString& property, QsciLexer* lexer );
	Q_MONKEY_EXPORT void resetLexer( QsciLexer* lexer );
	Q_MONKEY_EXPORT void applyProperties();
	Q_MONKEY_EXPORT void setEditorProperties( pEditor* editor );
	
	/***** GENERAL *****/
	Q_MONKEY_EXPORT void setSaveFilesOnCustomAction( bool save );
	Q_MONKEY_EXPORT bool saveFilesOnCustomAction();
	Q_MONKEY_EXPORT void setDefaultProjectsDirectory( const QString& dirName );
	Q_MONKEY_EXPORT QString defaultProjectsDirectory();
	Q_MONKEY_EXPORT void setTabsHaveCloseButton( bool have );
	Q_MONKEY_EXPORT bool tabsHaveCloseButton();
	Q_MONKEY_EXPORT void setTabsHaveShortcut( bool have );
	Q_MONKEY_EXPORT bool tabsHaveShortcut();
	Q_MONKEY_EXPORT void setTabsElided( bool elided );
	Q_MONKEY_EXPORT bool tabsElided();
	Q_MONKEY_EXPORT void setTabsTextColor( const QColor& color );
	Q_MONKEY_EXPORT QColor tabsTextColor();
	Q_MONKEY_EXPORT void setCurrentTabTextColor( const QColor& color );
	Q_MONKEY_EXPORT QColor currentTabTextColor();
	Q_MONKEY_EXPORT void setDocumentMode( pWorkspace::ViewMode mode );
	Q_MONKEY_EXPORT pWorkspace::ViewMode documentMode();
	Q_MONKEY_EXPORT void setSaveSessionOnClose( bool save );
	Q_MONKEY_EXPORT bool saveSessionOnClose();
	Q_MONKEY_EXPORT void setRestoreSessionOnStartup( bool restore );
	Q_MONKEY_EXPORT bool restoreSessionOnStartup();
	Q_MONKEY_EXPORT void setShowQuickFileAccess( bool show );
	Q_MONKEY_EXPORT bool showQuickFileAccess();
	Q_MONKEY_EXPORT void setOpenedFileSortingMode( pOpenedFileModel::SortMode mode );
	Q_MONKEY_EXPORT pOpenedFileModel::SortMode openedFileSortingMode();
	
	/******	EDITOR ******/
	// General
	Q_MONKEY_EXPORT void setAutoSyntaxCheck( bool check );
	Q_MONKEY_EXPORT bool autoSyntaxCheck();
	Q_MONKEY_EXPORT void setConvertTabsUponOpen( bool convert );
	Q_MONKEY_EXPORT bool convertTabsUponOpen();
	Q_MONKEY_EXPORT void setCreateBackupUponOpen( bool backup );
	Q_MONKEY_EXPORT bool createBackupUponOpen();
	Q_MONKEY_EXPORT void setAutoEolConversion( bool convert );
	Q_MONKEY_EXPORT bool autoEolConversion();
	Q_MONKEY_EXPORT void setDefaultCodec( const QString& codec );
	Q_MONKEY_EXPORT QString defaultCodec();
	Q_MONKEY_EXPORT void setSelectionBackgroundColor( const QColor& background );
	Q_MONKEY_EXPORT QColor selectionBackgroundColor();
	Q_MONKEY_EXPORT void setSelectionForegroundColor( const QColor& foreground );
	Q_MONKEY_EXPORT QColor selectionForegroundColor();
	Q_MONKEY_EXPORT void setDefaultDocumentColours( bool defaultColors );
	Q_MONKEY_EXPORT bool defaultDocumentColours();
	Q_MONKEY_EXPORT void setDefaultDocumentPen( const QColor& defaultPen );
	Q_MONKEY_EXPORT QColor defaultDocumentPen();
	Q_MONKEY_EXPORT void setDefaultDocumentPaper( const QColor& defaultPaper );
	Q_MONKEY_EXPORT QColor defaultDocumentPaper();
	// Auto Completion
	Q_MONKEY_EXPORT void setAutoCompletionCaseSensitivity( bool caseSensitive );
	Q_MONKEY_EXPORT bool autoCompletionCaseSensitivity();
	Q_MONKEY_EXPORT void setAutoCompletionReplaceWord( bool replace );
	Q_MONKEY_EXPORT bool autoCompletionReplaceWord();
	Q_MONKEY_EXPORT void setAutoCompletionShowSingle( bool showSingle );
	Q_MONKEY_EXPORT bool autoCompletionShowSingle();
	Q_MONKEY_EXPORT void setAutoCompletionSource( QsciScintilla::AutoCompletionSource source );
	Q_MONKEY_EXPORT QsciScintilla::AutoCompletionSource autoCompletionSource();
	Q_MONKEY_EXPORT void setAutoCompletionThreshold( int count );
	Q_MONKEY_EXPORT int autoCompletionThreshold();
	// CallTips
	Q_MONKEY_EXPORT void setCallTipsBackgroundColor( const QColor& background );
	Q_MONKEY_EXPORT QColor callTipsBackgroundColor();
	Q_MONKEY_EXPORT void setCallTipsForegroundColor( const QColor& foreground );
	Q_MONKEY_EXPORT QColor callTipsForegroundColor();
	Q_MONKEY_EXPORT void setCallTipsHighlightColor( const QColor& highlight );
	Q_MONKEY_EXPORT QColor callTipsHighlightColor();
	Q_MONKEY_EXPORT void setCallTipsStyle( QsciScintilla::CallTipsStyle style );
	Q_MONKEY_EXPORT QsciScintilla::CallTipsStyle callTipsStyle();
	Q_MONKEY_EXPORT void setCallTipsVisible( int count );
	Q_MONKEY_EXPORT int callTipsVisible();
	// Indentation
	Q_MONKEY_EXPORT void setAutoIndent( bool indent );
	Q_MONKEY_EXPORT bool autoIndent();
	Q_MONKEY_EXPORT void setBackspaceUnindents( bool unindents );
	Q_MONKEY_EXPORT bool backspaceUnindents();
	Q_MONKEY_EXPORT void setIndentationGuides( bool guides );
	Q_MONKEY_EXPORT bool indentationGuides();
	Q_MONKEY_EXPORT void setIndentationsUseTabs( bool tabs );
	Q_MONKEY_EXPORT bool indentationsUseTabs();
	Q_MONKEY_EXPORT void setAutoDetectIndent( bool detect );
	Q_MONKEY_EXPORT bool autoDetectIndent();
	Q_MONKEY_EXPORT void setIndentationWidth( int width );
	Q_MONKEY_EXPORT int indentationWidth();
	Q_MONKEY_EXPORT void setTabIndents( bool indents );
	Q_MONKEY_EXPORT bool tabIndents();
	Q_MONKEY_EXPORT void setTabWidth( int width );
	Q_MONKEY_EXPORT int tabWidth();
	Q_MONKEY_EXPORT void setIndentationGuidesBackgroundColor( const QColor& background );
	Q_MONKEY_EXPORT QColor indentationGuidesBackgroundColor();
	Q_MONKEY_EXPORT void setIndentationGuidesForegroundColor( const QColor& foreground );
	Q_MONKEY_EXPORT QColor indentationGuidesForegroundColor();
	// Brace Matching
	Q_MONKEY_EXPORT void setBraceMatching( QsciScintilla::BraceMatch mode );
	Q_MONKEY_EXPORT QsciScintilla::BraceMatch braceMatching();
	Q_MONKEY_EXPORT void setMatchedBraceBackgroundColor( const QColor& background );
	Q_MONKEY_EXPORT QColor matchedBraceBackgroundColor();
	Q_MONKEY_EXPORT void setMatchedBraceForegroundColor( const QColor& foreground );
	Q_MONKEY_EXPORT QColor matchedBraceForegroundColor();
	Q_MONKEY_EXPORT void setUnmatchedBraceBackgroundColor( const QColor& background );
	Q_MONKEY_EXPORT QColor unmatchedBraceBackgroundColor();
	Q_MONKEY_EXPORT void setUnmatchedBraceForegroundColor( const QColor& foreground );
	Q_MONKEY_EXPORT QColor unmatchedBraceForegroundColor();
	// Edge Mode
	Q_MONKEY_EXPORT void setEdgeMode( QsciScintilla::EdgeMode mode );
	Q_MONKEY_EXPORT QsciScintilla::EdgeMode edgeMode();
	Q_MONKEY_EXPORT void setEdgeColor( const QColor& color );
	Q_MONKEY_EXPORT QColor edgeColor();
	Q_MONKEY_EXPORT void setEdgeColumn( int columns );
	Q_MONKEY_EXPORT int edgeColumn();
	// Caret
	Q_MONKEY_EXPORT void setCaretLineVisible( bool visible );
	Q_MONKEY_EXPORT bool caretLineVisible();
	Q_MONKEY_EXPORT void setCaretLineBackgroundColor( const QColor& background );
	Q_MONKEY_EXPORT QColor caretLineBackgroundColor();
	Q_MONKEY_EXPORT void setCaretForegroundColor( const QColor& foreground );
	Q_MONKEY_EXPORT QColor caretForegroundColor();
	Q_MONKEY_EXPORT void setCaretWidth( int width );
	Q_MONKEY_EXPORT int caretWidth();
	// Margins
	Q_MONKEY_EXPORT void setLineNumbersMarginEnabled( bool enabled );
	Q_MONKEY_EXPORT bool lineNumbersMarginEnabled();
	Q_MONKEY_EXPORT void setLineNumbersMarginWidth( int width );
	Q_MONKEY_EXPORT int lineNumbersMarginWidth();
	Q_MONKEY_EXPORT void setLineNumbersMarginAutoWidth( bool width );
	Q_MONKEY_EXPORT bool lineNumbersMarginAutoWidth();
	Q_MONKEY_EXPORT void setFolding( QsciScintilla::FoldStyle style );
	Q_MONKEY_EXPORT QsciScintilla::FoldStyle folding();
	Q_MONKEY_EXPORT void setFoldMarginBackgroundColor( const QColor& background );
	Q_MONKEY_EXPORT QColor foldMarginBackgroundColor();
	Q_MONKEY_EXPORT void setFoldMarginForegroundColor( const QColor& foreground );
	Q_MONKEY_EXPORT QColor foldMarginForegroundColor();
	Q_MONKEY_EXPORT void setMarginsEnabled( bool enabled );
	Q_MONKEY_EXPORT bool marginsEnabled();
	Q_MONKEY_EXPORT void setMarginsBackgroundColor( const QColor& background );
	Q_MONKEY_EXPORT QColor marginsBackgroundColor();
	Q_MONKEY_EXPORT void setMarginsForegroundColor( const QColor& foreground );
	Q_MONKEY_EXPORT QColor marginsForegroundColor();
	Q_MONKEY_EXPORT void setMarginsFont( const QFont& font );
	Q_MONKEY_EXPORT QFont marginsFont();
	// Special Characters
	Q_MONKEY_EXPORT void setEolMode( QsciScintilla::EolMode mode );
	Q_MONKEY_EXPORT QsciScintilla::EolMode eolMode();
	Q_MONKEY_EXPORT QString getEol( QsciScintilla::EolMode mode = pMonkeyStudio::eolMode() );
	Q_MONKEY_EXPORT void setEolVisibility( bool visible );
	Q_MONKEY_EXPORT bool eolVisibility();
	Q_MONKEY_EXPORT void setAutoDetectEol( bool detect );
	Q_MONKEY_EXPORT bool autoDetectEol();
	Q_MONKEY_EXPORT void setWhitespaceVisibility( QsciScintilla::WhitespaceVisibility visibility );
	Q_MONKEY_EXPORT QsciScintilla::WhitespaceVisibility whitespaceVisibility();
	Q_MONKEY_EXPORT void setWrapMode( QsciScintilla::WrapMode mode );
	Q_MONKEY_EXPORT QsciScintilla::WrapMode wrapMode();
	Q_MONKEY_EXPORT void setWrapVisualFlagsEnabled( bool enabled );
	Q_MONKEY_EXPORT bool wrapVisualFlagsEnabled();
	Q_MONKEY_EXPORT void setStartWrapVisualFlag( QsciScintilla::WrapVisualFlag flag );
	Q_MONKEY_EXPORT QsciScintilla::WrapVisualFlag startWrapVisualFlag();
	Q_MONKEY_EXPORT void setEndWrapVisualFlag( QsciScintilla::WrapVisualFlag flag );
	Q_MONKEY_EXPORT QsciScintilla::WrapVisualFlag endWrapVisualFlag();
	Q_MONKEY_EXPORT void setWrappedLineIndentWidth( int witdh );
	Q_MONKEY_EXPORT int wrappedLineIndentWidth();
};

#endif // PMONKEYSTUDIO_H
