#include <QSettings>

#include "cmainwindow.h"
#include "ui_cmainwindow.h"

#include "cdatabase.h"
#include "cmediainfo.h"


void addFile(const QString& szFile, const QSqlDatabase db)
{
	if(szFile.isEmpty())
		return;
	cMediaInfo*	lpMediaInfo     = new cMediaInfo;
	lpMediaInfo->readFromFile(szFile);
	if(lpMediaInfo->isValid())
		lpMediaInfo->writeToDB(db);
	delete lpMediaInfo;
}

void addPath(const QString& szPath, const QSqlDatabase db)
{
	QDir		Dir(szPath);
	QStringList	Dirs	= Dir.entryList(QDir::Dirs);
	QStringList	Files	= Dir.entryList(QDir::Files);
	int			z;

	if(!Files.isEmpty())
	{
		for(z = 0;z < Files.count();z++)
			addFile(szPath + QString("/") + Files.at(z), db);
	}

	Dirs.removeAll(".");
	Dirs.removeAll((".."));

	if(!Dirs.isEmpty())
	{
		for(z = 0;z < Dirs.count();z++)
			addPath(szPath + QString("/") + Dirs.at(z), db);
	}
}

cMainWindow::cMainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::cMainWindow)
{
	QSettings	settings;

	if(settings.value("application/version", QVariant(0.0)).toDouble() == 0.0)
		initSettings();

	cDatabase*	lpDatabase;
	lpDatabase	= new cDatabase(this);
/*
	bool bRet;
	bRet = lpDatabase->getDB().transaction();
	addPath("C:\\Users\\birkeh\\Music", lpDatabase->getDB());
	bRet = lpDatabase->getDB().commit();
*/

	cMediaInfo*	lpMediaInfo;
	lpMediaInfo	= new cMediaInfo(this);
//	lpMediaInfo->readFromFile("C:\\Users\\birkeh\\Music\\Wig\\A Day In The Life\\CD1\\Wig - A Day In The Life - Part 1 - Wake Up.mp3");
//	lpMediaInfo->readFromFile("C:\\Users\\birkeh\\Music\\Amy MacDonald\\Life In A Beautiful Light\\CD1\\Amy Macdonald - Life In A Beautiful Light - 05 - The Game.mp3");
//	lpMediaInfo->readFromFile("C:\\Users\\birkeh\\Music\\Amy MacDonald\\A Curious Thing\\CD1\\Amy MacDonald - A Curious Thing - 01 - Don't tell me it's over.mp3");
//	lpMediaInfo->readFromFile("C:\\Users\\birkeh\\Music\\id3v1\\id3v1_140_genre_W.mp3");
//	lpMediaInfo->readFromFile("C:\\Users\\vet0572\\Music\\Heartbeat\\Strong\\CD1\\Heartbeat - Strong - 01 - Strong.mp3");
	lpMediaInfo->readFromFile("C:\\Users\\vet0572\\Music\\Amy MacDonald\\Under Stars (Deluxe)\\02 - Under Stars.mp3");
	QString			sz;
	QStringList		szL;
	qint16			i;
	qint32			i32;
	qint64			i64;
	QDateTime		dt;
	bool			b;
	cImageList		images;
	cStringTimeList	szTL;
	cMediaInfo::MEDIA_TYPE		mt;
	cMediaInfo::CHANNEL_MODE	cm;

	sz		= lpMediaInfo->fileName();
	mt		= lpMediaInfo->fileType1();
	sz		= lpMediaInfo->fileType1Text();

	i32		= lpMediaInfo->length1();
	sz		= lpMediaInfo->length1Text();
	i		= lpMediaInfo->bitrate();
	sz		= lpMediaInfo->bitrateText();
	i32		= lpMediaInfo->sampleRate();
	sz		= lpMediaInfo->sampleRateText();
	i		= lpMediaInfo->channels();
	sz		= lpMediaInfo->channelsText();
	i		= lpMediaInfo->bitsPerSample();
	sz		= lpMediaInfo->bitsPerSampleText();
	i		= lpMediaInfo->layer();
	sz		= lpMediaInfo->layerText();
	i		= lpMediaInfo->version();
	sz		= lpMediaInfo->versionText();
	i		= lpMediaInfo->sampleWidth();
	sz		= lpMediaInfo->sampleWidthText();
	i64		= lpMediaInfo->sampleFrames();
	sz		= lpMediaInfo->sampleFramesText();
	b		= lpMediaInfo->isEncrypted();
	i		= lpMediaInfo->trackGain();
	i		= lpMediaInfo->albumGain();
	i		= lpMediaInfo->trackPeak();
	i		= lpMediaInfo->albumPeak();
	b		= lpMediaInfo->protectionEnabled();
	cm		= lpMediaInfo->channelMode();
	sz		= lpMediaInfo->channelModeText();
	b		= lpMediaInfo->isCopyrighted();
	b		= lpMediaInfo->isOriginal();

	sz		= lpMediaInfo->contentGroupDescription();
	sz		= lpMediaInfo->title();
	sz		= lpMediaInfo->subTitle();
	sz		= lpMediaInfo->album();
	sz		= lpMediaInfo->originalAlbum();
	sz		= lpMediaInfo->trackNumber();
	sz		= lpMediaInfo->partOfSet();
	sz		= lpMediaInfo->subTitleOfSet();
	sz		= lpMediaInfo->internationalStandardRecordingCode();

	sz		= lpMediaInfo->leadArtist();
	sz		= lpMediaInfo->band();
	sz		= lpMediaInfo->conductor();
	szL		= lpMediaInfo->interpret();
	sz		= lpMediaInfo->originalArtist();
	sz		= lpMediaInfo->textWriter();
	sz		= lpMediaInfo->originalTextWriter();
	sz		= lpMediaInfo->composer();
	sz		= lpMediaInfo->encodedBy();

	i		= lpMediaInfo->beatsPerMinute();
	i		= lpMediaInfo->length();
	szL		= lpMediaInfo->language();
	szL		= lpMediaInfo->contentType();
	sz		= lpMediaInfo->fileType();
	szL		= lpMediaInfo->mediaType();
	sz		= lpMediaInfo->mood();

	sz		= lpMediaInfo->copyright();
	sz		= lpMediaInfo->producedNotice();
	sz		= lpMediaInfo->publisher();
	sz		= lpMediaInfo->fileOwner();
	sz		= lpMediaInfo->internetRadioStationName();
	sz		= lpMediaInfo->internetRadioStationOwner();

	sz		= lpMediaInfo->originalFilename();
	i		= lpMediaInfo->playlistDelay();
	dt		= lpMediaInfo->encodingTime();
	dt		= lpMediaInfo->originalReleaseTime();
	dt		= lpMediaInfo->recordingTime();
	dt		= lpMediaInfo->releaseTime();
	dt		= lpMediaInfo->taggingTime();
	szL		= lpMediaInfo->swhwSettings();
	sz		= lpMediaInfo->albumSortOrder();
	sz		= lpMediaInfo->performerSortOrder();
	sz		= lpMediaInfo->titleSortOrder();

	szL		= lpMediaInfo->unsynchronizedLyrics();
	szTL	= lpMediaInfo->synchronizedLyrics();

	images	= lpMediaInfo->images();

//	lpMediaInfo->writeToDB(lpDatabase->getDB());
	delete lpMediaInfo;

/*
	lpMediaInfo	= new cMediaInfo(this);
	lpMediaInfo->readFromFile("/data/Musik/MP3/Wig/First/CD1/Wig - First - 01 - Arie.flac");
	lpMediaInfo->writeToDB(lpDatabase->getDB());
	delete lpMediaInfo;

	lpMediaInfo	= new cMediaInfo(this);
	lpMediaInfo->readFromFile("/data/Musik/MP3/Wig/First/CD1/Wig - First - 01 - Arie.m4a");
	lpMediaInfo->writeToDB(lpDatabase->getDB());
	delete lpMediaInfo;

	lpMediaInfo	= new cMediaInfo(this);
	lpMediaInfo->readFromFile("/data/Musik/MP3/Wig/First/CD1/Wig - First - 01 - Arie.mp3");
	lpMediaInfo->writeToDB(lpDatabase->getDB());
	delete lpMediaInfo;

	lpMediaInfo	= new cMediaInfo(this);
	lpMediaInfo->readFromFile("/data/Musik/MP3/Wig/First/CD1/sh3.ape");
	lpMediaInfo->writeToDB(lpDatabase->getDB());
	delete lpMediaInfo;
*/
	ui->setupUi(this);

	delete (lpDatabase);
}

cMainWindow::~cMainWindow()
{
	delete ui;
}

void cMainWindow::initSettings()
{
	QSettings	settings;
	QDir		dir;

	settings.setValue("application/version", QVariant(0.1));
	settings.setValue("application/data", QDir::homePath()+QDir::separator()+".qtjukebox");
	settings.setValue("database/type", "SQLITE");
	settings.setValue("database/database", "qtjukebox.db");

	dir.mkdir(settings.value("application/data").toString());
}
