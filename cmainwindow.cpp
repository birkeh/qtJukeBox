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
//	if(lpMediaInfo->isValid())
//		lpMediaInfo->writeToDB();
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

	bool bRet;
	bRet = lpDatabase->getDB().transaction();
//	addPath("C:/Users/vet0572/Music");
//	addPath("C:/Users/vet0572/Music/Bee Gees");
	addPath("C:/Users/vet0572/Music/Ambros, Tauchen, Prokopetz");
	addPath("C:/Users/vet0572/Music/Amy MacDonald");
	addPath("C:/Users/vet0572/Music/Amy Winehouse");
	addPath("C:/Users/vet0572/Music/Andreas Bourani");
	addPath("C:/Users/vet0572/Music/Angus & Julia Stone");
	addPath("C:/Users/vet0572/Music/Anna Maria Kaufmann and Joey Tempest");

	addPath("C:/Users/vet0572/Music/Bee Gees/2 Years On");
	addPath("C:/Users/vet0572/Music/Bee Gees/Bee Gees Story");
	addPath("C:/Users/vet0572/Music/Bee Gees/Best of Bee Gees");
	addPath("C:/Users/vet0572/Music/Bee Gees/Brillant Front Birth");
	addPath("C:/Users/vet0572/Music/Bee Gees/Children of the World");
	addPath("C:/Users/vet0572/Music/Bee Gees/Cucumber Castle");
	addPath("C:/Users/vet0572/Music/Bee Gees/E.S.P");
	addPath("C:/Users/vet0572/Music/Bee Gees/First");
	addPath("C:/Users/vet0572/Music/Bee Gees/Greatest");
	addPath("C:/Users/vet0572/Music/Bee Gees/Here at Last...Bee Gees...Live");
	addPath("C:/Users/vet0572/Music/Bee Gees/High Civilization");
	addPath("C:/Users/vet0572/Music/Bee Gees/Horizontal");
	addPath("C:/Users/vet0572/Music/Bee Gees/Idea");
	addPath("C:/Users/vet0572/Music/Bee Gees/In the beginning");
	addPath("C:/Users/vet0572/Music/Bee Gees/Inception And Nostalgia");
	addPath("C:/Users/vet0572/Music/Bee Gees/Instrumental & Other Rarities");
	addPath("C:/Users/vet0572/Music/Bee Gees/Kick In The Head");
	addPath("C:/Users/vet0572/Music/Bee Gees/Life In A Tin Can");
	addPath("C:/Users/vet0572/Music/Bee Gees/Live At The Beeb");
	addPath("C:/Users/vet0572/Music/Bee Gees/Living Eyes");
	addPath("C:/Users/vet0572/Music/Bee Gees/Love Songs");
	addPath("C:/Users/vet0572/Music/Bee Gees/Main Course");
	addPath("C:/Users/vet0572/Music/Bee Gees/Melody");
	addPath("C:/Users/vet0572/Music/Bee Gees/Mr. Natural");
	addPath("C:/Users/vet0572/Music/Bee Gees/Number Ones");
	addPath("C:/Users/vet0572/Music/Bee Gees/Odessa");
	addPath("C:/Users/vet0572/Music/Bee Gees/One");
	addPath("C:/Users/vet0572/Music/Bee Gees/One Night Only");
	addPath("C:/Users/vet0572/Music/Bee Gees/Rare Precious And Beautiful");
	addPath("C:/Users/vet0572/Music/Bee Gees/Rare precious and beautiful - Vol 3");
	addPath("C:/Users/vet0572/Music/Bee Gees/Rare, Precious & Beautiful 2");
	addPath("C:/Users/vet0572/Music/Bee Gees/Saturday night fever");
	addPath("C:/Users/vet0572/Music/Bee Gees/Sayonara Tokyo");
	addPath("C:/Users/vet0572/Music/Bee Gees/Sing And Play 14 Barry Gibb Songs");
	addPath("C:/Users/vet0572/Music/Bee Gees/Size Isn't Everything");
	addPath("C:/Users/vet0572/Music/Bee Gees/Spicks and Specks");
	addPath("C:/Users/vet0572/Music/Bee Gees/Spirits Having Flown");
	addPath("C:/Users/vet0572/Music/Bee Gees/Staying Alive");
	addPath("C:/Users/vet0572/Music/Bee Gees/Still Waters");
	addPath("C:/Users/vet0572/Music/Bee Gees/The best of Volume 02");
	addPath("C:/Users/vet0572/Music/Bee Gees/The Very Best");
	addPath("C:/Users/vet0572/Music/Bee Gees/This is where I came in");
	addPath("C:/Users/vet0572/Music/Bee Gees/To Whom It May Concern");
	addPath("C:/Users/vet0572/Music/Bee Gees/Tomorrow The World");
	addPath("C:/Users/vet0572/Music/Bee Gees/Trafalgar");
	addPath("C:/Users/vet0572/Music/Bee Gees/Turn Around, Look At Me");
	addPath("C:/Users/vet0572/Music/Bee Gees/Turn around, look at us");

	addPath("C:/Users/vet0572/Music/Ambros, Tauchen, Prokopetz");
	addPath("C:/Users/vet0572/Music/Amy MacDonald");
	addPath("C:/Users/vet0572/Music/Amy Winehouse");
	addPath("C:/Users/vet0572/Music/Andreas Bourani");
	addPath("C:/Users/vet0572/Music/Angus & Julia Stone");
	addPath("C:/Users/vet0572/Music/Anna Maria Kaufmann and Joey Tempest");
	addPath("C:/Users/vet0572/Music/Boney M");
	addPath("C:/Users/vet0572/Music/Delta Moon");
	addPath("C:/Users/vet0572/Music/Dire Straits");
	addPath("C:/Users/vet0572/Music/Erste Allgemeine Verunsicherung");
	addPath("C:/Users/vet0572/Music/Flogging Molly");
	addPath("C:/Users/vet0572/Music/Frank Zander");
	addPath("C:/Users/vet0572/Music/Game Of Thrones");
	addPath("C:/Users/vet0572/Music/Game Of Thrones- Season 4");
	addPath("C:/Users/vet0572/Music/Heartbeat");
	addPath("C:/Users/vet0572/Music/Hubert von Goisern");
	addPath("C:/Users/vet0572/Music/Hugh Laurie");
	addPath("C:/Users/vet0572/Music/Jon Lord");
	addPath("C:/Users/vet0572/Music/Karthago");
	addPath("C:/Users/vet0572/Music/Legend of The Seeker");
	addPath("C:/Users/vet0572/Music/Ludwig Hirsch");
	addPath("C:/Users/vet0572/Music/Mass Effect");
	addPath("C:/Users/vet0572/Music/Matt Simons");
	addPath("C:/Users/vet0572/Music/Mike Krüger - 79er Motzbeutel");
	addPath("C:/Users/vet0572/Music/Mike Oldfield");
	addPath("C:/Users/vet0572/Music/Ostbahn Kurti & Die Chefpartie");
	addPath("C:/Users/vet0572/Music/Papermoon");
	addPath("C:/Users/vet0572/Music/Passenger");
	addPath("C:/Users/vet0572/Music/Ramin Djawadi");
	addPath("C:/Users/vet0572/Music/Roger Hodgson");
	addPath("C:/Users/vet0572/Music/Shadow Of The Beast");
	addPath("C:/Users/vet0572/Music/Simon & Garfunkel");
	addPath("C:/Users/vet0572/Music/Stars On 45");
	addPath("C:/Users/vet0572/Music/STS");
	addPath("C:/Users/vet0572/Music/The Piano Guys");
	addPath("C:/Users/vet0572/Music/The Pogues");
	addPath("C:/Users/vet0572/Music/The Stand");
	addPath("C:/Users/vet0572/Music/Tracy Chapman");
	addPath("C:/Users/vet0572/Music/Vienna Symphony Orchestra");
	addPath("C:/Users/vet0572/Music/Wig");
	addPath("C:/Users/vet0572/Music/Wolfgang Ambros");
	bRet = lpDatabase->getDB().commit();

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
