#include "cmediainfo.h"

#include <QFileInfo>
#include <QSqlQuery>
#include <QVariant>

#include <QByteArray>
#include <QImage>

#include <QDebug>


cMediaInfo::cMediaInfo(bool bAPE, bool bID3V1, bool bID3V2, bool bProperties, QObject *parent) :
	QObject(parent), m_bAPE(bAPE), m_bID3V1(bID3V1), m_bID3V2(bID3V2), m_bProperties(bProperties), m_bIsValid(false), m_FileType(MEDIA_TYPE_UNKNOWN)
{
}

cMediaInfo::~cMediaInfo()
{
	clear();
}

void cMediaInfo::clear()
{
	m_szFileName			= "";
	m_FileType				= MEDIA_TYPE_UNKNOWN;
	m_bIsValid				= false;

	m_iLength				= -1;
	m_iBitrate				= -1;
	m_iSampleRate			= -1;
	m_iChannels				= -1;
	m_iBitsPerSample		= -1;
	m_iLayer				= -1;
	m_iVersion				= -1;
	m_iSampleWidth			= -1;
	m_ullSampleFrames		= 0;
	m_bIsEncrypted			= false;
	m_iTrackGain			= -1;
	m_iAlbumGain			= -1;
	m_iTrackPeak			= -1;
	m_iAlbumPeak			= -1;
	m_bProtectionEnabled	= false;
	m_channelMode			= CHANNEL_MODE_UNKNOWN;
	m_bIsCopyrighted		= false;
	m_bIsOriginal			= false;

	m_szContentGroupDescription				= "";
	m_szTitle								= "";
	m_szSubTitle							= "";
	m_szAlbum								= "";
	m_szOriginalAlbum						= "";
	m_szTrackNumber							= "";
	m_szPartOfSet							= "";
	m_szSubTitleOfSet						= "";
	m_szInternationalStandardRecordingCode	= "";

	m_szContentGroupDescription				= "";
	m_szTitle								= "";
	m_szSubTitle							= "";
	m_szAlbum								= "";
	m_szOriginalAlbum						= "";
	m_szTrackNumber							= "";
	m_szPartOfSet							= "";
	m_szSubTitleOfSet						= "";
	m_szInternationalStandardRecordingCode	= "";

	m_szLeadArtist							= "";
	m_szBand								= "";
	m_szConductor							= "";
	m_szInterpret.clear();
	m_szOriginalArtist						= "";
	m_szTextWriter							= "";
	m_szOriginalTextWriter					= "";
	m_szComposer							= "";
	m_szEncodedBy							= "";

	m_iBeatsPerMinute						= -1;
	m_szLanguage.clear();
	m_szContentType.clear();
	m_szFileType							= "";
	m_szMediaType.clear();
	m_szMood								= "";

	m_szCopyright							= "";
	m_szProducedNotice						= "";
	m_szPublisher							= "";
	m_szFileOwner							= "";
	m_szInternetRadioStationName			= "";
	m_szInternetRadioStationOwner			= "";

	m_szOriginalFilename					= "";
	m_iPlaylistDelay						= -1;
	m_szswhwSettings.clear();
	m_szAlbumSortOrder						= "";
	m_szPerformerSortOrder					= "";
	m_szTitleSortOrder						= "";

	m_szSynchronizedLyrics.clear();
	m_szSynchronizedLyricsLanguage			= "";
	m_szSynchronizedLyricsDescription		= "";

	m_szUnsynchronizedLyrics.clear();
	m_szUnsynchronizedLyricsLanguage		= "";
	m_szUnsynchronizedLyricsDescription		= "";

	m_imageList.clear();

	m_szAlbum				= "";
	m_szArtistList.clear();
	m_szAlbumArtistList.clear();
	m_szComposerList.clear();
	m_szComment				= "";
	m_szGenreList.clear();
	m_szTitle				= "";
	m_szRating				= "";
	m_szCopyright			= "";
	m_szTrackNumber			= "";
	m_iYear					= -1;

	m_iID3v2Version			= -1;
	m_iID3v2Revision		= -1;
	m_iID3v2Size			= -1;

	m_TAGAPEList.clear();
	m_TAGPropertiesList.clear();

	m_imageList.clear();
}

