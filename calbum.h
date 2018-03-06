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
	cAlbum(const QString& szAlbum, const QString& szLeadArtist);

	void			setAlbum(const QString& szAlbum);
	QString			album();

	void			setLeadArtist(const QString& szLeadArtist);
	QString			leadArtist();

	cTrack*			addTrack(const QString& szTitle, const qint16& trackNumber, const QString& szPartOfSet, const QString& szBand, const QString& szComposer, const QDate& recordingTime);
	cTrackList		trackList();
private:
	QString			m_szAlbum;
	QString			m_szLeadArtist;
	cTrackList		m_trackList;
};

Q_DECLARE_METATYPE(cAlbum*)

class cAlbumList : public QList<cAlbum*>
{
public:
	cAlbum*			add(const QString& szAlbum, const QString &szLeadArtist);
};


#endif // CALBUM_H
