#ifndef CMEDIAINFO_H
#define CMEDIAINFO_H

#include <QObject>

#include <QString>
#include <QStringList>
#include <QList>
#include <QDateTime>
#include <QMetaType>
#include <QSqlDatabase>

#include <taglib/tbytevector.h>
#include <taglib/apefile.h>
#include <taglib/asffile.h>
#include <taglib/flacfile.h>
#include <taglib/mp4file.h>
#include <taglib/mpcfile.h>
#include <taglib/mpegfile.h>
#include <taglib/trueaudiofile.h>
#include <taglib/wavpackfile.h>
#include <taglib/wavfile.h>

#include <taglib/id3v1tag.h>
#include <taglib/id3v2tag.h>
#include <taglib/id3v2frame.h>
#include <taglib/id3v2header.h>
#include <taglib/apetag.h>
#include <taglib/tpropertymap.h>

#include <taglib/attachedpictureframe.h>
#include <taglib/synchronizedlyricsframe.h>
#include <taglib/unsynchronizedlyricsframe.h>

#include "ctag.h"
#include "cstring.h"
#include "cimage.h"


using namespace TagLib;

class cMediaInfo : public QObject
{
	Q_OBJECT
public:
	enum MEDIA_TYPE
	{
		MEDIA_TYPE_UNKNOWN		= 0,
		MEDIA_TYPE_APE			= 1,	// APE
		MEDIA_TYPE_ASF			= 2,	// WMA
		MEDIA_TYPE_FLAC			= 3,	// FLAC
		MEDIA_TYPE_MP4			= 4,	// AAC, MP4, M4A
		MEDIA_TYPE_MPC			= 5,	// MPC
		MEDIA_TYPE_MPEG			= 6,	// MP1, MP2, MP3
		MEDIA_TYPE_TRUEAUDIO	= 7,	// TTA
		MEDIA_TYPE_WAVPACK		= 8,	// WV
		MEDIA_TYPE_WAV			= 9,	// WAV
	};
	enum CHANNEL_MODE
	{
		CHANNEL_MODE_UNKNOWN		= 0,
		CHANNEL_MODE_STEREO			= 1,
		CHANNEL_MODE_JOINTSTEREO	= 2,
		CHANNEL_MODE_MONO			= 3,
		CHANNEL_MODE_DUALMONO		= 4
	};

	explicit		cMediaInfo(bool bAPE = true, bool bID3V1 = true, bool bID3V2 = true, bool bProperties = true, QObject *parent = 0);
	~cMediaInfo();

	bool			readFromFile(const QString& szFileName);
	bool			readFromDB(const QSqlDatabase& db);
	bool			writeToDB(const QSqlDatabase& db);

	bool			isValid();

	//
	QString			fileName();
	MEDIA_TYPE		fileType1();
	QString			fileType1Text();

	//
	qint32			length1();
	QString			length1Text();
	qint16			bitrate();
	QString			bitrateText();
	qint32			sampleRate();
	QString			sampleRateText();
	qint16			channels();
	QString			channelsText();
	qint16			bitsPerSample();
	QString			bitsPerSampleText();
	qint16			layer();
	QString			layerText();
	qint16			version();
	QString			versionText();
	qint16			sampleWidth();
	QString			sampleWidthText();
	qint64			sampleFrames();
	QString			sampleFramesText();
	bool			isEncrypted();
	qint16			trackGain();
	qint16			albumGain();
	qint16			trackPeak();
	qint16			albumPeak();
	bool			protectionEnabled();
	CHANNEL_MODE	channelMode();
	QString			channelModeText();
	bool			isCopyrighted();
	bool			isOriginal();

	// Identification Frames
	QString			contentGroupDescription();				// TIT1
	QString			title();								// TIT2
	QString			subTitle();								// TIT3
	QString			album();								// TALB
	QString			originalAlbum();						// TOAL
	QString			trackNumber();							// TRCK
	QString			partOfSet();							// TPOS
	QString			subTitleOfSet();						// TSST
	QString			internationalStandardRecordingCode();	// TSRC

	// Involved Persons Frames
	QString			leadArtist();							// TPE1
	QString			band();									// TPE2
	QString			conductor();							// TPE3
	QStringList		interpret();							// TPE4
	QString			originalArtist();						// TOPE
	QString			textWriter();							// TEXT
	QString			originalTextWriter();					// TOLY
	QString			composer();								// TCOM
	//cString21List	musicianCredits();						// TMCL
	//cString21List	involvedPeople();						// TIPL
	QString			encodedBy();							// TENC

	// Derived and subjective properties frames
	qint16			beatsPerMinute();						// TBPM
	qint32			length();								// TLEN
	// TKEY
	QStringList		language();								// TLAN
	QStringList		contentType();							// TCON
	QString			fileType();								// TFLT
	QStringList		mediaType();							// TMED
	QString			mood();									// TMOO

	// Rights and license frames
	QString			copyright();							// TCOP
	QString			producedNotice();						// TPRO
	QString			publisher();							// TPUB
	QString			fileOwner();							// TOWN
	QString			internetRadioStationName();				// TRSN
	QString			internetRadioStationOwner();			// TRSO

