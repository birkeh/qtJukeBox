#include <QSettings>

#include "common.h"

#include "cmainwindow.h"
#include "ui_cmainwindow.h"

#include "cmediainfo.h"

#include "cmusicviewitemdelegate.h"

#include <QStringList>
#include <QScrollBar>

#include <QTime>
#include <QDir>

#include <QSqlQuery>
#include <QSqlError>


void cMainWindow::addFile(const QString& szFile)
{
	if(szFile.isEmpty())
		return;

	cMediaInfo*	lpMediaInfo     = new cMediaInfo;
	lpMediaInfo->importFromFile(szFile);
/*
	if(lpMediaInfo->isValid())
	{
		m_lpDB->getDB().transaction();
		lpMediaInfo->writeToDB();
		m_lpDB->getDB().commit();
	}
*/
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
	m_lpMusicListModel(0),
	m_bProcessing(false)
{
	ui->setupUi(this);

	QSettings	settings;

	if(settings.value("application/version", QVariant(0.0)).toDouble() == 0.0)
		initSettings();

	m_lpMusicListModel	= new QStandardItemModel(0, 3);
	ui->m_lpMusicListOriginal->setModel(m_lpMusicListModel);
	ui->m_lpMusicListOriginal->setItemDelegate(new cMusicViewItemDelegate(ui->m_lpMusicListOriginal));
	ui->m_lpMusicListNew->setModel(m_lpMusicListModel);
	ui->m_lpMusicListNew->setItemDelegate(new cMusicViewItemDelegate(ui->m_lpMusicListNew));

	connect(ui->m_lpMusicListOriginal, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onCustomContextMenuRequestedOriginal(QPoint)));
	connect(ui->m_lpMusicListNew, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onCustomContextMenuRequestedNew(QPoint)));

	connect(ui->m_lpMusicListOriginal, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onDoubleClickedOriginal(QModelIndex)));
	connect(ui->m_lpMusicListNew, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onDoubleClickedNew(QModelIndex)));

	connect(ui->m_lpMusicListOriginal, SIGNAL(pressed(QModelIndex)), this, SLOT(onPressedOriginal(QModelIndex)));
	connect(ui->m_lpMusicListNew, SIGNAL(pressed(QModelIndex)), this, SLOT(onPressedNew(QModelIndex)));

	connect(ui->m_lpMusicListOriginal, SIGNAL(expanded(QModelIndex)), this, SLOT(onExpandedOriginal(QModelIndex)));
	connect(ui->m_lpMusicListNew, SIGNAL(expanded(QModelIndex)), this, SLOT(onExpandedNew(QModelIndex)));

	connect(ui->m_lpMusicListOriginal, SIGNAL(collapsed(QModelIndex)), this, SLOT(onCollapsedOriginal(QModelIndex)));
	connect(ui->m_lpMusicListNew, SIGNAL(collapsed(QModelIndex)), this, SLOT(onCollapsedNew(QModelIndex)));

	connect(ui->m_lpMusicListOriginal->selectionModel(), &QItemSelectionModel::selectionChanged, this, &cMainWindow::onSelectionChangedOriginal);
	connect(ui->m_lpMusicListNew->selectionModel(), &QItemSelectionModel::selectionChanged, this, &cMainWindow::onSelectionChangedNew);

	connect(ui->m_lpMusicListOriginal->verticalScrollBar(), &QScrollBar::valueChanged, this, &cMainWindow::onScrollbarValueChangedOriginal);
	connect(ui->m_lpMusicListNew->verticalScrollBar(), &QScrollBar::valueChanged, this, &cMainWindow::onScrollbarValueChangedNew);

	initDB();
	loadDB();
	displayDB();

//	QTime	time;
//	time.start();
//	addPath("C:/Users/birkeh/Music");
//	addPath("C:/Users/vet0572/Music");
//	qDebug() << time.elapsed();
//	addPath("C:/Users/vet0572/Music");
//	addPath("C:/Users/birkeh/Music");
	addFile("C:/Users/vet0572/Music/Amy MacDonald/Under Stars (Deluxe)/01 - Dream On.mp3");
	addFile("C:/Users/vet0572/Music/Amy MacDonald/Under Stars (Deluxe)/01 - Dream On.mp3");
}