bool cMediaInfo::readFromFile(const QString& szFileName)
{
	clear();

	QFileInfo	FileInfo(szFileName);
	if(!FileInfo.exists())
		return(false);

	if(!FileInfo.suffix().compare("APE", Qt::CaseInsensitive))
		m_FileType	= MEDIA_TYPE_APE;
	else if(!FileInfo.suffix().compare("WMA", Qt::CaseInsensitive))
		m_FileType	= MEDIA_TYPE_ASF;
	else if(!FileInfo.suffix().compare("FLAC", Qt::CaseInsensitive))
		m_FileType	= MEDIA_TYPE_FLAC;
	else if(!FileInfo.suffix().compare("AAC", Qt::CaseInsensitive))
		m_FileType	= MEDIA_TYPE_MP4;
	else if(!FileInfo.suffix().compare("MP4", Qt::CaseInsensitive))
		m_FileType	= MEDIA_TYPE_MP4;
	else if(!FileInfo.suffix().compare("M4A", Qt::CaseInsensitive))
		m_FileType	= MEDIA_TYPE_MP4;
	else if(!FileInfo.suffix().compare("MPC", Qt::CaseInsensitive))
		m_FileType	= MEDIA_TYPE_MPC;
	else if(!FileInfo.suffix().compare("MP1", Qt::CaseInsensitive))
		m_FileType	= MEDIA_TYPE_MPEG;
	else if(!FileInfo.suffix().compare("MP2", Qt::CaseInsensitive))
		m_FileType	= MEDIA_TYPE_MPEG;
	else if(!FileInfo.suffix().compare("MP3", Qt::CaseInsensitive))
		m_FileType	= MEDIA_TYPE_MPEG;
	else if(!FileInfo.suffix().compare("TTA", Qt::CaseInsensitive))
		m_FileType	= MEDIA_TYPE_TRUEAUDIO;
	else if(!FileInfo.suffix().compare("WV", Qt::CaseInsensitive))
		m_FileType	= MEDIA_TYPE_WAVPACK;
	else if(!FileInfo.suffix().compare("WAV", Qt::CaseInsensitive))
		m_FileType	= MEDIA_TYPE_WAV;

	if(m_FileType	== MEDIA_TYPE_UNKNOWN)
		return(false);

	m_szFileName					= szFileName;
	ID3v1::Tag*			lpTagV1		= 0;
	ID3v2::Tag*			lpTagV2		= 0;
	APE::Tag*			lpTagAPE	= 0;
	APE::File*			lpApe		= 0;
	ASF::File*			lpASF		= 0;
	FLAC::File*			lpFlac		= 0;
	MP4::File*			lpMP4		= 0;
	MPC::File*			lpMPC		= 0;
	MPEG::File*			lpMPEG		= 0;
	TrueAudio::File*	lpTrueAudio	= 0;
	WavPack::File*		lpWavPack	= 0;
	RIFF::WAV::File*	lpWav		= 0;
	TagLib::PropertyMap	tags;

	QString				szTmp;

	switch(m_FileType)
	{
	case MEDIA_TYPE_APE:
	{
		lpApe	= new APE::File(m_szFileName.toLocal8Bit().data());

		lpTagV1										= lpApe->ID3v1Tag();
		lpTagAPE									= lpApe->APETag();
		tags										= lpApe->properties();

		APE::Properties* lpAudioProperties			= lpApe->audioProperties();
		m_iLength			= lpAudioProperties->length();
		m_iBitrate			= lpAudioProperties->bitrate();
		m_iSampleRate		= lpAudioProperties->sampleRate();
		m_iChannels			= lpAudioProperties->channels();
		m_iBitsPerSample	= lpAudioProperties->bitsPerSample();
		m_iVersion			= lpAudioProperties->version();
		break;
	}
	case MEDIA_TYPE_ASF:
	{
		lpASF	= new ASF::File(m_szFileName.toLocal8Bit().data());
		TagLib::ASF::Tag*	lpTag					= lpASF->tag();
		m_szTitle									= QString(lpTag->title().toCString());
		szTmp										= QString(lpTag->artist().toCString());
		if(!szTmp.isEmpty() && m_bID3V1)
			m_szArtistList							= szTmp.split("\n");
		m_szAlbum									= QString(lpTag->album().toCString());
		m_szComment									= QString(lpTag->comment().toCString());
		szTmp										= QString(lpTag->genre().toCString());
		if(!szTmp.isEmpty() && m_bID3V1)
			m_szGenreList							= szTmp.split("\n");
		m_szRating									= QString(lpTag->genre().toCString());
		m_szCopyright								= QString(lpTag->copyright().toCString());
		m_iYear										= lpTag->year();
		m_szTrackNumber								= QString("%1").arg(lpTag->track());

		tags										= lpASF->properties();

		ASF::Properties* lpAudioProperties			= lpASF->audioProperties();
		m_iLength			= lpAudioProperties->length();
		m_iBitrate			= lpAudioProperties->bitrate();
		m_iSampleRate		= lpAudioProperties->sampleRate();
		m_iChannels			= lpAudioProperties->channels();
		m_bIsEncrypted		= lpAudioProperties->isEncrypted();
		break;
	}
	case MEDIA_TYPE_FLAC:
	{
		lpFlac	= new FLAC::File(m_szFileName.toLocal8Bit().data());
		lpTagV1										= lpFlac->ID3v1Tag();
		lpTagV2										= lpFlac->ID3v2Tag();
		tags										= lpFlac->properties();

		FLAC::Properties* lpAudioProperties			= lpFlac->audioProperties();
		m_iLength			= lpAudioProperties->length();
		m_iBitrate			= lpAudioProperties->bitrate();
		m_iSampleRate		= lpAudioProperties->sampleRate();
		m_iChannels			= lpAudioProperties->channels();
		m_iSampleWidth		= lpAudioProperties->sampleWidth();
		m_ullSampleFrames	= lpAudioProperties->sampleFrames();
		break;
	}
	case MEDIA_TYPE_MP4:
	{
		lpMP4	= new MP4::File(m_szFileName.toLocal8Bit().data());
		TagLib::MP4::Tag*	lpTag					= lpMP4->tag();
		m_szTitle									= QString(lpTag->title().toCString());
		szTmp										= QString(lpTag->artist().toCString());
		if(!szTmp.isEmpty() && m_bID3V1)
			m_szArtistList							= szTmp.split("\n");
		m_szAlbum									= QString(lpTag->album().toCString());
		m_szComment									= QString(lpTag->comment().toCString());
		szTmp										= QString(lpTag->genre().toCString());
		if(!szTmp.isEmpty() && m_bID3V1)
			m_szGenreList							= szTmp.split("\n");
		m_iYear										= lpTag->year();
		m_szTrackNumber								= QString("%1").arg(lpTag->track());

		tags										= lpMP4->properties();

		MP4::Properties* lpAudioProperties			= lpMP4->audioProperties();
		m_iLength			= lpAudioProperties->length();
		m_iBitrate			= lpAudioProperties->bitrate();
		m_iSampleRate		= lpAudioProperties->sampleRate();
		m_iChannels			= lpAudioProperties->channels();
		m_iBitsPerSample	= lpAudioProperties->bitsPerSample();
		m_bIsEncrypted		= lpAudioProperties->isEncrypted();
		break;
	}
	case MEDIA_TYPE_MPC:
	{
		lpMPC	= new MPC::File(m_szFileName.toLocal8Bit().data());
		lpTagV1										= lpMPC->ID3v1Tag();
		lpTagAPE									= lpMPC->APETag();
		tags										= lpMPC->properties();

		MPC::Properties* lpAudioProperties			= lpMPC->audioProperties();
		m_iLength			= lpAudioProperties->length();
		m_iBitrate			= lpAudioProperties->bitrate();
		m_iSampleRate		= lpAudioProperties->sampleRate();
		m_iChannels			= lpAudioProperties->channels();
		m_iVersion			= lpAudioProperties->mpcVersion();
		m_ullSampleFrames	= lpAudioProperties->sampleFrames();
		m_iTrackGain		= lpAudioProperties->trackGain();
		m_iAlbumGain		= lpAudioProperties->albumGain();
		m_iTrackPeak		= lpAudioProperties->trackPeak();
		m_iAlbumPeak		= lpAudioProperties->albumPeak();
		break;
	}
	case MEDIA_TYPE_MPEG:
	{
		lpMPEG	= new MPEG::File(m_szFileName.toLocal8Bit().data());
		lpTagV1										= lpMPEG->ID3v1Tag();
		lpTagV2										= lpMPEG->ID3v2Tag();
		lpTagAPE									= lpMPEG->APETag();
		tags										= lpMPEG->properties();

		MPEG::Properties*	lpAudioProperties		= lpMPEG->audioProperties();
		m_iLength			= lpAudioProperties->length();
		m_iBitrate			= lpAudioProperties->bitrate();
		m_iSampleRate		= lpAudioProperties->sampleRate();
		m_iChannels			= lpAudioProperties->channels();
		m_iLayer			= lpAudioProperties->layer();
		switch(lpAudioProperties->version())
		{
		case MPEG::Header::Version1:
			m_iVersion	= 10;
			break;
		case MPEG::Header::Version2:
			m_iVersion	= 20;
			break;
		case MPEG::Header::Version2_5:
			m_iVersion	= 25;
			break;
		}
		m_bProtectionEnabled	= lpAudioProperties->protectionEnabled();
		switch(lpAudioProperties->channelMode())
		{
		case MPEG::Header::Stereo:
			m_channelMode	= CHANNEL_MODE_STEREO;
			break;
		case MPEG::Header::JointStereo:
			m_channelMode	= CHANNEL_MODE_JOINTSTEREO;
			break;
		case MPEG::Header::DualChannel:
			m_channelMode	= CHANNEL_MODE_DUALMONO;
			break;
		case MPEG::Header::SingleChannel:
			m_channelMode	= CHANNEL_MODE_MONO;
			break;
		}
		m_bIsCopyrighted	= lpAudioProperties->isCopyrighted();
		m_bIsOriginal		= lpAudioProperties->isOriginal();
		break;
	}
	case MEDIA_TYPE_TRUEAUDIO:
	{
		lpTrueAudio	= new TrueAudio::File(m_szFileName.toLocal8Bit().data());
		lpTagV1										= lpTrueAudio->ID3v1Tag();
		lpTagV2										= lpTrueAudio->ID3v2Tag();
		tags										= lpTrueAudio->properties();

		TrueAudio::Properties* lpAudioProperties	= lpTrueAudio->audioProperties();
		m_iLength			= lpAudioProperties->length();
		m_iBitrate			= lpAudioProperties->bitrate();
		m_iSampleRate		= lpAudioProperties->sampleRate();
		m_iChannels			= lpAudioProperties->channels();
		m_iBitsPerSample	= lpAudioProperties->bitsPerSample();
		m_iVersion			= lpAudioProperties->ttaVersion();
		break;
	}
	case MEDIA_TYPE_WAVPACK:
	{
		lpWavPack	= new WavPack::File(m_szFileName.toLocal8Bit().data());
		lpTagV1										= lpWavPack->ID3v1Tag();
		lpTagAPE									= lpWavPack->APETag();
		tags										= lpWavPack->properties();

		WavPack::Properties* lpAudioProperties		= lpWavPack->audioProperties();
		m_iLength			= lpAudioProperties->length();
		m_iBitrate			= lpAudioProperties->bitrate();
		m_iSampleRate		= lpAudioProperties->sampleRate();
		m_iChannels			= lpAudioProperties->channels();
		m_iBitsPerSample	= lpAudioProperties->bitsPerSample();
		m_ullSampleFrames	= lpAudioProperties->sampleFrames();
		m_iVersion			= lpAudioProperties->version();
		break;
	}
	case MEDIA_TYPE_WAV:
	{
		lpWav	= new RIFF::WAV::File(m_szFileName.toLocal8Bit().data());
		lpTagV2										= lpWav->tag();
		tags										= lpWav->properties();

		RIFF::WAV::Properties* lpAudioProperties	= lpWav->audioProperties();
		m_iLength			= lpAudioProperties->length();
		m_iBitrate			= lpAudioProperties->bitrate();
		m_iSampleRate		= lpAudioProperties->sampleRate();
		m_iChannels			= lpAudioProperties->channels();
		m_iSampleWidth		= lpAudioProperties->sampleWidth();
		m_ullSampleFrames	= lpAudioProperties->sampleFrames();
		break;
	}
	default:
		break;
	}

	if(lpTagV2 && m_bID3V2)
		readTagV2(lpTagV2);
	if(lpTagV1 && m_bID3V1)
		readTagV1(lpTagV1);
	if(lpTagAPE && m_bAPE)
		readTagAPE(lpTagAPE);
	if(m_bProperties)
		readTagProperties(tags);

	if(lpApe)
		delete lpApe;
	if(lpASF)
		delete lpASF;
	if(lpFlac)
		delete lpFlac;
	if(lpMP4)
		delete lpMP4;
	if(lpMPC)
		delete lpMPC;
	if(lpMPEG)
		delete lpMPEG;
	if(lpTrueAudio)
		delete lpTrueAudio;
	if(lpWavPack)
		delete lpWavPack;
	if(lpWav)
		delete lpWav;

	m_bIsValid	= true;
	return(true);
}

