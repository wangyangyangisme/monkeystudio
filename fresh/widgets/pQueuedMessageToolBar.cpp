#include "pQueuedMessageToolBar.h"

#include <QAction>

int pQueuedMessageToolBar::mDefaultTimeout = 5000;
QPixmap* pQueuedMessageToolBar::mDefaultPixmap = 0;
QBrush pQueuedMessageToolBar::mDefaultBackground = QBrush( QColor( 250, 230, 147 ) );
QBrush pQueuedMessageToolBar::mDefaultForeground = QBrush();

/*!
	\details Create a new pQueuedMessageToolBar object
	\param parent The parent widget
*/
pQueuedMessageToolBar::pQueuedMessageToolBar( QWidget* parent )
	: QToolBar( parent )
{
	setMovable( false );
	setFloatable( false );
	setAllowedAreas( Qt::TopToolBarArea );
	toggleViewAction()->setEnabled( false );
	toggleViewAction()->setVisible( false );
	mDefaultPalette = palette();
	// create pQueuedMessageWidget
	mQueuedWidget = new pQueuedMessageWidget( this );
	addWidget( mQueuedWidget );
	// connections
	connect( mQueuedWidget, SIGNAL( messageShown( const pQueuedMessage& ) ), this, SLOT( messageShown( const pQueuedMessage& ) ) );
	connect( mQueuedWidget, SIGNAL( cleared() ), this, SLOT( messageCleared() ) );
}

pQueuedMessageToolBar::~pQueuedMessageToolBar()
{
}

void pQueuedMessageToolBar::setDefaultTimeout( int timeout )
{
	mDefaultTimeout = timeout;
}

const int& pQueuedMessageToolBar::defaultTimeout()
{
	return mDefaultTimeout;
}

void pQueuedMessageToolBar::setDefaultPixmap( const QPixmap& pixmap )
{
	if ( !mDefaultPixmap )
	{
		mDefaultPixmap = new QPixmap();
	}
	
	*mDefaultPixmap = pixmap;
}

const QPixmap& pQueuedMessageToolBar::defaultPixmap()
{
	if ( !mDefaultPixmap )
	{
		mDefaultPixmap = new QPixmap();
	}
	
	return *mDefaultPixmap;
}

void pQueuedMessageToolBar::setDefaultBackground( const QBrush& brush )
{
	mDefaultBackground = brush;
}

const QBrush& pQueuedMessageToolBar::defaultBackground()
{
	return mDefaultBackground;
}

void pQueuedMessageToolBar::setDefaultForeground( const QBrush& brush )
{
	mDefaultForeground = brush;
}

const QBrush& pQueuedMessageToolBar::defaultForeground()
{
	return mDefaultForeground;
}

/*!
	\details Append a new message and return it's id
	\param message The message structure to show
*/
int pQueuedMessageToolBar::appendMessage( const pQueuedMessage& message )
{
	return mQueuedWidget->append( message );
}

/*!
	\details Append a new message and return it's id
	\param message The message to show
	\param milliseconds The timeout before the message is auto closed, 0 for no timeout
	\param pixmap The pixmap to associate with the message
	\param background The background of the message
	\param foreground The foreground of the message
*/
int pQueuedMessageToolBar::appendMessage( const QString& message, int milliseconds, const QPixmap& pixmap, const QBrush& background, const QBrush& foreground )
{
	return mQueuedWidget->append( message, milliseconds, pixmap, background, foreground );
}

/*!
	\details Remove a message from the queued list
	\param message The message structure to remove
*/
void pQueuedMessageToolBar::removeMessage( const pQueuedMessage& message )
{
	mQueuedWidget->remove( message );
}

/*!
	\details Remove a message from the queued list
	\param id The message id to remove
*/
void pQueuedMessageToolBar::removeMessage( int id )
{
	mQueuedWidget->remove( id );
}

void pQueuedMessageToolBar::messageShown( const pQueuedMessage& message )
{
	Q_UNUSED( message );
	
	setAutoFillBackground( true );
	QPalette pal = mDefaultPalette;
	pal.setBrush( backgroundRole(), message.Background );
	setPalette( pal );
	
	if ( !isVisible() )
	{
		setVisible( true );
	}
}

void pQueuedMessageToolBar::messageCleared()
{
	setAutoFillBackground( false );
	setPalette( mDefaultPalette );
	
	if ( isVisible() )
	{
		setVisible( false );
	}
}
