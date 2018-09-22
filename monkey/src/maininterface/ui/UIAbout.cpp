/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : UIAbout.cpp
** Date      : 2008-01-14T00:36:54
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
#include "UIAbout.h"
#include "main.h"

#include <pIconManager.h>

#include <QFile>
#include <QDesktopServices>
#include <QTabBar>
#include <QDate>

const QString mInformations =
"<table width=\"100%\" cellpadding=\"0\" cellspacing=\"0\" border=\"0\" style=\"font-size:medium;\">"
"	<tr>"
"		<td align=\"center\"><br /><img src=\":/application/icons/application/monkey2.png\" width=\"32\" height=\"32\"></td>"
"		<td>%2 v%3 (%4)<br />%5<br />The Monkey Studio Team<br /><a href=\"http://%6\">Home page</a></td>"
"	</tr>"
"</table>";

const QString mDatasMask =
"<table width=\"100%\" cellpadding=\"0\" cellspacing=\"0\" border=\"0\" style=\"font-size:medium;\">"
"	<tr>"
"		<td>"
"			<b>%1</b><br />"
"			%2 %3"
"			%4 (<a href=\"%5\">%6</a>)<br />"
"		</td>"
"	</tr>"
"</table>";

const QList<pDatas> mTeamates = QList<pDatas>()
	<< pDatas( "Filipe Azevedo", "Nox P@sNox", QObject::tr( "France" ), "pasnox@gmail.com", QObject::tr( "Creator & Principal Developer" ) )
	<< pDatas( "Kopats Andrei", "hlamer", QObject::tr( "Belarus" ), "hlamer@tut.by", QObject::tr( "Principal Developer, Class Browser, Beloruss translator" ) )
	<< pDatas( "Yannick", "xiantia", QObject::tr( "France" ), "xiantia@gmail.com", QObject::tr( "GNU Debugger Plugin" ) )
	<< pDatas( "Roper Alexander", "Minirop", QObject::tr( "France" ), "minirop@peyj.com", QObject::tr( "Qt Pro Parser, Some Features/Help" ) )
	<< pDatas( "Mashin Evgeniy", "evgenM", QObject::tr( "Russia" ), "mashin.zhenya@gmail.com", QObject::tr( "Many shared code between our IDEs" ) )
	<< pDatas( "Manuel Schmidt", "oversize", QObject::tr( "Germany" ), "manuel@schmidtman.de", QObject::tr( "Web Developer & Web Designer" ) )
	<< pDatas( "Julien Decologne", "Judd", QObject::tr( "France" ), "judd@hotmail.com", QObject::tr( "Splashscreen & Icons Designer" ) )
	<< pDatas( "Plano Marc", "Marc31", QObject::tr( "France" ), "marc31boss@gmail.com", QObject::tr( "French Translator" ) )
	<< pDatas( "Lukic Djordje", "rumpl", QObject::tr( "Serbia" ), "rumplstiltzkin@gmail.com", QObject::tr( "SDK Script Generator" ) )
	<< pDatas( QString::fromUtf8( "Aurélien MICHON" ), "aurelien", QObject::tr( "France" ), "aurelien.french@gmail.com", QObject::tr( "Winter Splashscreen Designer" ) );

const QList<pDatas> mDonors = QList<pDatas>()
	<< pDatas( "Filipe Azevedo", "Nox P@sNox", QObject::tr( "France" ), "pasnox@gmail.com", QObject::tr( "No donations for now, you can use this <a href=\"http://sourceforge.net/project/project_donations.php?group_id=163493\">link</a> to make donation. Donations will help paying host/domain, and relatives things about the project." ) );