bool cMediaInfo::readFromDB(const QSqlDatabase& db)
{
	return(true);
}
/*
qint32 cMediaInfo::writeFilename(const QSqlDatabase& db)
{
	QSqlQuery	query(db);
	QString		sz;

	sz	= QString("SELECT id FROM file WHERE filename = \"%1\"").arg(m_szFileName);
	if(!query.exec(sz))
		return(-1);
	query.first();
	if(query.isValid())
		return(-1);

	sz	= QString("INSERT INTO file (filename, filetype, length, bitrate, samplerate, channels, bitspersample, layer, version, samplewidth, sampleframes, isencrypted, trackgain, albumgain, trackpeak, albumpeak, protectionenabled, channelmode, iscopyrighted, isoriginal, album, comment, title, rating, copyright, track, year)\n"
				  "values\n"
				  "(\"%1\", %2, %3, %4, %5, %6, %7, %8, %9, %10,\n"
				  " %11, %12, %13, %14, %15, %16, %17, %18, %19, %20, \"%21\", \"%22\", \"%23\", \"%24\", \"%25\", \"%26\", %27)").arg(m_szFileName).arg(m_FileType).arg(m_iLength).arg(m_iBitrate).arg(m_iSampleRate).arg(m_iChannels).arg(m_iBitsPerSample).arg(m_iLayer).arg(m_iVersion).arg(m_iSampleWidth).arg(m_ullSampleFrames).arg(m_bIsEncrypted).arg(m_iTrackGain).arg(m_iAlbumGain).arg(m_iTrackPeak).arg(m_iAlbumPeak).arg(m_bProtectionEnabled).arg(m_channelMode).arg(m_bIsCopyrighted).arg(m_bIsOriginal).arg(m_szAlbum).arg(m_szComment).arg(m_szTitle).arg(m_szRating).arg(m_szCopyright).arg(m_szTrackNumber).arg(m_iYear);

	if(!query.exec(sz))
		return(-1);

	sz	= QString("SELECT id FROM file WHERE filename = \"%1\"").arg(m_szFileName);
	if(!query.exec(sz))
		return(-1);
	query.first();
	if(!query.isValid())
		return(-1);
	return(query.value(0).toInt());
}

qint32 cMediaInfo::writeArtist(const QSqlDatabase& db, qint32 idFile, const QString& szArtist)
{
	QSqlQuery	query(db);
	QString		sz;

	sz	= QString("SELECT id FROM id3v1artist WHERE fileid = %1 AND person = \"%2\"").arg(idFile).arg(szArtist);
	if(!query.exec(sz))
		return(-1);
	query.first();
	if(query.isValid())
		return(-1);

	for(int z = 0;z < m_szArtistList.count();z++)
	{
		QString	szArtist	= m_szArtistList.at(z);
		sz	= QString("INSERT INTO id3v1artist (fileid, person) VALUES (%1, \"%2\")").arg(idFile).arg(szArtist);
		if(!query.exec(sz))
			return(-1);
	}
}
*/
bool cMediaInfo::writeToDB(const QSqlDatabase& db)
{
/*
	QSqlQuery	query(db);
	QString		sz;
	qint32		idFile;
	qint32		id3v2;
	qint32		idAPE;
	qint32		idProperties;

	idFile	= writeFilename(db);
	if(idFile == -1)
		return(false);

	if(m_szArtistList.count())
	{
		sz	= QString("SELECT id FROM id3v1artist WHERE fileid = %1").arg(idFile);
		if(!query.exec(sz))
			return(false);
		query.first();
		if(query.isValid())
			return(false);

		for(int z = 0;z < m_szArtistList.count();z++)
		{
			QString	szArtist	= m_szArtistList.at(z);
			sz	= QString("INSERT INTO id3v1artist (fileid, person) VALUES (%1, \"%2\")").arg(idFile).arg(szArtist);
			if(!query.exec(sz))
				return(false);
		}
	}

	if(m_szGenreList.count())
	{
		sz	= QString("SELECT id FROM id3v1genre WHERE fileid = %1").arg(idFile);
		if(!query.exec(sz))
			return(false);
		query.first();
		if(query.isValid())
			return(false);

		for(int z = 0;z < m_szGenreList.count();z++)
		{
			QString	szGenre	= m_szGenreList.at(z);
			sz	= QString("INSERT INTO id3v1genre (fileid, genre) VALUES (%1, \"%2\")").arg(idFile).arg(szGenre);
			if(!query.exec(sz))
				return(false);
		}
	}

	if(m_TAGv2List.count())
	{
		sz	= QString("SELECT id FROM id3v2 WHERE fileid = %1").arg(idFile);
		if(!query.exec(sz))
			return(false);
		query.first();
		if(query.isValid())
			return(false);

		sz	= QString("INSERT INTO id3v2 (fileid, version, revision, size) VALUES (%1, %2, %3, %4)").arg(idFile).arg(m_iID3v2Version).arg(m_iID3v2Revision).arg(m_iID3v2Size);
		if(!query.exec(sz))
			return(false);
		sz	= QString("SELECT id FROM id3v2 WHERE fileid = %1").arg(idFile);
		if(!query.exec(sz))
			return(false);
		query.first();
		id3v2	= query.value(0).toInt();

		for(int z = 0;z < m_TAGv2List.count();z++)
		{
			cTAG*		lpTag	= m_TAGv2List.at(z);
			QString		szTag	= lpTag->tag();
			QStringList	szValue	= lpTag->valueList();
			for(int y = 0;y < szValue.count();y++)
			{
				sz	= QString("INSERT INTO id3v2tag (id3v2id, tag, value) VALUES (%1, \"%2\", \"%3\")").arg(id3v2).arg(szTag).arg(szValue.at(y));
				if(!query.exec(sz))
					return(false);
			}
		}
	}

	if(m_TAGAPEList.count())
	{
		sz	= QString("SELECT id FROM ape WHERE fileid = %1").arg(idFile);
		if(!query.exec(sz))
			return(false);
		query.first();
		if(query.isValid())
			return(false);

		sz	= QString("INSERT INTO ape (fileid) VALUES (%1)").arg(idFile);
		if(!query.exec(sz))
			return(false);
		sz	= QString("SELECT id FROM ape WHERE fileid = %1").arg(idFile);
		if(!query.exec(sz))
			return(false);
		query.first();
		idAPE	= query.value(0).toInt();

		for(int z = 0;z < m_TAGAPEList.count();z++)
		{
			cTAG*		lpTag	= m_TAGAPEList.at(z);
			QString		szTag	= lpTag->tag();
			QStringList	szValue	= lpTag->valueList();
			for(int y = 0;y < szValue.count();y++)
			{
				sz	= QString("INSERT INTO apetag (apeid, tag, value) VALUES (%1, \"%2\", \"%3\")").arg(idAPE).arg(szTag).arg(szValue.at(y));
				if(!query.exec(sz))
					return(false);
			}
		}
	}

	if(m_TAGPropertiesList.count())
	{
		sz	= QString("SELECT id FROM properties WHERE fileid = %1").arg(idFile);
		if(!query.exec(sz))
			return(false);
		query.first();
		if(query.isValid())
			return(false);

		sz	= QString("INSERT INTO properties (fileid) VALUES (%1)").arg(idFile);
		if(!query.exec(sz))
			return(false);
		sz	= QString("SELECT id FROM properties WHERE fileid = %1").arg(idFile);
		if(!query.exec(sz))
			return(false);
		query.first();
		idProperties	= query.value(0).toInt();

		for(int z = 0;z < m_TAGPropertiesList.count();z++)
		{
			cTAG*		lpTag	= m_TAGPropertiesList.at(z);
			QString		szTag	= lpTag->tag();
			QStringList	szValue	= lpTag->valueList();
			for(int y = 0;y < szValue.count();y++)
			{
				sz	= QString("INSERT INTO propertiestag (propertiesid, tag, value) VALUES (%1, \"%2\", \"%3\")").arg(idProperties).arg(szTag).arg(szValue.at(y));
				if(!query.exec(sz))
					return(false);
			}
		}
	}
*/
	return(true);
}

