#include "cmediainfo.h"

#include "common.h"

#include <QFileInfo>
#include <QSqlQuery>
#include <QSqlError>

#include <QVariant>

#include <QByteArray>
#include <QImage>
#include <QPixmap>

#include <QDebug>


cMediaInfo::cMediaInfo(bool bAPE, bool bID3V1, bool bID3V2, bool bProperties, QObject *parent) :
	QObject(parent), m_bAPE(bAPE), m_bID3V1(bID3V1), m_bID3V2(bID3V2), m_bProperties(bProperties), m_bIsValid(false), m_fileType(MEDIA_TYPE_UNKNOWN)
{
}

cMediaInfo::~cMediaInfo()
{
	clear();
}

void cMediaInfo::clear()
{
	m_szFileName							= "";
	m_fileType								= MEDIA_TYPE_UNKNOWN;
	m_bIsValid								= false;

	m_iLength								= -1;
	m_iBitrate								= -1;
	m_iSampleRate							= -1;
	m_iChannels								= -1;
	m_iBitsPerSample						= -1;
	m_iLayer								= -1;
	m_iVersion								= -1;
	m_iSampleWidth							= -1;
	m_ullSampleFrames						= 0;
	m_bIsEncrypted							= false;
	m_iTrackGain							= -1;
	m_iAlbumGain							= -1;
	m_iTrackPeak							= -1;
	m_iAlbumPeak							= -1;
	m_bProtectionEnabled					= false;
	m_channelMode							= CHANNEL_MODE_UNKNOWN;
	m_bIsCopyrighted						= false;
	m_bIsOriginal							= false;

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

	m_szAlbum								= "";
	m_szArtistList.clear();
	m_szAlbumArtistList.clear();
	m_szComposerList.clear();
	m_szComment								= "";
	m_szGenreList.clear();
	m_szTitle								= "";
	m_szRating								= "";
	m_szCopyright							= "";
	m_szTrackNumber							= "";
	m_iYear									= -1;

	m_iID3v2Version							= -1;
	m_iID3v2Revision						= -1;
	m_iID3v2Size							= -1;

	m_TAGAPEList.clear();
	m_TAGPropertiesList.clear();

	m_imageList.clear();
	m_images.clear();
}