const QList<pDatas> mLinks = QList<pDatas>()
	<< pDatas( "", "", QObject::tr( "Location" ), "http://qt.nokia.com", QObject::tr( "Nokia's Qt homepage." ) )
	<< pDatas( "", "", QObject::tr( "Location" ), "http://webissues.mimec.org", QObject::tr( "Team collaboration across the Internet." ) )
	<< pDatas( "", "", QObject::tr( "Location" ), "http://webissues.monkeystudio.org", QObject::tr( "Our webissues server (login: anonymous, password: anonymous)." ) )
	<< pDatas( "", "", QObject::tr( "Location" ), "https://launchpad.net/monkeystudio", QObject::tr( "You can send bug/patch/ideas/what ever you want to our bug tracker." ) )
	<< pDatas( "", "", QObject::tr( "Location" ), "http://monkeystudio.org/forum", QObject::tr( "Forums are at your disposition." ) )
	<< pDatas( "", "", QObject::tr( "Location" ), "http://monkeystudio.org/rss.xml", QObject::tr( "Get the latest news using the rss feed." ) )
	<< pDatas( "", "", QObject::tr( "Location" ), "http://monkeystudio.org/team", QObject::tr( "Meet the Monkeys (Team's members)." ) );

UIAbout::UIAbout( QWidget* p )
	: QDialog( p )
{
	setupUi( this );
	setAttribute( Qt::WA_DeleteOnClose );

	// window title
	setWindowTitle( windowTitle().arg( PACKAGE_NAME ) );

	// mouse cursor
	twAbout->findChild<QTabBar*>()->setCursor( Qt::PointingHandCursor );

	// change label color
	QPalette lp( lInformations->palette() );
	lp.setColor( lInformations->backgroundRole(), Qt::white );
	lInformations->setPalette( lp );

	// show informations table
	lInformations->setTextInteractionFlags( Qt::TextBrowserInteraction | Qt::TextSelectableByKeyboard );
	lInformations->setText( mInformations.arg( PACKAGE_NAME ).arg( PACKAGE_VERSION ).arg( PACKAGE_VERSION_STR ).arg( PACKAGE_COPYRIGHTS ).arg( PACKAGE_DOMAIN ) );

	// logo
	bool isXMas = false;
	
	switch ( QDate::currentDate().month() )
	{
		case 11:
		case 12:
		case 1:
			isXMas = true;
			break;
	}

	if ( isXMas )
	{
		lLogo->setPixmap( pIconManager::pixmap( "splashscreen_christmas.png", ":/application" ) );
	}

	// team
	foreach ( pDatas i, mTeamates )
		tbTeam->append( mDatasMask.arg( i.Comment ).arg( i.Name +" -" ).arg( i.Login +"<br />" ).arg( i.Pays ).arg( QString( "mailto:" ).append( i.Email ) ).arg( i.Email ) );
	tbTeam->moveCursor( QTextCursor::Start );

	// license
	QFile file( ":/licenses/texts/license.gpl" );
	file.open( QFile::ReadOnly );
	tbLicense->setPlainText( QString::fromUtf8( file.readAll() ) );
	file.close();
	tbLicense->moveCursor( QTextCursor::Start );

	// donors
	foreach ( pDatas i, mDonors )
		tbDonations->append( mDatasMask.arg( i.Comment ).arg( i.Name +" -" ).arg( i.Login +"<br />" ).arg( i.Pays ).arg( QString( "mailto:" ).append( i.Email ) ).arg( i.Email ) );
	tbDonations->moveCursor( QTextCursor::Start );

	// links
	foreach ( pDatas i, mLinks )
		tbLinks->append( mDatasMask.arg( i.Comment ).arg( i.Name ).arg( i.Login ).arg( i.Pays ).arg( i.Email ).arg( i.Email ) );
	tbLinks->moveCursor( QTextCursor::Start );

	// connections
	foreach ( QTextBrowser* b, twAbout->findChildren<QTextBrowser*>() )
		connect( b, SIGNAL( anchorClicked( const QUrl& ) ), this, SLOT( anchorClicked( const QUrl& ) ) );
}

void UIAbout::anchorClicked( const QUrl& u )
{
	QTextBrowser* b = qobject_cast<QTextBrowser*>( sender() );
	if ( !b )
		return;
	b->setHtml( b->toHtml() );
	QDesktopServices::openUrl( u );
}