cMainWindow::~cMainWindow()
{
	if(m_DB.isOpen())
		m_DB.close();

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

bool cMainWindow::initDB()
{
	QDir		d;
	QString		szDB;
	QSettings	settings;

	m_DB	= QSqlDatabase::addDatabase("QSQLITE");
	szDB	= settings.value("application/data").toString()+QDir::separator()+settings.value("database/database").toString();
	m_DB.setDatabaseName(szDB);
	if(!m_DB.open())
	{
		myDebug << m_DB.lastError().text();
		return(false);
	}

	QSqlQuery	query(m_DB);

	if(!m_DB.tables().contains("version"))
	{
		if(!query.exec("CREATE TABLE version\n"
					   "(version integer)"))
		{
			myDebug << query.lastError().text();
			return(false);
		}

		if(!query.exec("INSERT INTO version (version) VALUES (1)"))
		{
			myDebug << query.lastError().text();
			return(false);
		}
	}

	if(!m_DB.tables().contains("file"))
	{
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
			return(false);
		}
	}

	if(!m_DB.tables().contains("image"))
	{
		if(!query.exec("CREATE TABLE image ("
						"   id          INTEGER PRIMARY KEY,"
						"   fileID      INTEGER REFERENCES file(id),"
						"   fileName    STRING,"
						"   imageType   INTEGER,"
						"   description STRING,"
						"   image       BLOB);"))
		{
			myDebug << query.lastError().text();
			return(false);
		}
	}

	QSqlQuery	version("SELECT version FROM version", m_DB);
	if(!version.exec())
		return(false);
	else
	{
		version.first();
		m_iVersion = version.value("version").toInt();
	}
	return(true);
}

void cMainWindow::loadDB()
{
	QSqlQuery	query;
	QString		szOldLeadArtist	= "";
	QString		szOldAlbum		= "";
	QString		szAlbum;
	QString		szTitle;
	qint16		iTrackNumber;
	qint16		iPartOfSet;
	QString		szLeadArtist;
	QString		szBand;
	QString		szComposer;
	QDate		recordingTime;

	cAlbum*		lpAlbum			= 0;

	query.prepare("SELECT leadArtist, album, title, trackNumber, partOfSet, band, composer, recordingTime FROM file ORDER BY leadArtist, album, trackNumber;");
	if(!query.exec())
	{
		myDebug << query.lastError().text();
		return;
	}

	while(query.next())
	{
		szAlbum			= query.value("album").toString();
		szTitle			= query.value("title").toString();
		iTrackNumber	= query.value("trackNumber").toInt();
		iPartOfSet		= query.value("partOfSet").toInt();
		szLeadArtist	= query.value("leadArtist").toString();
		szBand			= query.value("band").toString();
		szComposer		= query.value("composer").toString();
		recordingTime	= query.value("recordingTime").toDate();

		if(szOldLeadArtist != szLeadArtist || szOldAlbum != szAlbum)
		{
			szOldLeadArtist	= szLeadArtist;
			szOldAlbum		= szAlbum;

			cAlbum*	lpAlbum	= m_albumList.add(szAlbum, szLeadArtist);
		}
	}
}

void cMainWindow::displayDB()
{
	m_lpMusicListModel->clear();

	QStringList	header;
	header << "Original" << "New";

	m_lpMusicListModel->setHorizontalHeaderLabels(header);

	ui->m_lpMusicListOriginal->setColumnHidden(1, true);
	ui->m_lpMusicListNew->setColumnHidden(0, true);

	QString					szOldLeadArtist		= "";
	QString					szOldAlbum			= "";
	QList<QStandardItem*>	lpLeadArtistItem;
	QList<QStandardItem*>	lpAlbumItem;

	for(int x = 0;x < m_albumList.count();x++)
	{
		cAlbum*					lpAlbum	= m_albumList.at(x);

		if(szOldLeadArtist != lpAlbum->leadArtist())
		{

			szOldLeadArtist	= lpAlbum->leadArtist();
			szOldAlbum		= "";

			lpLeadArtistItem.clear();
			lpLeadArtistItem.append(new QStandardItem(szOldLeadArtist));
			lpLeadArtistItem.append(new QStandardItem(szOldLeadArtist));

			m_lpMusicListModel->appendRow(lpLeadArtistItem);
		}

		if(szOldAlbum != lpAlbum->album())
		{
			szOldAlbum		= lpAlbum->album();

			lpAlbumItem.clear();
			lpAlbumItem.append(new QStandardItem(szOldAlbum));
			lpAlbumItem.at(0)->setData(QVariant::fromValue(lpAlbum), Qt::UserRole+MUSICLIST_ORI_ALBUM);
			lpAlbumItem.append(new QStandardItem(szOldAlbum));

			lpLeadArtistItem.at(0)->appendRow(lpAlbumItem);
		}

		QList<QStandardItem*>	lpTrackItem;
		lpTrackItem.append(new QStandardItem("track 1"));
		lpTrackItem.append(new QStandardItem("track 1"));
		lpAlbumItem.at(0)->appendRow(lpTrackItem);
	}
}

