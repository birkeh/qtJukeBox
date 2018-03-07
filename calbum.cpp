#include "calbum.h"
#include "ctrack.h"


cAlbum::cAlbum(const QString& szAlbum, const QString &szBand) :
	m_szAlbum(szAlbum),
	m_szBand(szBand)
{
}

void cAlbum::setAlbum(const QString& szAlbum)
{
	m_szAlbum	= szAlbum;
}

QString cAlbum::album()
{
	return(m_szAlbum);
}

void cAlbum::setBand(const QString& szBand)
{
	m_szBand	= szBand;
}

QString cAlbum::band()
{
	return(m_szBand);
}

cTrack* cAlbum::addTrack(const QString& szTitle, const QString &szTrackNumber, const QString& szPartOfSet, const QString& szBand, const QString& szComposer, const QDate &recordingTime)
{
	return(m_trackList.add(this, szTitle, szTrackNumber, szPartOfSet, szBand, szComposer, recordingTime));
}

cTrackList cAlbum::trackList()
{
	return(m_trackList);
}

cAlbum* cAlbumList::add(const QString& szAlbum, const QString& szBand)
{
	QString	szAlbum1		= szAlbum;
	QString	szLeadArtist1	= szLeadArtist;

	if(szAlbum1.isEmpty())
		szAlbum1		= "*** NO ALBUM ***";
	if(szLeadArtist1.isEmpty())
		szLeadArtist1	= "*** NO LEAD ARTIST ***";

	for(int x = 0;x < count();x++)
	{
		if(at(x)->album() == szAlbum1 && at(x)->leadArtist() == szLeadArtist1)
			return(at(x));
	}

	cAlbum*	lpAlbumNew	= new cAlbum(szAlbum1, szLeadArtist1);
	append(lpAlbumNew);
	return(lpAlbumNew);
}
