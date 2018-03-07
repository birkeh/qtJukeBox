#include <QSettings>

#include "common.h"

#include "cmainwindow.h"
#include "ui_cmainwindow.h"

#include "cmediainfo.h"

#include "cmusicviewitemdelegate.h"

#include <QStringList>
#include <QScrollBar>

#include <QTime>


void cMainWindow::addFile(const QString& szFile)
{
	if(szFile.isEmpty())
		return;
qDebug() << szFile;

	cMediaInfo*	lpMediaInfo     = new cMediaInfo;
	lpMediaInfo->readFromFile(szFile);

	if(lpMediaInfo->isValid())
	{
		m_lpDB->getDB().transaction();
		lpMediaInfo->writeToDB();
		m_lpDB->getDB().commit();
	}

	delete lpMediaInfo;
}

void cMainWindow::addPath(const QString& szPath)
{
	QDir		Dir(szPath);
	QStringList	Dirs	= Dir.entryList(QDir::Dirs);
	QStringList	Files	= Dir.entryList(QDir::Files);
	int			z;

	if(!Files.isEmpty())
	{
		for(z = 0;z < Files.count();z++)
			addFile(szPath + QString("/") + Files.at(z));
	}

	Dirs.removeAll(".");
	Dirs.removeAll((".."));

	if(!Dirs.isEmpty())
	{
		for(z = 0;z < Dirs.count();z++)
			addPath(szPath + QString("/") + Dirs.at(z));
	}
}

cMainWindow::cMainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::cMainWindow),
	m_lpDB(0),
	m_lpMusicListModel(0),
	m_bProcessing(false)
{
	ui->setupUi(this);

	QSettings	settings;

	if(settings.value("application/version", QVariant(0.0)).toDouble() == 0.0)
		initSettings();

	m_lpDB	= new cDatabase(this);

	m_lpMusicListModel	= new QStandardItemModel(0, 3);
	ui->m_lpMusicList->setModel(m_lpMusicListModel);
//	ui->m_lpMusicList->setItemDelegate(new cMusicViewItemDelegate(ui->m_lpMusicList));

	connect(ui->m_lpMusicList, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onCustomContextMenuRequested(QPoint)));
	connect(ui->m_lpMusicList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onDoubleClicked(QModelIndex)));
	connect(ui->m_lpMusicList, SIGNAL(pressed(QModelIndex)), this, SLOT(onPressed(QModelIndex)));

	loadDB();
	displayDB();

//	addPath("/data/Music");
//	addPath("C:/Users/vet0572/Music");
//	addPath("C:/Temp/Musik");
//	addPath("C:/Users/birkeh/Music");
//	addFile("C:/Users/vet0572/Music/Amy MacDonald/Life In A Beautiful Light/CD1/Amy Macdonald - Life In A Beautiful Light - 18 - Slow It Down (Singalong Instrumental Version).mp3");
}

