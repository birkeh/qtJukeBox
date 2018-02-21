#include <QSettings>

#include "cmainwindow.h"
#include "ui_cmainwindow.h"

#include "cmediainfo.h"

#include <QTime>


void cMainWindow::addFile(const QString& szFile)
{
	if(szFile.isEmpty())
		return;

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
	ui(new Ui::cMainWindow)
{
	QSettings	settings;

	if(settings.value("application/version", QVariant(0.0)).toDouble() == 0.0)
		initSettings();

	m_lpDB	= new cDatabase(this);

	QTime t;


//	bool bRet;
//	bRet = lpDatabase->getDB().transaction();
	t.start();
	addPath("C:/Users/vet0572/Music");
	qDebug() << t.elapsed();
//	addPath("C:/Users/birkeh/Music");
//	bRet = lpDatabase->getDB().commit();

//	addFile("C:/Users/vet0572/Music/Amy MacDonald/Under Stars (Deluxe)/01 - Dream On.mp3");

	ui->setupUi(this);
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
