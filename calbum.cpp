#include "calbum.h"


cAlbum::cAlbum(const QString& szAlbum, const QString &szLeadArtist) :
	m_szAlbum(szAlbum),
	m_szLeadArtist(szLeadArtist)
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

void cAlbum::setLeadArtist(const QString& szLeadArtist)
{
	m_szLeadArtist	= szLeadArtist;
}

QString cAlbum::leadArtist()
{
	return(m_szLeadArtist);
}

cAlbum* cAlbumList::add(const QString& szAlbum, const QString& szLeadArtist)
{
	for(int x = 0;x < count();x++)
	{
		if(at(x)->album() == szAlbum && at(x)->leadArtist() == szLeadArtist)
			return(at(x));
	}

	cAlbum*	lpAlbumNew	= new cAlbum(szAlbum, szLeadArtist);
	append(lpAlbumNew);
	return(lpAlbumNew);
}