void cMediaInfo::readTagV1(ID3v1::Tag* lpTag)
{
	QString	szTmp;
	m_szAlbum				= QString(lpTag->album().toCString());
	szTmp					= QString(lpTag->artist().toCString());
	if(!szTmp.isEmpty())
		m_szArtistList		= szTmp.split("\n");
	m_szAlbum				= QString(lpTag->album().toCString());
	m_szComment				= QString(lpTag->comment().toCString());
	szTmp					= QString(lpTag->genre().toCString());
	if(!szTmp.isEmpty())
		m_szGenreList		= szTmp.split("\n");
	m_szTitle				= QString(lpTag->title().toCString());
	m_szTrackNumber			= QString("%1").arg(lpTag->track());
	m_iYear					= lpTag->year();
}

void cMediaInfo::readTagV2(ID3v2::Tag* lpTag)
{
	m_iID3v2Version		= lpTag->header()->majorVersion();
	m_iID3v2Revision	= lpTag->header()->revisionNumber();
	m_iID3v2Size		= lpTag->header()->tagSize();

	for(ID3v2::FrameList::ConstIterator	it = lpTag->frameList().begin();it != lpTag->frameList().end();it++)
	{
		QString	szID	= QString("%1").arg((*it)->frameID().data()).left(4);

		if(!szID.compare("TIT1"))
			m_szContentGroupDescription				= (*it)->toString().toCString();
		else if(!szID.compare("TIT2"))
			m_szTitle								= (*it)->toString().toCString();
		else if(!szID.compare("TIT3"))
			m_szSubTitle							= (*it)->toString().toCString();
		else if(!szID.compare("TALB"))
			m_szAlbum								= (*it)->toString().toCString();
		else if(!szID.compare("TOAL"))
			m_szOriginalAlbum						= (*it)->toString().toCString();
		else if(!szID.compare("TRCK"))
			m_szTrackNumber							= (*it)->toString().toCString();
		else if(!szID.compare("TPOS"))
			m_szPartOfSet							= (*it)->toString().toCString();
		else if(!szID.compare("TSST"))
			m_szSubTitleOfSet						= (*it)->toString().toCString();
		else if(!szID.compare("TSRC"))
			m_szInternationalStandardRecordingCode	= (*it)->toString().toCString();
		else if(!szID.compare("TPE1"))
			m_szLeadArtist							= (*it)->toString().toCString();
		else if(!szID.compare("TPE2"))
			m_szBand								= (*it)->toString().toCString();
		else if(!szID.compare("TPE3"))
			m_szConductor							= (*it)->toString().toCString();
		else if(!szID.compare("TPE4"))
			m_szInterpret							= QString((*it)->toString().toCString()).split("\r\n");
		else if(!szID.compare("TOPE"))
			m_szOriginalArtist						= (*it)->toString().toCString();
		else if(!szID.compare("TEXT"))
			m_szTextWriter							= (*it)->toString().toCString();
		else if(!szID.compare("TOLY"))
			m_szOriginalTextWriter					= (*it)->toString().toCString();
		else if(!szID.compare("TCOM"))
			m_szComposer							= (*it)->toString().toCString();
		else if(!szID.compare("TENC"))
			m_szEncodedBy							= (*it)->toString().toCString();
		else if(!szID.compare("TBPM"))
			m_iBeatsPerMinute						= QString((*it)->toString().toCString()).toInt();
		else if(!szID.compare("TLEN") && m_iLength == 0)
			m_iLength								= QString((*it)->toString().toCString()).toInt();
		else if(!szID.compare("TLAN"))
				m_szLanguage						= QString((*it)->toString().toCString()).split("\r\n");
		else if(!szID.compare("TCON"))
				m_szContentType						= QString((*it)->toString().toCString()).split("\r\n");
		else if(!szID.compare("TFLT"))
			m_szFileType							= (*it)->toString().toCString();
		else if(!szID.compare("TMED"))
				m_szMediaType						= QString((*it)->toString().toCString()).split("\r\n");
		else if(!szID.compare("TMOO"))
			m_szMood								= (*it)->toString().toCString();
		else if(!szID.compare("TCOP"))
			m_szCopyright							= (*it)->toString().toCString();
		else if(!szID.compare("TPRO"))
			m_szProducedNotice						= (*it)->toString().toCString();
		else if(!szID.compare("TPUB"))
			m_szPublisher							= (*it)->toString().toCString();
		else if(!szID.compare("TOWN"))
			m_szFileOwner							= (*it)->toString().toCString();
		else if(!szID.compare("TRSN"))
			m_szInternetRadioStationName			= (*it)->toString().toCString();
		else if(!szID.compare("TRSO"))
			m_szInternetRadioStationOwner			= (*it)->toString().toCString();
		else if(!szID.compare("TOFN"))
			m_szOriginalFilename					= (*it)->toString().toCString();
		else if(!szID.compare("TDLY"))
			m_iPlaylistDelay						= QString((*it)->toString().toCString()).toInt();
		else if(!szID.compare("TDEN"))
			m_EncodingTime							= str2TS((*it)->toString().toCString());
		else if(!szID.compare("TDOR"))
			m_OriginalReleaseTime					= str2TS((*it)->toString().toCString());
		else if(!szID.compare("TDRC"))
			m_RecordingTime							= str2TS((*it)->toString().toCString());
		else if(!szID.compare("TDRL"))
			m_ReleaseTime							= str2TS((*it)->toString().toCString());
		else if(!szID.compare("TDTG"))
			m_TaggingTime							= str2TS((*it)->toString().toCString());
		else if(!szID.compare("TSSE"))
			m_szswhwSettings						= QString((*it)->toString().toCString()).split("\r\n");
		else if(!szID.compare("TSOA"))
			m_szAlbumSortOrder						= (*it)->toString().toCString();
		else if(!szID.compare("TSOP"))
			m_szPerformerSortOrder					= (*it)->toString().toCString();
		else if(!szID.compare("TSOT"))
			m_szTitleSortOrder						= (*it)->toString().toCString();
		else if(!szID.compare("SYLT"))
		{
			TagLib::ID3v2::SynchronizedLyricsFrame*	lpLyrics	= static_cast<TagLib::ID3v2::SynchronizedLyricsFrame *> (*it);

			m_szSynchronizedLyricsLanguage			= QString(lpLyrics->language().data()).left(3);
			String::Type	type					= lpLyrics->textEncoding();
			m_szSynchronizedLyricsDescription		= lpLyrics->description().toCString();

			TagLib::ID3v2::SynchronizedLyricsFrame::SynchedTextList	list	= lpLyrics->synchedText();

			for(ID3v2::SynchronizedLyricsFrame::SynchedTextList::ConstIterator it1 = list.begin(); it1 != list.end();it1++)
			{
				ID3v2::SynchronizedLyricsFrame::SynchedText	t	= *(it1);
				m_szSynchronizedLyrics.add(t.time, QString(t.text.toCString()));
			}
		}
		else if(!szID.compare("USLT"))
		{
			TagLib::ID3v2::UnsynchronizedLyricsFrame*	lpLyrics	= static_cast<TagLib::ID3v2::UnsynchronizedLyricsFrame *> (*it);

			m_szUnsynchronizedLyricsLanguage		= QString(lpLyrics->language().data()).left(3);
			String::Type	type					= lpLyrics->textEncoding();
			m_szUnsynchronizedLyricsDescription		= lpLyrics->description().toCString();
			QString			szText					= lpLyrics->text().toCString();
			if(szText.contains("\r\n"))
				m_szUnsynchronizedLyrics			= szText.split("\r\n");
			else if(szText.contains("\r"))
				m_szUnsynchronizedLyrics			= szText.split("\r");
			else if(szText.contains("\n"))
				m_szUnsynchronizedLyrics			= szText.split("\n");
			else
				m_szUnsynchronizedLyrics.append(szText);
		}
		else if(!szID.compare("APIC"))
		{
			TagLib::ID3v2::AttachedPictureFrame*		lpPicture	= static_cast<TagLib::ID3v2::AttachedPictureFrame *> (*it);
			TagLib::ID3v2::AttachedPictureFrame::Type	t			= lpPicture->type();
			QString	szDescription;
			szDescription	= lpPicture->description().toCString();

			QByteArray	pictureData	= QByteArray(lpPicture->picture().data(), lpPicture->picture().size());
			m_imageList.add(pictureData, "", (cImage::ImageType)t, szDescription);
		}
	}
}

