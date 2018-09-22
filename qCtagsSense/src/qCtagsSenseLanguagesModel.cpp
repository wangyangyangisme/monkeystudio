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
#include "qCtagsSenseLanguagesModel.h"
#include "qCtagsSenseSQL.h"

#include <QThread>
#include <QMutex>
#include <QSqlRecord>
#include <QDebug>

class QCTAGSSENSE_EXPORT qCtagsSenseLanguagesThread : public QThread
{
	Q_OBJECT
	
public:
	qCtagsSenseLanguagesThread( qCtagsSenseSQL* parent )
		: QThread( parent )
	{
		mSQL = parent;
		mStop = false;
		mRestart = false;
		mLanguages = 0;
	}
	
	virtual ~qCtagsSenseLanguagesThread()
	{
		QMutexLocker locker( &mMutex );
		mStop = true;
		locker.unlock();
		wait();
	}

public slots:
	void executeQuery( const QString& sql, QStringList* languages )
	{
		{
			QMutexLocker locker( &mMutex );
			mStop = false;
			mRestart = isRunning();
			mQuery = sql;
			mLanguages = languages;
		}
		
		if ( !isRunning() )
		{
			start();
		}
	}

protected:
	QMutex mMutex;
	qCtagsSenseSQL* mSQL;
	bool mStop;
	bool mRestart;
	QString mQuery;
	QStringList* mLanguages;
	
	virtual void run()
	{
		{
			QMutexLocker locker( &mMutex );
			mRestart = false;
			mStop = false;
		}
		
		forever
		{
			delete mLanguages;
			mLanguages = 0;
			
			QSqlQuery q = mSQL->query();
			q.setForwardOnly( true );
			
			if ( !q.exec( mQuery ) )
			{
				qWarning() << "Can't get languages";
				return;
			}
			
			QStringList languages;
			
			while ( q.next() )
			{
				const QString language = q.record().value( 0 ).toString();
				languages << language;
				
				{
					QMutexLocker locker( &mMutex );
					
					if ( mStop )
					{
						return;
					}
					else if ( mRestart )
					{
						break;
					}
				}
			}
			
			{
				QMutexLocker locker( &mMutex );
				
				if ( mRestart )
				{
					mRestart = false;
					continue;
				}
			}
			
			qSort( languages );
			
			emit queryFinished( languages );
			break;
		}
	}

signals:
	void queryFinished( const QStringList& languages );
};

qCtagsSenseLanguagesModel::qCtagsSenseLanguagesModel( qCtagsSenseSQL* parent )
	: QAbstractItemModel( parent )
{
	mSQL = parent;
	mLanguages = 0;
	mThread = new qCtagsSenseLanguagesThread( mSQL );
	
	connect( mThread, SIGNAL( queryFinished( const QStringList& ) ), this, SLOT( queryFinished( const QStringList& ) ) );
}

qCtagsSenseLanguagesModel::~qCtagsSenseLanguagesModel()
{
	delete mLanguages;
}

int qCtagsSenseLanguagesModel::columnCount( const QModelIndex& parent ) const
{
	Q_UNUSED( parent );
	return 1;
}

QVariant qCtagsSenseLanguagesModel::data( const QModelIndex& index, int role ) const
{
	if ( index.isValid() )
	{
		if ( role == Qt::DisplayRole )
		{
			return mLanguages->at( index.internalId() );
		}
		else if ( role == Qt::ToolTipRole )
		{
			return mLanguages->at( index.internalId() );
		}
	}
	
	return QVariant();
}

QModelIndex qCtagsSenseLanguagesModel::index( int row, int column, const QModelIndex& parent ) const
{
	if ( !parent.isValid() )
	{
		if ( row >= 0 && row < rowCount( parent ) && column >= 0 && column < columnCount( parent ) )
		{
			return createIndex( row, column, row );
		}
	}
	
	return QModelIndex();
}

QModelIndex qCtagsSenseLanguagesModel::parent( const QModelIndex& index ) const
{
	Q_UNUSED( index );
	return QModelIndex();
}

int qCtagsSenseLanguagesModel::rowCount( const QModelIndex& parent ) const
{
	return parent.isValid() || !mLanguages ? 0 : mLanguages->count();
}

bool qCtagsSenseLanguagesModel::hasChildren( const QModelIndex& parent ) const
{
	return parent.isValid() || !mLanguages ? false : !mLanguages->isEmpty();
}

QString qCtagsSenseLanguagesModel::language( int id ) const
{
	return mLanguages ? mLanguages->value( id ) : QString::null;
}

int qCtagsSenseLanguagesModel::indexOf( const QString& language ) const
{
	return mLanguages ? mLanguages->indexOf( language ) : -1;
}

bool qCtagsSenseLanguagesModel::isRunning() const
{
	return mThread->isRunning();
}

void qCtagsSenseLanguagesModel::refresh()
{
	const QString sql = QString(
		"SELECT DISTINCT( language ) FROM files"
	);
	
	QStringList* languages = mLanguages;
	mLanguages = 0;
	
	reset();
	mThread->executeQuery( sql, languages );
}

void qCtagsSenseLanguagesModel::queryFinished( const QStringList& languages )
{
	mLanguages = new QStringList( languages );
	
	reset();
	emit ready();
}

#include "qCtagsSenseLanguagesModel.moc"