void cMainWindow::onCustomContextMenuRequestedOriginal(const QPoint &pos)
{
	onCustomContextMenuRequested(ui->m_lpMusicListOriginal, pos);
}

void cMainWindow::onDoubleClickedOriginal(const QModelIndex &index)
{
	onDoubleClicked(index);
}

void cMainWindow::onPressedOriginal(const QModelIndex &index)
{
	onPressed(index);
}

void cMainWindow::onExpandedOriginal(const QModelIndex &index)
{
	ui->m_lpMusicListNew->expand(index);
}

void cMainWindow::onCollapsedOriginal(const QModelIndex &index)
{
	ui->m_lpMusicListNew->collapse(index);
}

void cMainWindow::onSelectionChangedOriginal(const QItemSelection &selected, const QItemSelection &deselected)
{
	if(m_bProcessing)
		return;

	m_bProcessing	= true;

	QItemSelectionModel*	lpOriginal	= ui->m_lpMusicListOriginal->selectionModel();
	QItemSelectionModel*	lpNew		= ui->m_lpMusicListNew->selectionModel();
	QModelIndexList			selected1	= lpOriginal->selectedIndexes();

	lpNew->clearSelection();

	for(int z = 0;z < selected1.count();z++)
	{

		QStandardItem*	lpItem	= m_lpMusicListModel->itemFromIndex(selected1.at(z));
		QModelIndex		index	= lpItem->index();
		lpNew->select(index, QItemSelectionModel::Select);
	}

	m_bProcessing	= false;
}

void cMainWindow::onScrollbarValueChangedOriginal(int value)
{
	if(m_bProcessing)
		return;

	m_bProcessing	= true;

	ui->m_lpMusicListNew->verticalScrollBar()->setValue(value);

	m_bProcessing	= false;
}

void cMainWindow::onCustomContextMenuRequestedNew(const QPoint &pos)
{
	onCustomContextMenuRequested(ui->m_lpMusicListNew, pos);
}

void cMainWindow::onDoubleClickedNew(const QModelIndex &index)
{
	onDoubleClicked(index);
}

void cMainWindow::onPressedNew(const QModelIndex &index)
{
	onPressed(index);
}

void cMainWindow::onExpandedNew(const QModelIndex &index)
{
	ui->m_lpMusicListOriginal->expand(index);
}

void cMainWindow::onCollapsedNew(const QModelIndex &index)
{
	ui->m_lpMusicListOriginal->collapse(index);
}

void cMainWindow::onSelectionChangedNew(const QItemSelection &selected, const QItemSelection &deselected)
{
	if(m_bProcessing)
		return;

	m_bProcessing	= true;

	QItemSelectionModel*	lpOriginal	= ui->m_lpMusicListOriginal->selectionModel();
	QItemSelectionModel*	lpNew		= ui->m_lpMusicListNew->selectionModel();
	QModelIndexList			selected2	= lpNew->selectedIndexes();

	lpOriginal->clearSelection();

	for(int z = 0;z < selected2.count();z++)
	{
		QStandardItem*	lpItem	= m_lpMusicListModel->itemFromIndex(selected2.at(z));
		QModelIndex		index	= lpItem->index();
		lpOriginal->select(index, QItemSelectionModel::Select);
	}

	m_bProcessing	= false;
}

void cMainWindow::onScrollbarValueChangedNew(int value)
{
	if(m_bProcessing)
		return;

	m_bProcessing	= true;

	ui->m_lpMusicListOriginal->verticalScrollBar()->setValue(value);

	m_bProcessing	= false;
}

void cMainWindow::onCustomContextMenuRequested(const QTreeView* lpTreeView, const QPoint &pos)
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

void cMainWindow::onDoubleClicked(const QModelIndex &index)
{
}

void cMainWindow::onPressed(const QModelIndex &index)
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