void cMediaInfo::readTagAPE(APE::Tag* lpTag)
{
	for(APE::ItemListMap::ConstIterator it = lpTag->itemListMap().begin();it != lpTag->itemListMap().end();++it)
	{
		QString szID	= (*it).first.toCString();
		QString	szVal	= (*it).second.toString().toCString();
		if(!szID.isEmpty() && !szVal.isEmpty())
			m_TAGAPEList.add(szID.left(4), szVal);
	}
}

void cMediaInfo::readTagProperties(TagLib::PropertyMap& tags)
{
	QString		szID;

	for(TagLib::PropertyMap::ConstIterator i = tags.begin(); i != tags.end(); ++i)
	{
		QStringList	szValue;
		szID	= i->first.toCString();
		if(!szID.isEmpty())
		{
			for(TagLib::StringList::ConstIterator j = i->second.begin(); j != i->second.end(); ++j)
			{
				QString	sz = QString((*j).toCString());
				if(!sz.isEmpty())
				{
					if(!szID.compare("LYRICS"))
					{
						sz.replace("\"", "'");
						sz.replace("\r\n", "\n");
						szValue.append(sz);
					}
					else
						szValue.append(sz.split("\n"));
				}
			}
			if(!szID.isEmpty() && !szValue.isEmpty())
				m_TAGPropertiesList.add(szID, szValue);
		}
	}
}