bool cMediaInfo::readFromFile(const QString& szFileName)
{
	clear();

	QFileInfo	fileInfo(szFileName);
	if(!fileInfo.exists())
		return(false);

	if(!fileInfo.suffix().compare("APE", Qt::CaseInsensitive))
		m_fileType	= MEDIA_TYPE_APE;
	else if(!fileInfo.suffix().compare("WMA", Qt::CaseInsensitive))
		m_fileType	= MEDIA_TYPE_ASF;
	else if(!fileInfo.suffix().compare("FLAC", Qt::CaseInsensitive))
		m_fileType	= MEDIA_TYPE_FLAC;
	else if(!fileInfo.suffix().compare("AAC", Qt::CaseInsensitive))
		m_fileType	= MEDIA_TYPE_MP4;
	else if(!fileInfo.suffix().compare("MP4", Qt::CaseInsensitive))
		m_fileType	= MEDIA_TYPE_MP4;
	else if(!fileInfo.suffix().compare("M4A", Qt::CaseInsensitive))
		m_fileType	= MEDIA_TYPE_MP4;
	else if(!fileInfo.suffix().compare("MPC", Qt::CaseInsensitive))
		m_fileType	= MEDIA_TYPE_MPC;
	else if(!fileInfo.suffix().compare("MP1", Qt::CaseInsensitive))
		m_fileType	= MEDIA_TYPE_MPEG;
	else if(!fileInfo.suffix().compare("MP2", Qt::CaseInsensitive))
		m_fileType	= MEDIA_TYPE_MPEG;
	else if(!fileInfo.suffix().compare("MP3", Qt::CaseInsensitive))
		m_fileType	= MEDIA_TYPE_MPEG;
	else if(!fileInfo.suffix().compare("TTA", Qt::CaseInsensitive))
		m_fileType	= MEDIA_TYPE_TRUEAUDIO;
	else if(!fileInfo.suffix().compare("WV", Qt::CaseInsensitive))
		m_fileType	= MEDIA_TYPE_WAVPACK;
	else if(!fileInfo.suffix().compare("WAV", Qt::CaseInsensitive))
		m_fileType	= MEDIA_TYPE_WAV;

	if(m_fileType	== MEDIA_TYPE_UNKNOWN)
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

	switch(m_fileType)
	{
	case MEDIA_TYPE_APE:
	{
		lpApe	= new APE::File(m_szFileName.toLocal8Bit().data());

		lpTagV1										= lpApe->ID3v1Tag();
		lpTagAPE									= lpApe->APETag();
		tags										= lpApe->properties();

		APE::Properties* lpAudioProperties			= lpApe->audioProperties();
		m_iLength									= lpAudioProperties->length();
		m_iBitrate									= lpAudioProperties->bitrate();
		m_iSampleRate								= lpAudioProperties->sampleRate();
		m_iChannels									= lpAudioProperties->channels();
		m_iBitsPerSample							= lpAudioProperties->bitsPerSample();
		m_iVersion									= lpAudioProperties->version();
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
		m_iLength									= lpAudioProperties->length();
		m_iBitrate									= lpAudioProperties->bitrate();
		m_iSampleRate								= lpAudioProperties->sampleRate();
		m_iChannels									= lpAudioProperties->channels();
		m_bIsEncrypted								= lpAudioProperties->isEncrypted();
		break;
	}
	case MEDIA_TYPE_FLAC:
	{
		lpFlac	= new FLAC::File(m_szFileName.toLocal8Bit().data());
		lpTagV1										= lpFlac->ID3v1Tag();
		lpTagV2										= lpFlac->ID3v2Tag();
		tags										= lpFlac->properties();

		FLAC::Properties* lpAudioProperties			= lpFlac->audioProperties();
		m_iLength									= lpAudioProperties->length();
		m_iBitrate									= lpAudioProperties->bitrate();
		m_iSampleRate								= lpAudioProperties->sampleRate();
		m_iChannels									= lpAudioProperties->channels();
		m_iSampleWidth								= lpAudioProperties->sampleWidth();
		m_ullSampleFrames							= lpAudioProperties->sampleFrames();
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
		m_iLength									= lpAudioProperties->length();
		m_iBitrate									= lpAudioProperties->bitrate();
		m_iSampleRate								= lpAudioProperties->sampleRate();
		m_iChannels									= lpAudioProperties->channels();
		m_iBitsPerSample							= lpAudioProperties->bitsPerSample();
		m_bIsEncrypted								= lpAudioProperties->isEncrypted();
		break;
	}
	case MEDIA_TYPE_MPC:
	{
		lpMPC	= new MPC::File(m_szFileName.toLocal8Bit().data());
		lpTagV1										= lpMPC->ID3v1Tag();
		lpTagAPE									= lpMPC->APETag();
		tags										= lpMPC->properties();

		MPC::Properties* lpAudioProperties			= lpMPC->audioProperties();
		m_iLength									= lpAudioProperties->length();
		m_iBitrate									= lpAudioProperties->bitrate();
		m_iSampleRate								= lpAudioProperties->sampleRate();
		m_iChannels									= lpAudioProperties->channels();
		m_iVersion									= lpAudioProperties->mpcVersion();
		m_ullSampleFrames							= lpAudioProperties->sampleFrames();
		m_iTrackGain								= lpAudioProperties->trackGain();
		m_iAlbumGain								= lpAudioProperties->albumGain();
		m_iTrackPeak								= lpAudioProperties->trackPeak();
		m_iAlbumPeak								= lpAudioProperties->albumPeak();
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
		m_iLength									= lpAudioProperties->length();
		m_iBitrate									= lpAudioProperties->bitrate();
		m_iSampleRate								= lpAudioProperties->sampleRate();
		m_iChannels									= lpAudioProperties->channels();
		m_iLayer									= lpAudioProperties->layer();
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
		m_bProtectionEnabled						= lpAudioProperties->protectionEnabled();
		switch(lpAudioProperties->channelMode())
		{
		case MPEG::Header::Stereo:
			m_channelMode							= CHANNEL_MODE_STEREO;
			break;
		case MPEG::Header::JointStereo:
			m_channelMode							= CHANNEL_MODE_JOINTSTEREO;
			break;
		case MPEG::Header::DualChannel:
			m_channelMode							= CHANNEL_MODE_DUALMONO;
			break;
		case MPEG::Header::SingleChannel:
			m_channelMode							= CHANNEL_MODE_MONO;
			break;
		}
		m_bIsCopyrighted							= lpAudioProperties->isCopyrighted();
		m_bIsOriginal								= lpAudioProperties->isOriginal();

		break;
	}
	case MEDIA_TYPE_TRUEAUDIO:
	{
		lpTrueAudio	= new TrueAudio::File(m_szFileName.toLocal8Bit().data());
		lpTagV1										= lpTrueAudio->ID3v1Tag();
		lpTagV2										= lpTrueAudio->ID3v2Tag();
		tags										= lpTrueAudio->properties();

		TrueAudio::Properties* lpAudioProperties	= lpTrueAudio->audioProperties();
		m_iLength									= lpAudioProperties->length();
		m_iBitrate									= lpAudioProperties->bitrate();
		m_iSampleRate								= lpAudioProperties->sampleRate();
		m_iChannels									= lpAudioProperties->channels();
		m_iBitsPerSample							= lpAudioProperties->bitsPerSample();
		m_iVersion									= lpAudioProperties->ttaVersion();
		break;
	}
	case MEDIA_TYPE_WAVPACK:
	{
		lpWavPack	= new WavPack::File(m_szFileName.toLocal8Bit().data());
		lpTagV1										= lpWavPack->ID3v1Tag();
		lpTagAPE									= lpWavPack->APETag();
		tags										= lpWavPack->properties();

		WavPack::Properties* lpAudioProperties		= lpWavPack->audioProperties();
		m_iLength									= lpAudioProperties->length();
		m_iBitrate									= lpAudioProperties->bitrate();
		m_iSampleRate								= lpAudioProperties->sampleRate();
		m_iChannels									= lpAudioProperties->channels();
		m_iBitsPerSample							= lpAudioProperties->bitsPerSample();
		m_ullSampleFrames							= lpAudioProperties->sampleFrames();
		m_iVersion									= lpAudioProperties->version();
		break;
	}
	case MEDIA_TYPE_WAV:
	{
		lpWav	= new RIFF::WAV::File(m_szFileName.toLocal8Bit().data());
		lpTagV2										= lpWav->tag();
		tags										= lpWav->properties();

		RIFF::WAV::Properties* lpAudioProperties	= lpWav->audioProperties();
		m_iLength									= lpAudioProperties->length();
		m_iBitrate									= lpAudioProperties->bitrate();
		m_iSampleRate								= lpAudioProperties->sampleRate();
		m_iChannels									= lpAudioProperties->channels();
		m_iSampleWidth								= lpAudioProperties->sampleWidth();
		m_ullSampleFrames							= lpAudioProperties->sampleFrames();
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

bool cMediaInfo::readFromDB()
{
	return(true);
}

qint32 cMediaInfo::writeFilename()
{
	QSqlQuery	query;

	query.prepare("SELECT id FROM file WHERE fileName=:fileName AND fileSize=:fileSize AND fileDate=:fileDate;");
	query.bindValue(":fileName", fileName());
	query.bindValue(":fileSize", fileSize());
	query.bindValue(":fileDate", fileDate());

	if(!query.exec())
	{
		myDebug << query.lastError().text();
		return(-1);
	}

	if(query.next())
		query.prepare("UPDATE file SET fileType1=:fileType1, fileType=:fileType, length1=:length1, length=:length, bitrate=:bitrate, sampleRate=:sampleRate, channels=:channels, bitsPerSample=:bitsPerSample, layer=:layer, version=:version, sampleWidth=:sampleWidth, sampleFrames=:sampleFrames, isEncrypted=:isEncrypted, trackGain=:trackGain, albumGain=:albumGain, trackPeak=:trackPeak, albumPeak=:albumPeak, protectionEnabled=:protectionEnabled, channelMode=:channelMode, isCopyrighted=:isCopyrighted, isOriginal=:isOriginal, album=:album, title=:title, copyright=:copyright, tracknumber=:tracknumber, contentGroupDescription=:contentGroupDescription, subTitle=:subTitle, originalAlbum=:originalAlbum, partOfSet=:partOfSet, subTitleOfSet=:subTitleOfSet, internationalStandardRecordingCode=:internationalStandardRecordingCode, leadArtist=:leadArtist, band=:band, conductor=:conductor, interpret=:interpret, originalArtist=:originalArtist, textWriter=:textWriter, originalTextWriter=:originalTextWriter, composer=:composer, encodedBy=:encodedBy, beatsPerMinute=:beatsPerMinute, language=:language, contentType=:contentType, mediaType=:mediaType, mood=:mood, producedNotice=:producedNotice, publisher=:publisher, fileOwner=:fileOwner, internetRadioStationName=:internetRadioStationName, internetRadioStationOwner=:internetRadioStationOwner, originalFilename=:originalFilename, playlistDelay=:playlistDelay, encodingTime=:encodingTime, originalReleaseTime=:originalReleaseTime, recordingTime=:recordingTime, releaseTime=:releaseTime, taggingTime=:taggingTime, swhwSettings=:swhwSettings, albumSortOrder=:albumSortOrder, performerSortOrder=:performerSortOrder, titleSortOrder=:titleSortOrder, synchronizedLyrics=:synchronizedLyrics, unsynchronizedLyrics=:unsynchronizedLyrics WHERE filename=:filename AND filesize=:filesize AND filedate=:filedate;");
	else
		query.prepare("INSERT INTO file (fileName, fileSize, fileDate, fileType1, fileType, length1, length, bitrate, sampleRate, channels, bitsPerSample, layer, version, sampleWidth, sampleFrames, isEncrypted, trackGain, albumGain, trackPeak, albumPeak, protectionEnabled, channelMode, isCopyrighted, isOriginal, album, title, copyright, tracknumber, contentGroupDescription, subTitle, originalAlbum, partOfSet, subTitleOfSet, internationalStandardRecordingCode, leadArtist, band, conductor, interpret, originalArtist, textWriter, originalTextWriter, composer, encodedBy, beatsPerMinute, language, contentType, mediaType, mood, producedNotice, publisher, fileOwner, internetRadioStationName, internetRadioStationOwner, originalFilename, playlistDelay, encodingTime, originalReleaseTime, recordingTime, releaseTime, taggingTime, swhwSettings, albumSortOrder, performerSortOrder, titleSortOrder, synchronizedLyrics, unsynchronizedLyrics) VALUES (:fileName, :fileSize, :fileDate, :fileType1, :fileType, :length1, :length, :bitrate, :sampleRate, :channels, :bitsPerSample, :layer, :version, :sampleWidth, :sampleFrames, :isEncrypted, :trackGain, :albumGain, :trackPeak, :albumPeak, :protectionEnabled, :channelMode, :isCopyrighted, :isOriginal, :album, :title, :copyright, :tracknumber, :contentGroupDescription, :subTitle, :originalAlbum, :partOfSet, :subTitleOfSet, :internationalStandardRecordingCode, :leadArtist, :band, :conductor, :interpret, :originalArtist, :textWriter, :originalTextWriter, :composer, :encodedBy, :beatsPerMinute, :language, :contentType, :mediaType, :mood, :producedNotice, :publisher, :fileOwner, :internetRadioStationName, :internetRadioStationOwner, :originalFilename, :playlistDelay, :encodingTime, :originalReleaseTime, :recordingTime, :releaseTime, :taggingTime, :swhwSettings, :albumSortOrder, :performerSortOrder, :titleSortOrder, :synchronizedLyrics, :unsynchronizedLyrics);");

	query.bindValue(":fileName", fileName());
	query.bindValue(":fileSize", fileSize());
	query.bindValue(":fileDate", fileDate());
	query.bindValue(":fileType1", fileType1());
	query.bindValue(":fileType", fileType());
	query.bindValue(":length1", length1());
	query.bindValue(":length", length());
	query.bindValue(":bitrate", bitrate());
	query.bindValue(":sampleRate", sampleRate());
	query.bindValue(":channels", channels());
	query.bindValue(":bitsPerSample", bitsPerSample());
	query.bindValue(":layer", layer());
	query.bindValue(":version", version());
	query.bindValue(":sampleWidth", sampleWidth());
	query.bindValue(":sampleFrames", sampleFrames());
	query.bindValue(":isEncrypted", isEncrypted());
	query.bindValue(":trackGain", trackGain());
	query.bindValue(":albumGain", albumGain());
	query.bindValue(":trackPeak", trackPeak());
	query.bindValue(":albumPeak", albumPeak());
	query.bindValue(":protectionEnabled", protectionEnabled());
	query.bindValue(":channelMode", channelMode());
	query.bindValue(":isCopyrighted", isCopyrighted());
	query.bindValue(":isOriginal", isOriginal());
	query.bindValue(":album", album());
	query.bindValue(":title", title());
	query.bindValue(":copyright", copyright());
	query.bindValue(":tracknumber", trackNumber());
	query.bindValue(":contentGroupDescription", contentGroupDescription());
	query.bindValue(":subTitle", subTitle());
	query.bindValue(":originalAlbum", originalAlbum());
	query.bindValue(":partOfSet", partOfSet());
	query.bindValue(":subTitleOfSet", subTitleOfSet());
	query.bindValue(":internationalStandardRecordingCode", internationalStandardRecordingCode());
	query.bindValue(":leadArtist", leadArtist());
	query.bindValue(":band", band());
	query.bindValue(":conductor", conductor());
	query.bindValue(":interpret", interpret().join(", "));
	query.bindValue(":originalArtist", originalArtist());
	query.bindValue(":textWriter", textWriter());
	query.bindValue(":originalTextWriter", originalTextWriter());
	query.bindValue(":composer", composer());
	query.bindValue(":encodedBy", encodedBy());
	query.bindValue(":beatsPerMinute", beatsPerMinute());
	query.bindValue(":language", language().join(", "));
	query.bindValue(":contentType", contentType().join(", "));
	query.bindValue(":mediaType", mediaType().join(", "));
	query.bindValue(":mood", mood());
	query.bindValue(":producedNotice", producedNotice());
	query.bindValue(":publisher", publisher());
	query.bindValue(":fileOwner", fileOwner());
	query.bindValue(":internetRadioStationName", internetRadioStationName());
	query.bindValue(":internetRadioStationOwner", internetRadioStationOwner());
	query.bindValue(":originalFilename", originalFilename());
	query.bindValue(":playlistDelay", playlistDelay());
	query.bindValue(":encodingTime", encodingTime());
	query.bindValue(":originalReleaseTime", originalReleaseTime());
	query.bindValue(":recordingTime", recordingTime());
	query.bindValue(":releaseTime", releaseTime());
	query.bindValue(":taggingTime", taggingTime());
	query.bindValue(":swhwSettings", swhwSettings().join(", "));
	query.bindValue(":albumSortOrder", albumSortOrder());
	query.bindValue(":performerSortOrder", performerSortOrder());
	query.bindValue(":titleSortOrder", titleSortOrder());
	query.bindValue(":synchronizedLyrics", synchronizedLyrics().join());
	query.bindValue(":unsynchronizedLyrics", unsynchronizedLyrics().join("||"));

	if(!query.exec())
	{
		myDebug << query.lastError().text();
		return(-1);
	}

	query.prepare("SELECT id FROM file WHERE fileName=:fileName AND fileSize=:fileSize AND fileDate=:fileDate;");
	query.bindValue(":fileName", fileName());
	query.bindValue(":fileSize", fileSize());
	query.bindValue(":fileDate", fileDate());

	if(!query.exec())
	{
		myDebug << query.lastError().text();
		return(-1);
	}

	if(query.next())
		return(query.value("id").toInt());

	return(-1);

//	cImageList		images();
}

bool cMediaInfo::writeToDB()
{
	qint32		idFile	= writeFilename();

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
			m_encodingTime							= str2TS((*it)->toString().toCString());
		else if(!szID.compare("TDOR"))
			m_originalReleaseTime					= str2TS((*it)->toString().toCString());
		else if(!szID.compare("TDRC"))
			m_recordingTime							= str2TS((*it)->toString().toCString());
		else if(!szID.compare("TDRL"))
			m_releaseTime							= str2TS((*it)->toString().toCString());
		else if(!szID.compare("TDTG"))
			m_taggingTime							= str2TS((*it)->toString().toCString());
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
//			QPixmap		pixmap;
//			pixmap.loadFromData(pictureData);
//			m_imageList.add(pictureData, m_szFileName, (cImage::ImageType)t, szDescription);
			QImage		image		= QImage::fromData(pictureData);
			m_images.append(image);
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

qint64 cMediaInfo::fileSize()
{
	QFileInfo	fileInfo(m_szFileName);
	return(fileInfo.size());
}

QDateTime cMediaInfo::fileDate()
{
	QFileInfo	fileInfo(m_szFileName);
	return(fileInfo.created());
}

cMediaInfo::MEDIA_TYPE cMediaInfo::fileType1()
{
	return(m_fileType);
}

QString cMediaInfo::fileType1Text()
{
	switch(m_fileType)
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
	return(m_encodingTime);
}

QDateTime cMediaInfo::originalReleaseTime()
{
	return(m_originalReleaseTime);
}

QDateTime cMediaInfo::recordingTime()
{
	return(m_recordingTime);
}

QDateTime cMediaInfo::releaseTime()
{
	return(m_releaseTime);
}

QDateTime cMediaInfo::taggingTime()
{
	return(m_taggingTime);
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
