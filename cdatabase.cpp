#include "cdatabase.h"
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
		return(0);
	if(!query.exec("INSERT INTO version (version) VALUES (1)"))
		return(0);

	if(!query.exec("CREATE TABLE file\n"
				   "(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,\n"
				   " filename TEXT NOT NULL,\n"
				   " filetype INTEGER DEFAULT (0),\n"
				   " length INTEGER DEFAULT (-1),\n"
				   " bitrate INTEGER DEFAULT (-1),\n"
				   " samplerate INTEGER DEFAULT (-1),\n"
				   " channels INTEGER DEFAULT (-1),\n"
				   " bitspersample INTEGER DEFAULT (-1),\n"
				   " layer INTEGER DEFAULT (-1),\n"
				   " version INTEGER DEFAULT (-1),\n"
				   " samplewidth INTEGER DEFAULT (-1),\n"
				   " sampleframes INTEGER DEFAULT (0),\n"
				   " isencrypted INTEGER DEFAULT (0),\n"
				   " trackgain INTEGER DEFAULT (-1),\n"
				   " albumgain INTEGER DEFAULT (-1),\n"
				   " trackpeak INTEGER DEFAULT (-1),\n"
				   " albumpeak INTEGER DEFAULT (-1),\n"
				   " protectionenabled INTEGER DEFAULT (0),\n"
				   " channelmode INTEGER DEFAULT (0),\n"
				   " iscopyrighted INTEGER DEFAULT (0),\n"
				   " isoriginal INTEGER DEFAULT (0),\n"
				   " album TEXT,\n"
				   " comment TEXT,\n"
				   " title TEXT\n,"
				   " rating TEXT,\n"
				   " copyright TEXT,\n"
				   " track TEXT,\n"
				   " year INTEGER)"))
		return(0);

	if(!query.exec("CREATE TABLE id3v1artist\n"
				   "(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,\n"
				   " fileid INTEGER NOT NULL,\n"
				   " person TEXT NOT NULL)"))
		return(0);

	if(!query.exec("CREATE TABLE id3v1genre\n"
				   "(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,\n"
				   " fileid INTEGER NOT NULL,\n"
				   " genre TEXT NOT NULL)"))
		return(0);

	if(!query.exec("CREATE TABLE id3v2\n"
				   "(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,\n"
				   " fileid INTEGER NOT NULL,\n"
				   " version INTEGER DEFAULT (-1),\n"
				   " revision INTEGER DEFAULT (-1),\n"
				   " size INTEGER DEFAULT (-1))"))
		return(0);

	if(!query.exec("CREATE TABLE id3v2tag\n"
				   "(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,\n"
				   " id3v2id INTEGER NOT NULL,\n"
				   " tag TEXT NOT NULL,\n"
				   " value TEXT)"))
		return(0);

	if(!query.exec("CREATE TABLE ape\n"
				   "(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,\n"
				   " fileid INTEGER NOT NULL\n)"))
		return(0);

	if(!query.exec("CREATE TABLE apetag\n"
				   "(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,\n"
				   " apeid INTEGER NOT NULL,\n"
				   " tag TEXT NOT NULL,\n"
				   " value TEXT)"))
		return(0);

	if(!query.exec("CREATE TABLE properties\n"
				   "(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,\n"
				   " fileid INTEGER NOT NULL)"))
		return(0);

	if(!query.exec("CREATE TABLE propertiestag\n"
				   "(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,\n"
				   " propertiesid INTEGER NOT NULL,\n"
				   " tag TEXT NOT NULL,\n"
				   " value TEXT)"))
		return(0);

	return(1);
}