QDateTime cMediaInfo::str2TS(const QString& sz)
{
	QDateTime	dt;

	switch(sz.length())
	{
	case 4:
		dt	= QDateTime::fromString(sz, QString("yyyy"));
		break;
	case 7:
		dt	= QDateTime::fromString(sz, QString("yyyy-MM"));
		break;
	case 10:
		dt	= QDateTime::fromString(sz, QString("yyyy-MM-dd"));
		break;
	case 13:
		dt	= QDateTime::fromString(sz, QString("yyyy-MM-ddTHH"));
		break;
	case 16:
		dt	= QDateTime::fromString(sz, QString("yyyy-MM-ddTHH:mm"));
		break;
	case 19:
		dt	= QDateTime::fromString(sz, QString("yyyy-MM-ddTHH:mm:ss"));
		break;
	default:
		return(QDateTime());
	}
	return(dt);
}

bool cMediaInfo::isValid()
{
	return(m_bIsValid);
}

QString cMediaInfo::fileName()
{
	return(m_szFileName);
}

cMediaInfo::MEDIA_TYPE cMediaInfo::fileType1()
{
	return(m_FileType);
}

QString cMediaInfo::fileType1Text()
{
	switch(m_FileType)
	{
	case MEDIA_TYPE_APE:
		return("APE");
	case MEDIA_TYPE_ASF:
		return("ASF");
	case MEDIA_TYPE_FLAC:
		return("FLAC");
	case MEDIA_TYPE_MP4:
		return("MP4");
	case MEDIA_TYPE_MPC:
		return("MPC");
	case MEDIA_TYPE_MPEG:
		return("MPEG");
	case MEDIA_TYPE_TRUEAUDIO:
		return("TrueAudio");
	case MEDIA_TYPE_WAVPACK:
		return("WavPack");
	case MEDIA_TYPE_WAV:
		return("Wave");
	default:
		return("unknown");
	}
}

