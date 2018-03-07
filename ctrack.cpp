#include "ctrack.h"


cTrack::cTrack(cAlbum* lpAlbum, const QString& szTitle, const QString &szTrackNumber, const QString& szPartOfSet, const QString& szBand, const QString& szComposer, const QDate& recordingTime) :
	m_lpAlbum(lpAlbum),
	m_szTitle(szTitle),
	m_szTrackNumber(szTrackNumber),
	m_szPartOfSet(szPartOfSet),
	m_szBand(szBand),
	m_szComposer(szComposer),
	m_recordingTime(recordingTime)
{
}

void cTrack::setAlbum(cAlbum* lpAlbum)
{
	m_lpAlbum	= lpAlbum;
}

cAlbum* cTrack::album()
{
	return(m_lpAlbum);
}

void cTrack::setTitle(const QString& szTitle)
{
	m_szTitle	= szTitle;
}

QString cTrack::title()
{
	return(m_szTitle);
}

void cTrack::setTrackNumber(const QString &szTrackNumber)
{
	m_szTrackNumber	= szTrackNumber;
}

QString cTrack::trackNumber()
{
	return(m_szTrackNumber);
}

void cTrack::setPartOfSet(const QString& szPartOfSet)
{
	m_szPartOfSet	= szPartOfSet;
}

QString cTrack::partOfSet()
{
	return(m_szPartOfSet);
}

void cTrack::setBand(const QString& szBand)
{
	m_szBand	= szBand;
}

QString cTrack::band()
{
	return(m_szBand);
}

void cTrack::setComposer(const QString& szComposer)
{
	m_szComposer	= szComposer;
}

QString cTrack::composer()
{
	return(m_szComposer);
}

void cTrack::setRecordingTime(const QDate& recordingTime)
{
	m_recordingTime	= recordingTime;
}

QDate cTrack::recordingTime()
{
	return(m_recordingTime);
}

cTrack*  cTrackList::add(cAlbum* lpAlbum, const QString& szTitle, const QString &szTrackNumber, const QString& szPartOfSet, const QString& szBand, const QString& szComposer, const QDate &recordingTime)
{
	QString	szTitle1		= szTitle;
	QString	szBand1			= szBand;
	QString	szComposer1		= szComposer;

	if(szTitle1.isEmpty())
		szTitle1		= "*** NO TITLE ***";
	if(szBand1.isEmpty())
		szBand1	= "*** NO BAND ***";
	if(szComposer1.isEmpty())
		szComposer1	= "*** NO COMPOSER ***";

	cTrack*	lpTrackNew	= new cTrack(lpAlbum, szTitle1, szTrackNumber, szPartOfSet, szBand1, szComposer1, recordingTime);
	append(lpTrackNew);
	return(lpTrackNew);
}
