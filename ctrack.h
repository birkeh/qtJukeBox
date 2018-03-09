#ifndef CTRACK_H
#define CTRACK_H


#include <QMetaType>
#include <QString>
#include <QDate>
#include <QList>


class cAlbum;

/*
					   " fileName TEXT, "
					   " fileSize INTEGER, "
					   " fileDate DATE, "
					   " fileType TEXT, "
					   " length INTEGER, "
					   " bitrate INTEGER, "
					   " sampleRate INTEGER, "
					   " channels INTEGER, "
					   " bitsPerSample INTEGER, "
					   " layer INTEGER, "
					   " version INTEGER, "
					   " sampleWidth INTEGER, "
					   " sampleFrames INTEGER, "
					   " isEncrypted BOOL, "
					   " trackGain INTEGER, "
					   " albumGain INTEGER, "
					   " trackPeak INTEGER, "
					   " albumPeak INTEGER, "
					   " protectionEnabled BOOL, "
					   " channelMode INTEGER, "
					   " isCopyrighted BOOL, "
					   " isOriginal BOOL, "
					   " copyright TEXT, "
					   " contentGroupDescription TEXT, "
					   " subTitle TEXT, "
					   " originalAlbum TEXT, "
					   " subTitleOfSet TEXT, "
					   " internationalStandardRecordingCode TEXT, "
					   " leadArtist TEXT, "
					   " conductor TEXT, "
					   " interpret TEXT, "
					   " originalArtist TEXT, "
					   " textWriter TEXT, "
					   " originalTextWriter TEXT, "
					   " encodedBy TEXT, "
					   " beatsPerMinute INTEGER, "
					   " language TEXT, "
					   " contentType TEXT, "
					   " mediaType TEXT, "
					   " mood TEXT, "
					   " producedNotice TEXT, "
					   " publisher TEXT, "
					   " fileOwner TEXT, "
					   " internetRadioStationName TEXT, "
					   " internetRadioStationOwner TEXT, "
					   " originalFilename TEXT, "
					   " playlistDelay TEXT, "
					   " encodingTime INTEGER, "
					   " originalReleaseTime DATE, "
					   " releaseTime DATE, "
					   " taggingTime DATE, "
					   " swhwSettings TEXT, "
					   " albumSortOrder TEXT, "
					   " performerSortOrder TEXT, "
					   " titleSortOrder TEXT, "
					   " synchronizedLyrics TEXT, "
					   " unsynchronizedLyrics TEXT)"))
 */

class cTrack
{
public:
	cTrack(cAlbum* lpAlbum, const QString& szTitle, const QString& szTrackNumber, const QString& szPartOfSet, const QString& szBand, const QString &szLeadArtist, const QString& szComposer, const QDate& recordingTime, const QString &szFileName);

	void			setAlbum(cAlbum* lpAlbum);
	cAlbum*			album();

	void			setTitle(const QString& szTitle);
	QString			title();

	void			setTrackNumber(const QString& szTrackNumber);
	QString			trackNumber();

	void			setPartOfSet(const QString& szPartOfSet);
	QString			partOfSet();

	void			setBand(const QString& szBand);
	QString			band();

	void			setLeadArtist(const QString& szLeadArtist);
	QString			leadArtist();

	void			setComposer(const QString& szComposer);
	QString			composer();

	void			setRecordingTime(const QDate &recordingTime);
	QDate			recordingTime();

	void			setFileName(const QString& szFileName);
	QString			fileName();

private:
	cAlbum*			m_lpAlbum;
	QString			m_szTitle;
	QString			m_szTrackNumber;
	QString			m_szPartOfSet;
	QString			m_szBand;
	QString			m_szLeadArtist;
	QString			m_szComposer;
	QDate			m_recordingTime;
	QString			m_szFileName;
};

Q_DECLARE_METATYPE(cTrack*)

class cTrackList : public QList<cTrack*>
{
public:
	cTrack*			add(cAlbum* lpAlbum, const QString& szTitle, const QString& szTrackNumber, const QString& szPartOfSet, const QString& szBand, const QString& szLeadArtist, const QString& szComposer, const QDate& recordingTime, const QString& szFileName);
	void			sort(qint32 sort);
};

#endif // CTRACK_H