qint32 cMediaInfo::length1()
{
	return(m_iLength);
}

QString cMediaInfo::length1Text()
{
	return("not implemented");
}

qint16 cMediaInfo::bitrate()
{
	return(m_iBitrate);
}

QString cMediaInfo::bitrateText()
{
	return("not implemented");
}

qint32 cMediaInfo::sampleRate()
{
	return(m_iSampleRate);
}

QString cMediaInfo::sampleRateText()
{
	return("not implemented");
}

qint16 cMediaInfo::channels()
{
	return(m_iChannels);
}

QString cMediaInfo::channelsText()
{
	return("not implemented");
}

qint16 cMediaInfo::bitsPerSample()
{
	return(m_iBitsPerSample);
}

QString cMediaInfo::bitsPerSampleText()
{
	return("not implemented");
}

qint16 cMediaInfo::layer()
{
	return(m_iLayer);
}

QString cMediaInfo::layerText()
{
	return("not implemented");
}

qint16 cMediaInfo::version()
{
	return(m_iVersion);
}

QString cMediaInfo::versionText()
{
	return("not implemented");
}

qint16 cMediaInfo::sampleWidth()
{
	return(m_iSampleWidth);
}

QString cMediaInfo::sampleWidthText()
{
	return("not implemented");
}

qint64 cMediaInfo::sampleFrames()
{
	return(m_ullSampleFrames);
}

