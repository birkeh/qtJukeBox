#ifndef CTRACK_H
#define CTRACK_H


#include <QMetaType>
#include <QString>
#include <QDate>
#include <QList>


class cAlbum;

class cTrack
{
public:
	cTrack(cAlbum* lpAlbum, const QString& szTitle, const qint16& trackNumber, const QString& szPartOfSet, const QString& szBand, const QString& szComposer, const QDate& recordingTime);

	void			setAlbum(cAlbum* lpAlbum);
	cAlbum*			album();

	void			setTitle(const QString& szTitle);
	QString			title();

	void			setTrackNumber(const qint16& trackNumber);
	qint16			trackNumber();

	void			setPartOfSet(const QString& szPartOfSet);
	QString			partOfSet();

	void			setBand(const QString& szBand);
	QString			band();

	void			setComposer(const QString& szComposer);
	QString			composer();

	void			setRecordingTime(const QDate &recordingTime);
	QDate			recordingTime();

private:
	cAlbum*			m_lpAlbum;
	QString			m_szTitle;
	qint16			m_iTrackNumber;
	QString			m_szPartOfSet;
	QString			m_szBand;
	QString			m_szComposer;
	QDate			m_recordingTime;
};

Q_DECLARE_METATYPE(cTrack*)

class cTrackList : public QList<cTrack*>
{
public:
	cTrack*			add(cAlbum* lpAlbum, const QString& szTitle, const qint16& trackNumber, const QString& szPartOfSet, const QString& szBand, const QString& szComposer, const QDate& recordingTime);
};

#endif // CTRACK_H