cMainWindow::~cMainWindow()
{
	delete m_lpDB;
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

void cMainWindow::loadDB()
{
	QSqlQuery	query;
	QString		szOldLeadArtist	= "";
	QString		szOldAlbum		= "";
	QString		szAlbum;
	QString		szTitle;
	QString		szTrackNumber;
	QString		szPartOfSet;
	QString		szLeadArtist;
	QString		szBand;
	QString		szComposer;
	QDate		recordingTime;

	cAlbum*		lpAlbum			= 0;

	query.prepare("SELECT    id, "
				  "          fileName, "
				  "          fileSize, "
				  "          fileDate, "
				  "          fileType, "
				  "          length, "
				  "          bitrate, "
				  "          sampleRate, "
				  "          channels, "
				  "          bitsPerSample, "
				  "          layer, "
				  "          version, "
				  "          sampleWidth, "
				  "          sampleFrames, "
				  "          isEncrypted, "
				  "          trackGain, "
				  "          albumGain, "
				  "          trackPeak, "
				  "          albumPeak, "
				  "          protectionenabled, "
				  "          channelMode, "
				  "          isCopyrighted, "
				  "          isOriginal, "
				  "          album, "
				  "          title, "
				  "          copyright, "
				  "          trackNumber, "
				  "          contentGroupDescription, "
				  "          subTitle, "
				  "          originalAlbum, "
				  "          partOfSet, "
				  "          subTitleOfSet, "
				  "          internationalStandardRecordingCode, "
				  "          leadArtist, "
				  "          band, "
				  "          conductor, "
				  "          interpret, "
				  "          originalArtist, "
				  "          textWriter, "
				  "          originalTextWriter, "
				  "          composer, "
				  "          encodedBy, "
				  "          beatsPerMinute, "
				  "          language, "
				  "          contentType, "
				  "          mediaType, "
				  "          mood, "
				  "          producedNotice, "
				  "          publisher, "
				  "          fileOwner, "
				  "          internetRadioStationName, "
				  "          internetRadioStationOwner, "
				  "          originalFilename, "
				  "          playlistDelay, "
				  "          encodingTime, "
				  "          originalReleaseTime, "
				  "          recordingTime, "
				  "          releaseTime, "
				  "          taggingTime, "
				  "          swhwSettings, "
				  "          albumSortOrder, "
				  "          performerSortOrder, "
				  "          titleSortOrder, "
				  "          synchronizedLyrics, "
				  "          unsynchronizedLyrics "
				  "FROM      file;");
	if(!query.exec())
	{
		myDebug << query.lastError().text();
		return;
	}

	while(query.next())
	{
		szAlbum			= query.value("album").toString();
		szTitle			= query.value("title").toString();
		szTrackNumber	= query.value("trackNumber").toString();
		szPartOfSet		= query.value("partOfSet").toString();
		szLeadArtist	= query.value("leadArtist").toString();
		szBand			= query.value("band").toString();
		szComposer		= query.value("composer").toString();
		recordingTime	= query.value("recordingTime").toDate();

		lpAlbum			= m_albumList.add(szAlbum, szBand);
		if(lpAlbum)
			lpAlbum->addTrack(szTitle, szTrackNumber, szPartOfSet, szBand, szComposer, recordingTime);
	}
}

void cMainWindow::displayDB()
{
	m_lpMusicListModel->clear();

	m_albumList.sort();

	QStringList	header;
	header << "Original";

	m_lpMusicListModel->setHorizontalHeaderLabels(header);

	QString					szOldBand		= "OldBand";
	QString					szOldAlbum		= "OldAlbum";
	QList<QStandardItem*>	lpBandItem;
	QList<QStandardItem*>	lpAlbumItem;

	for(int x = 0;x < m_albumList.count();x++)
	{
		cAlbum*					lpAlbum	= m_albumList.at(x);

		if(szOldBand != lpAlbum->band())
		{

			szOldBand	= lpAlbum->band();
			szOldAlbum		= "";

			lpBandItem.clear();
			lpBandItem.append(new QStandardItem(szOldBand));

			m_lpMusicListModel->appendRow(lpBandItem);
		}

		if(szOldAlbum != lpAlbum->album())
		{
			szOldAlbum		= lpAlbum->album();

			lpAlbumItem.clear();
			lpAlbumItem.append(new QStandardItem(szOldAlbum));
			lpAlbumItem.at(0)->setData(QVariant::fromValue(lpAlbum), Qt::UserRole+MUSICLIST_ORI_ALBUM);
			lpAlbumItem.append(new QStandardItem(szOldAlbum));

			lpBandItem.at(0)->appendRow(lpAlbumItem);
		}

		cTrackList	trackList	= lpAlbum->trackList();
		for(int y = 0;y < trackList.count();y++)
		{
			cTrack*	lpTrack	= trackList.at(y);

			QList<QStandardItem*>	lpTrackItem;
			lpTrackItem.append(new QStandardItem(lpTrack->title()));
			lpAlbumItem.at(0)->appendRow(lpTrackItem);
		}
	}
}

void cMainWindow::onCustomContextMenuRequested(const QPoint &/*pos*/)
{
	/*
		QMenu*	lpMenu	= new QMenu(this);

		lpMenu->addAction("add", this, SLOT(onActionAdd()));

		lpMenu->addAction("update all", this, SLOT(onActionUpdateAll()));
		lpMenu->addAction("update unfinished", this, SLOT(onActionUpdateUnfinished()));
		lpMenu->addSeparator();

		if(lpTreeView->selectionModel()->selectedRows().count() == 1)
		{
			cSerie*	lpSerie	= m_lpSeriesListModel->itemFromIndex(lpTreeView->selectionModel()->selectedRows().at(0))->data(Qt::UserRole).value<cSerie*>();
			if(lpSerie)
			{
				lpMenu->addAction("update", this, SLOT(onActionUpdate()));
				lpMenu->addAction("delete", this, SLOT(onActionDelete()));
				lpMenu->addAction("edit", this, SLOT(onActionEdit()));
				lpMenu->addSeparator();

				if(!lpSerie->IMDBID().isEmpty())
					lpMenu->addAction("open IMDB", this, SLOT(onActionGotoIMDB()));

				if(!lpSerie->download().isEmpty())
				{
					lpMenu->addAction("open download link", this, SLOT(onActionGotoDownload()));
					lpMenu->addAction("copy download link", this, SLOT(onActionCopyDownload()));
				}
				lpMenu->addAction("open all download links", this, SLOT(onActionGotoAllDownload()));
				lpMenu->addAction("open all download links (open)", this, SLOT(onActionGotoAllDownloadOpen()));
				lpMenu->addSeparator();
				lpMenu->addAction("load images", this, SLOT(onActionLoadPictures()));
			}
		}
		else if(lpTreeView->selectionModel()->selectedRows().count())
		{
			lpMenu->addAction("update selected", this, SLOT(onActionUpdate()));
			lpMenu->addAction("delete selected", this, SLOT(onActionDelete()));
			lpMenu->addSeparator();
			lpMenu->addAction("load images", this, SLOT(onActionLoadPictures()));
		}

		lpMenu->addSeparator();
		lpMenu->addAction("export...", this, SLOT(onActionExport()));

		lpMenu->popup(lpTreeView->viewport()->mapToGlobal(pos));
	*/
}

void cMainWindow::onDoubleClicked(const QModelIndex &/*index*/)
{
}

void cMainWindow::onPressed(const QModelIndex &/*index*/)
{
	switch(QGuiApplication::mouseButtons())
	{
/*
	case Qt::MiddleButton:
		if(ui->m_lpSeriesList1->selectionModel()->selectedRows().count() == 1)
		{
			cSerie*	lpSerie	= m_lpSeriesListModel->itemFromIndex(ui->m_lpSeriesList1->selectionModel()->selectedRows().at(0))->data(Qt::UserRole).value<cSerie*>();
			if(lpSerie)
			{
				if(!lpSerie->download().isEmpty())
					onActionGotoDownload();
			}
		}
		break;
*/
	default:
		break;
	}
}
