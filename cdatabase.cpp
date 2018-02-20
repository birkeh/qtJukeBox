#include "cdatabase.h"
#include "common.h"

#include <QDir>
#include <QSettings>


cDatabase::cDatabase(QObject *parent) :
	QObject(parent), m_iVersion(-1)
{
	init();
}

cDatabase::~cDatabase()
{
	if(m_DB.isOpen())
		m_DB.close();
}

QSqlDatabase cDatabase::getDB()
{
	return(m_DB);
}

void cDatabase::init()
{
	QDir		d;
	QString		szDB;
	QSettings	settings;

	m_DB	= QSqlDatabase::addDatabase("QSQLITE");
	szDB	= settings.value("application/data").toString()+QDir::separator()+settings.value("database/database").toString();
	m_DB.setDatabaseName(szDB);
	m_DB.open();

	QSqlQuery	version("select version from version", m_DB);
	if(!version.exec())
		m_iVersion	= createDB();
	else
	{
		version.first();
		m_iVersion = version.value(0).toInt();
	}
}

int cDatabase::createDB()
{
	QSqlQuery	query(m_DB);

	if(!query.exec("CREATE TABLE version\n"
				   "(version integer)"))
	{
		myDebug << query.lastError().text();
		return(0);
	}

	if(!query.exec("INSERT INTO version (version) VALUES (1)"))
	{
		myDebug << query.lastError().text();
		return(0);
	}

	if(!query.exec("CREATE TABLE file "
				   "(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "
				   " fileName TEXT, "
				   " fileSize INTEGER, "
				   " fileDate DATE, "
				   " fileType1 INTEGER, "
				   " fileType TEXT, "
				   " length1 INTEGER, "
				   " length INTEGER, "
				   " bitrate INTEGER, "
				   " sampleRate INTEGER, "
				   " channels INTEGER, "
				   " bitsPerSample INTEGER, "
				   " layer INTEGER, "
				   " version INTEGER, "
				   " sampleWidth INTEGER, "
				   " sampleFrames INTEGER, "
				   " isEncrypted BOOL, "
				   " trackGain INTEGER, "
				   " albumGain INTEGER, "
				   " trackPeak INTEGER, "
				   " albumPeak INTEGER, "
				   " protectionEnabled BOOL, "
				   " channelMode INTEGER, "
				   " isCopyrighted BOOL, "
				   " isOriginal BOOL, "
				   " album TEXT, "
				   " title TEXT, "
				   " copyright TEXT, "
				   " tracknumber TEXT, "
				   " contentGroupDescription TEXT, "
				   " subTitle TEXT, "
				   " originalAlbum TEXT, "
				   " partOfSet TEXT, "
				   " subTitleOfSet TEXT, "
				   " internationalStandardRecordingCode TEXT, "
				   " leadArtist TEXT, "
				   " band TEXT, "
				   " conductor TEXT, "
				   " interpret TEXT, "
				   " originalArtist TEXT, "
				   " textWriter TEXT, "
				   " originalTextWriter TEXT, "
				   " composer TEXT, "
				   " encodedBy TEXT, "
				   " beatsPerMinute INTEGER, "
				   " language TEXT, "
				   " contentType TEXT, "
				   " mediaType TEXT, "
				   " mood TEXT, "
				   " producedNotice TEXT, "
				   " publisher TEXT, "
				   " fileOwner TEXT, "
				   " internetRadioStationName TEXT, "
				   " internetRadioStationOwner TEXT, "
				   " originalFilename TEXT, "
				   " playlistDelay TEXT, "
				   " encodingTime INTEGER, "
				   " originalReleaseTime DATE, "
				   " recordingTime DATE, "
				   " releaseTime DATE, "
				   " taggingTime DATE, "
				   " swhwSettings TEXT, "
				   " albumSortOrder TEXT, "
				   " performerSortOrder TEXT, "
				   " titleSortOrder TEXT, "
				   " synchronizedLyrics TEXT, "
				   " unsynchronizedLyrics TEXT)"))
	{
		myDebug << query.lastError().text();
		return(0);
	}

	return(1);
}
