#include "common.h"

#include "calbum.h"
#include "ctrack.h"


class cAlbumCompare
{
public:
	cAlbumCompare(qint32 sort) : m_sort(sort) {}

	bool operator()(cAlbum* lpFirst, cAlbum* lpSecond)
	{
		if(m_sort & SORT_BAND_ASC)
		{
			if(lpFirst->band() < lpSecond->band())
				return(true);
			else if(lpFirst->band() > lpSecond->band())
				return(false);
			else
			{
				if(m_sort & SORT_ALBUM_ASC)
				{
					if(lpFirst->album() < lpSecond->album())
						return(true);
					else
						return(false);
				}
				else if(m_sort & SORT_ALBUM_DESC)
				{
					if(lpFirst->album() > lpSecond->album())
						return(true);
					else
						return(false);
				}
				else
					return(false);
			}
		}
		else if(m_sort & SORT_BAND_DESC)
		{
			if(lpFirst->band() > lpSecond->band())
				return(true);
			else if(lpFirst->band() < lpSecond->band())
				return(false);
			else
			{
				if(m_sort & SORT_ALBUM_ASC)
				{
					if(lpFirst->album() < lpSecond->album())
						return(true);
					else
						return(false);
				}
				else if(m_sort & SORT_ALBUM_DESC)
				{
					if(lpFirst->album() > lpSecond->album())
						return(true);
					else
						return(false);
				}
				else
					return(false);
			}
		}
		else
		{
			if(m_sort & SORT_ALBUM_ASC)
			{
				if(lpFirst->album() < lpSecond->album())
					return(true);
				else
					return(false);
			}
			else if(m_sort & SORT_ALBUM_DESC)
			{
				if(lpFirst->album() > lpSecond->album())
					return(true);
				else
					return(false);
			}
			else
				return(false);
		}
	}

private:
	qint32		m_sort;
};

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

cTrack* cAlbum::addTrack(const QString& szTitle, const QString &szTrackNumber, const QString& szPartOfSet, const QString& szBand, const QString& szLeadArtist, const QString& szComposer, const QDate &recordingTime, const QString& szFileName)
{
	return(m_trackList.add(this, szTitle, szTrackNumber, szPartOfSet, szBand, szLeadArtist, szComposer, recordingTime, szFileName));
}

cTrackList cAlbum::trackList()
{
	return(m_trackList);
}

void cAlbum::sortTracks(qint32 sort)
{
	m_trackList.sort(sort);
}

cAlbum* cAlbumList::add(const QString& szAlbum, const QString& szBand)
{
	QString	szAlbum1	= szAlbum;
	QString	szBand1		= szBand;

	if(szAlbum1.isEmpty())
		szAlbum1	= "*** NO ALBUM ***";
	if(szBand1.isEmpty())
		szBand1		= "*** NO BAND ***";

	for(int x = 0;x < count();x++)
	{
		if(at(x)->album() == szAlbum1 && at(x)->band() == szBand1)
			return(at(x));
	}

	cAlbum*	lpAlbumNew	= new cAlbum(szAlbum1, szBand1);
	append(lpAlbumNew);
	return(lpAlbumNew);
}

void cAlbumList::sort(qint32 sort)
{
	std::sort(begin(), end(), cAlbumCompare(sort));

	if(sort & SORT_TRACK_ASC || sort & SORT_TRACK_DESC)
	{
		for(int x = 0;x < count();x++)
			at(x)->sortTracks(sort);
	}
}