QString cMediaInfo::sampleFramesText()
{
	return("not implemented");
}

bool cMediaInfo::isEncrypted()
{
	return(m_bIsEncrypted);
}

qint16 cMediaInfo::trackGain()
{
	return(m_iTrackGain);
}

qint16 cMediaInfo::albumGain()
{
	return(m_iAlbumGain);
}

qint16 cMediaInfo::trackPeak()
{
	return(m_iTrackPeak);
}

qint16 cMediaInfo::albumPeak()
{
	return(m_iAlbumPeak);
}

bool cMediaInfo::protectionEnabled()
{
	return(m_bProtectionEnabled);
}

cMediaInfo::CHANNEL_MODE cMediaInfo::channelMode()
{
	return(m_channelMode);
}

QString cMediaInfo::channelModeText()
{
	switch(m_channelMode)
	{
	case CHANNEL_MODE_STEREO:
		return("stereo");
	case CHANNEL_MODE_JOINTSTEREO:
		return("joint stereo");
	case CHANNEL_MODE_MONO:
		return("mono");
	case CHANNEL_MODE_DUALMONO:
		return("dual mono");
	default:
		return("unknown");
	}
}

bool cMediaInfo::isCopyrighted()
{
	return(m_bIsCopyrighted);
}

bool cMediaInfo::isOriginal()
{
	return(m_bIsOriginal);
}

QString cMediaInfo::contentGroupDescription()
{
	return(m_szContentGroupDescription);
}

QString cMediaInfo::title()
{
	return(m_szTitle);
}

QString cMediaInfo::subTitle()
{
	return(m_szSubTitle);
}

QString cMediaInfo::album()
{
	return(m_szAlbum);
}

QString cMediaInfo::originalAlbum()
{
	return(m_szOriginalAlbum);
}

QString cMediaInfo::trackNumber()
{
	return(m_szTrackNumber);
}

QString cMediaInfo::partOfSet()
{
	return(m_szPartOfSet);
}

QString cMediaInfo::subTitleOfSet()
{
	return(m_szSubTitleOfSet);
}

QString cMediaInfo::internationalStandardRecordingCode()
{
	return(m_szInternationalStandardRecordingCode);
}

QString cMediaInfo::leadArtist()
{
	return(m_szLeadArtist);
}

QString cMediaInfo::band()
{
	return(m_szBand);
}

QString cMediaInfo::conductor()
{
	return(m_szConductor);
}

QStringList cMediaInfo::interpret()
{
	return(m_szInterpret);
}

QString cMediaInfo::originalArtist()
{
	return(m_szOriginalArtist);
}

QString cMediaInfo::textWriter()
{
	return(m_szTextWriter);
}

QString cMediaInfo::originalTextWriter()
{
	return(m_szOriginalTextWriter);
}

QString cMediaInfo::composer()
{
	return(m_szComposer);
}

QString cMediaInfo::encodedBy()
{
	return(m_szEncodedBy);
}

qint16 cMediaInfo::beatsPerMinute()
{
	return(m_iBeatsPerMinute);
}

qint32 cMediaInfo::length()
{
	return(m_iLength);
}

QStringList cMediaInfo::language()
{
	return(m_szLanguage);
}

QStringList cMediaInfo::contentType()
{
	return(m_szContentType);
}

QString cMediaInfo::fileType()
{
	return(m_szFileType);
}

QStringList cMediaInfo::mediaType()
{
	return(m_szMediaType);
}

QString cMediaInfo::mood()
{
	return(m_szMood);
}

QString cMediaInfo::copyright()
{
	return(m_szCopyright);
}

QString cMediaInfo::producedNotice()
{
	return(m_szProducedNotice);
}

QString cMediaInfo::publisher()
{
	return(m_szPublisher);
}

QString cMediaInfo::fileOwner()
{
	return(m_szFileOwner);
}

QString cMediaInfo::internetRadioStationName()
{
	return(m_szInternetRadioStationName);
}

QString cMediaInfo::internetRadioStationOwner()
{
	return(m_szInternetRadioStationOwner);
}

QString cMediaInfo::originalFilename()
{
	return(m_szOriginalFilename);
}

qint32 cMediaInfo::playlistDelay()
{
	return(m_iPlaylistDelay);
}

QDateTime cMediaInfo::encodingTime()
{
	return(m_EncodingTime);
}

QDateTime cMediaInfo::originalReleaseTime()
{
	return(m_OriginalReleaseTime);
}

QDateTime cMediaInfo::recordingTime()
{
	return(m_RecordingTime);
}

QDateTime cMediaInfo::releaseTime()
{
	return(m_ReleaseTime);
}

QDateTime cMediaInfo::taggingTime()
{
	return(m_TaggingTime);
}

QStringList cMediaInfo::swhwSettings()
{
	return(m_szswhwSettings);
}

QString cMediaInfo::albumSortOrder()
{
	return(m_szAlbumSortOrder);
}

QString cMediaInfo::performerSortOrder()
{
	return(m_szPerformerSortOrder);
}

QString cMediaInfo::titleSortOrder()
{
	return(m_szTitleSortOrder);
}

cStringTimeList cMediaInfo::synchronizedLyrics()
{
	return(m_szSynchronizedLyrics);
}

QStringList cMediaInfo::unsynchronizedLyrics()
{
	return(m_szUnsynchronizedLyrics);
}

cImageList cMediaInfo::images()
{
	return(m_imageList);
}

/*
 * http://de.wikipedia.org/wiki/ID3-Tag
 * http://id3.org/id3v2.4.0-frames
 * http://id3.org/id3v2.4.0-structure
*/