	// Other text frames
	QString			originalFilename();						// TOFN
	qint32			playlistDelay();						// TDLY
	QDateTime		encodingTime();							// TDEN
	QDateTime		originalReleaseTime();					// TDOR
	QDateTime		recordingTime();						// TDRC
	QDateTime		releaseTime();							// TDRL
	QDateTime		taggingTime();							// TDTG
	QStringList		swhwSettings();							// TSSE
	QString			albumSortOrder();						// TSOA
	QString			performerSortOrder();					// TSOP
	QString			titleSortOrder();						// TSOT

	cStringTimeList	synchronizedLyrics();					// SYLT
	QStringList		unsynchronizedLyrics();					// USLT

	cImageList		images();								// APIC
signals:
	
public slots:

protected:
	bool			m_bAPE;
	bool			m_bID3V1;
	bool			m_bID3V2;
	bool			m_bProperties;
	bool			m_bIsValid;
	QString			m_szFileName;
	MEDIA_TYPE		m_FileType;

	qint32			m_iLength;
	qint16			m_iBitrate;
	qint32			m_iSampleRate;
	qint16			m_iChannels;
	qint16			m_iBitsPerSample;
	qint16			m_iLayer;
	qint16			m_iVersion;
	qint16			m_iSampleWidth;
	qint64			m_ullSampleFrames;
	bool			m_bIsEncrypted;
	qint16			m_iTrackGain;
	qint16			m_iAlbumGain;
	qint16			m_iTrackPeak;
	qint16			m_iAlbumPeak;
	bool			m_bProtectionEnabled;
	CHANNEL_MODE	m_channelMode;
	bool			m_bIsCopyrighted;
	bool			m_bIsOriginal;

	// ID3V1
	QStringList		m_szAlbumArtistList;
	QStringList		m_szArtistList;
	QString			m_szComment;
	QStringList		m_szComposerList;
	QStringList		m_szGenreList;
	QString			m_szRating;
	qint16			m_iYear;

	// Identification Frames
	QString			m_szContentGroupDescription;			// TIT1
	QString			m_szTitle;								// TIT2
	QString			m_szSubTitle;							// TIT3
	QString			m_szAlbum;								// TALB
	QString			m_szOriginalAlbum;						// TOAL
	QString			m_szTrackNumber;						// TRCK
	QString			m_szPartOfSet;							// TPOS
	QString			m_szSubTitleOfSet;						// TSST
	QString			m_szInternationalStandardRecordingCode;	// TSRC

	// Involved Persons Frames
	QString			m_szLeadArtist;							// TPE1
	QString			m_szBand;								// TPE2
	QString			m_szConductor;							// TPE3
	QStringList		m_szInterpret;							// TPE4
	QString			m_szOriginalArtist;						// TOPE
	QString			m_szTextWriter;							// TEXT
	QString			m_szOriginalTextWriter;					// TOLY
	QString			m_szComposer;							// TCOM
	QString			m_szEncodedBy;							// TENC

	// Derived and subjective properties frames
	qint16			m_iBeatsPerMinute;						// TBPM
//	qint16			m_iLength;								// TLEN
	// TKEY
	QStringList		m_szLanguage;							// TLAN
	QStringList		m_szContentType;						// TCON
	QString			m_szFileType;							// TFLT
	QStringList		m_szMediaType;							// TMED
	QString			m_szMood;								// TMOO

	// Rights and license frames
	QString			m_szCopyright;							// TCOP
	QString			m_szProducedNotice;						// TPRO
	QString			m_szPublisher;							// TPUB
	QString			m_szFileOwner;							// TOWN
	QString			m_szInternetRadioStationName;			// TRSN
	QString			m_szInternetRadioStationOwner;			// TRSO

	// Other text frames
	QString			m_szOriginalFilename;					// TOFN
	qint32			m_iPlaylistDelay;						// TDLY
	QDateTime		m_EncodingTime;							// TDEN
	QDateTime		m_OriginalReleaseTime;					// TDOR
	QDateTime		m_RecordingTime;						// TDRC
	QDateTime		m_ReleaseTime;							// TDRL
	QDateTime		m_TaggingTime;							// TDTG
	QStringList		m_szswhwSettings;						// TSSE
	QString			m_szAlbumSortOrder;						// TSOA
	QString			m_szPerformerSortOrder;					// TSOP
	QString			m_szTitleSortOrder;						// TSOT

	cStringTimeList	m_szSynchronizedLyrics;					// SYLT
	QString			m_szSynchronizedLyricsLanguage;
	QString			m_szSynchronizedLyricsDescription;
	QStringList		m_szUnsynchronizedLyrics;				// USLT
	QString			m_szUnsynchronizedLyricsLanguage;
	QString			m_szUnsynchronizedLyricsDescription;

	//ID3V2
	qint32			m_iID3v2Version;
	qint32			m_iID3v2Revision;
	qint32			m_iID3v2Size;
	//cTAGList		m_TAGv2List;
	cTAGList		m_TAGAPEList;
	cTAGList		m_TAGPropertiesList;

	cImageList		m_imageList;

	void			clear();

	void			readTagV1(ID3v1::Tag* lpTag);
	void			readTagV2(ID3v2::Tag* lpTag);
	void			readTagAPE(APE::Tag* lpTag);
	void			readTagProperties(TagLib::PropertyMap& tags);

	QDateTime		str2TS(const QString& sz);
};

#endif // CMEDIAINFO_H
