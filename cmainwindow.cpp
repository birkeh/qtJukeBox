#include <QSettings>

#include "cmainwindow.h"
#include "ui_cmainwindow.h"

#include "cdatabase.h"
#include "cmediainfo.h"


void addFile(const QString& szFile)
{
	if(szFile.isEmpty())
		return;

	cMediaInfo*	lpMediaInfo     = new cMediaInfo;
	lpMediaInfo->readFromFile(szFile);
	if(lpMediaInfo->isValid())
		lpMediaInfo->writeToDB();
	delete lpMediaInfo;
}

void addPath(const QString& szPath)
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

	cDatabase*	lpDatabase;
	lpDatabase	= new cDatabase(this);

//	bool bRet;
//	bRet = lpDatabase->getDB().transaction();
//	addPath("C:/Users/vet0572/Music");
	addPath("C:/Users/birkeh/Music");
//	bRet = lpDatabase->getDB().commit();

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
