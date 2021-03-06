#ifndef CALBUM_H
#define CALBUM_H


#include "ctrack.h"

#include <QMetaType>
#include <QString>
#include <QList>


class cTrack;
class cTrackList;

class cAlbum
{
public:
	cAlbum(const QString& szAlbum, const QString& szBand);

	void			setAlbum(const QString& szAlbum);
	QString			album();

	void			setBand(const QString& szBand);
	QString			band();

	cTrack*			addTrack(const QString& szTitle, const QString& szTrackNumber, const QString& szPartOfSet, const QString& szBand, const QString &szLeadArtist, const QString& szComposer, const QDate& recordingTime, const QString &szFileName);
	cTrackList		trackList();

	void			sortTracks(qint32 sort);
private:
	QString			m_szAlbum;
	QString			m_szBand;
	cTrackList		m_trackList;
};

Q_DECLARE_METATYPE(cAlbum*)

class cAlbumList : public QList<cAlbum*>
{
public:
	cAlbum*			add(const QString& szAlbum, const QString &szBand);
	void			sort(qint32 sort);
private:
};


#endif // CALBUM_H
