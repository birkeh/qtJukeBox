#include <QMessageBox>
#include <QFileInfo>
#include <string>
#include <stdio.h>

#include "cmediainfo.h"
#include "fields.h"


//#define SORT_WHOLE_LIST


extern "C" {
#include "libmpg123/mpg123.h"
}


cMediaInfo::cMediaInfo(QObject *parent) :
	QObject(parent),
	m_szFileName(""),
	m_FileCreated(QDateTime()),
	m_FileModified(QDateTime()),
	m_dwFileSize(0),
	m_dwBitrate(0),
	m_dwSamplerate(0),
	m_dwChannels(0),
	m_dwSeconds(0)
{
	initFields();
}

cMediaInfo::~cMediaInfo()
{
	for(int x = 0;x < m_ID3FieldList.count();x++)
		delete (m_ID3FieldList.at(x));
}

void cMediaInfo::clear()
{
	m_szFileName	= "";
	//m_ID3FieldList.clear();
	m_dwBitrate		= 0;
	m_dwSamplerate	= 0;
	m_dwChannels	= 0;
	m_dwSeconds		= 0;
}

uint16_t cMediaInfo::tagID(char* lpszTagName)
{
	cID3Field*	lpID3Field	= m_ID3FieldList.get(QString(lpszTagName));
	if(lpID3Field)
		return(lpID3Field->getID());
	return(cID3Field::TAG_MAXFIELDS);
}

QString cMediaInfo::tag2String(struct id3_frame* lpID3Frame, uint16_t dwField)
{
	QString	szRet("");

	if(id3_field_getnstrings(&lpID3Frame->fields[dwField]))
	{
		const id3_ucs4_t*	lpszUCS4;
		const id3_latin1_t*	lpszLatin1;

		lpszUCS4		= id3_field_getstrings(&lpID3Frame->fields[dwField], 0);
		if(lpszUCS4)
		{
			lpszLatin1	= id3_ucs4_latin1duplicate(lpszUCS4);
			szRet		= QString((const char*)lpszLatin1);
			//free((void*)lpszUCS4);
			free((void*)lpszLatin1);
		}
	}
	return(szRet);
}

QString cMediaInfo::tag2LyricsString(struct id3_frame* lpID3Frame)
{
	QString	szRet("");

	const id3_ucs4_t*	lpszUCS4;
	const id3_latin1_t*	lpszLatin1;

	if(lpID3Frame->fields[1].immediate.value[0] == 'X' &&
	   lpID3Frame->fields[1].immediate.value[1] == 'X' &&
	   lpID3Frame->fields[1].immediate.value[2] == 'X')
	{
		lpszUCS4	= id3_field_getfullstring(&lpID3Frame->fields[3]);
		if(lpszUCS4)
		{
			lpszLatin1	= id3_ucs4_latin1duplicate(lpszUCS4);
			szRet		= QString((const char*)lpszLatin1);
			free((void*)lpszLatin1);
		}
	}

	return(szRet);
}

void cMediaInfo::importInformation(struct id3_tag* lpID3Tag)
{
	struct id3_frame*	lpID3Frame;
	int					iIndex	= 0;
	uint16_t			dwID;
	QString				sz;

	while((lpID3Frame	= id3_tag_findframe(lpID3Tag, NULL, iIndex)) != NULL)
	{
		dwID	= tagID(lpID3Frame->id);
		switch(dwID)
		{
		case cID3Field::TAG_Album:
		case cID3Field::TAG_BeatsPerMinute:
		case cID3Field::TAG_Composer:
		case cID3Field::TAG_ContentType:
		case cID3Field::TAG_CopyrightMessage:
		case cID3Field::TAG_PlaylistDelay:
		case cID3Field::TAG_OriginalReleaseTime:
		case cID3Field::TAG_EncodingTime:
		case cID3Field::TAG_RecordingTime:
		case cID3Field::TAG_ReleaseTime:
		case cID3Field::TAG_TaggingTime:
		case cID3Field::TAG_EncodedBy:
		case cID3Field::TAG_Lyricist:
		case cID3Field::TAG_FileType:
		case cID3Field::TAG_InvolvedPeopleList:
		case cID3Field::TAG_ContentGroupDescription:
		case cID3Field::TAG_Title:
		case cID3Field::TAG_Subtitle:
		case cID3Field::TAG_InitialKey:
		case cID3Field::TAG_Languages:
		case cID3Field::TAG_Length:
		case cID3Field::TAG_MusicianCreditsList:
		case cID3Field::TAG_MediaType:
		case cID3Field::TAG_Mood:
		case cID3Field::TAG_OriginalAlbum:
		case cID3Field::TAG_OriginalFileName:
		case cID3Field::TAG_OriginalLyricist:
		case cID3Field::TAG_OriginalArtist:
		case cID3Field::TAG_FileOwner:
		case cID3Field::TAG_LeadPerformer:
		case cID3Field::TAG_Band:
		case cID3Field::TAG_Conductor:
		case cID3Field::TAG_InterpretedBy:
		case cID3Field::TAG_PartOfASet:
		case cID3Field::TAG_ProducedNotice:
		case cID3Field::TAG_Publisher:
		case cID3Field::TAG_TrackNumber:
		case cID3Field::TAG_InternetRadioStationName:
		case cID3Field::TAG_InternetRadioStationOwner:
		case cID3Field::TAG_AlbumSortOrder:
		case cID3Field::TAG_PerformerSortOrder:
		case cID3Field::TAG_TitleSortOrder:
		case cID3Field::TAG_InternationalStandardRecordingCode:
		case cID3Field::TAG_Software_Hardware:
		case cID3Field::TAG_SetSubtitle:
		case cID3Field::TAG_UserDefinedTextInformation:
		case cID3Field::TAG_UniqueFileIdentifier:
		case cID3Field::TAG_TermsOfUse:
			m_ID3FieldList.get(dwID)->setValue(QVariant(tag2String(lpID3Frame, 1)));	break;
		case cID3Field::TAG_UnsynchronizedLyric:
			sz = tag2LyricsString(lpID3Frame);
			if(sz.length())
				m_ID3FieldList.get(dwID)->setValue(QVariant(sz));	break;
			break;
		case cID3Field::TAG_AttachedPicture:
#ifdef WITH_PICTURE
			{
				uint16_t			dwType			= id3_field_getint(&lpID3Frame->fields[2]);
				QString				szDescription	= tag2String(lpID3Frame, 3);
				id3_length_t		dwLen;
				const id3_byte_t*	lpuData	= id3_field_getbinarydata(&lpID3Frame->fields[4], &dwLen);

				QPixmap		Pixmap;
				Pixmap.loadFromData(lpuData, dwLen);
				QVariant	v	= m_ID3FieldList.get(dwID)->getValue();
				if(v.canConvert<cPictureList*>())
				{
					cPictureList*	lpPictureList	= v.value<cPictureList*>();
					lpPictureList->add(Pixmap, dwType, szDescription);
				}
				else
				{
					cPictureList*	lpPictureList	= new cPictureList;
					lpPictureList->add(Pixmap, dwType, szDescription);
					v.setValue(lpPictureList);
					m_ID3FieldList.setValue(dwID, v);
				}
			}
#endif
			break;
		case cID3Field::TAG_AudioEncryption:
			break;
		case cID3Field::TAG_AudioSeekPointIndex:
			break;
		case cID3Field::TAG_Comments:
			break;
		case cID3Field::TAG_CommercialFrame:
			break;
		case cID3Field::TAG_EncryptionMethodRegistration:
			break;
		case cID3Field::TAG_Equalisation:
			break;
		case cID3Field::TAG_EventTimingCodes:
			break;
		case cID3Field::TAG_GeneralEncapsulatedObject:
			break;
		case cID3Field::TAG_GroupIdentificationRegistration:
			break;
		case cID3Field::TAG_LinkedInformation:
			break;
		case cID3Field::TAG_MusicCDIdentifier:
			break;
		case cID3Field::TAG_MPEGLocationLookupTable:
			break;
		case cID3Field::TAG_OwnershipFrame:
			break;
		case cID3Field::TAG_PrivateFrame:
			break;
		case cID3Field::TAG_PlayCounter:
			break;
		case cID3Field::TAG_Popularimeter:
			break;
		case cID3Field::TAG_PositionSynchronisationFrame:
			break;
		case cID3Field::TAG_RecommendedBufferSize:
			break;
		case cID3Field::TAG_RelativeVolumeAdjustment:
			break;
		case cID3Field::TAG_Reverb:
			break;
		case cID3Field::TAG_SeekFrame:
			break;
		case cID3Field::TAG_SignatureFrame:
			break;
		case cID3Field::TAG_SynchronisedLyric:
			break;
		case cID3Field::TAG_SynchronisedTempoCodes:
			break;
		case cID3Field::TAG_CommercialInformation:
			break;
		case cID3Field::TAG_LegalInformation:
			break;
		case cID3Field::TAG_OfficialAudioFileWebpage:
			break;
		case cID3Field::TAG_OfficialArtistWebpage:
			break;
		case cID3Field::TAG_OfficialAudioSourceWebpage:
			break;
		case cID3Field::TAG_OfficialInternetRadioStationWebpage:
			break;
		case cID3Field::TAG_Payment:
			break;
		case cID3Field::TAG_PublishersOfficialWebpage:
			break;
		case cID3Field::TAG_UserDefinedURLLinkFrame:
			break;
		}

		iIndex++;
	}
}

void cMediaInfo::readFileInformation(const QString& szFileName)
{
	FILE*	file	= fopen(szFileName.toLatin1(), "rb");
	if(!file)
		return;

	uint32_t		head;
	unsigned char	tmp[4];
	struct frame	frm;
	bool			id3_found	= false;


	if(fread(tmp, 1, 4, file) != 4)
	{
		fclose(file);
		return;
	}

	// Skip data of the ID3v2.x tag (It may contain data similar to mpeg frame
	// as, for example, in the id3 APIC frame) (patch from Artur Polaczynski)
	if (tmp[0] == 'I' && tmp[1] == 'D' && tmp[2] == '3' && tmp[3] < 0xFF)
	{
		// ID3v2 tag skipeer $49 44 33 yy yy xx zz zz zz zz [zz size]
		long id3v2size;
		fseek(file, 2, SEEK_CUR); // Size is 6-9 position
		if (fread(tmp, 1, 4, file) != 4) // Read bytes of tag size
		{
			fclose(file);
			return;
		}
		id3v2size = 10 + ( (long)(tmp[3]) | ((long)(tmp[2]) << 7) | ((long)(tmp[1]) << 14) | ((long)(tmp[0]) << 21) );
		fseek(file, id3v2size, SEEK_SET);
		if (fread(tmp, 1, 4, file) != 4) // Read mpeg header
		{
			fclose(file);
			return;
		}
	}

	head = ((uint32_t) tmp[0] << 24) | ((uint32_t) tmp[1] << 16) | ((uint32_t) tmp[2] << 8) | (uint32_t) tmp[3];
	while (!mpg123_head_check(head))
	{
		head <<= 8;
		if (fread(tmp, 1, 1, file) != 1)
		{
			fclose(file);
			return;
		}
		head |= tmp[0];
	}

	if (mpg123_decode_header(&frm, head))
	{
		unsigned char*	buf;
		double			tpf;
		int				pos;
		XHEADDATA		xing_header;
		uint32_t		num_frames;

		buf = (unsigned char*)malloc(frm.framesize + 4);
		fseek(file, -4, SEEK_CUR);
		fread(buf, 1, frm.framesize + 4, file);
		xing_header.toc = NULL;
		tpf = mpg123_compute_tpf(&frm);
		// MPEG and Layer version
		setMPEG(frm.mpeg25);
		setVersion(frm.lsf+1);
		setLayerVersion(frm.lay);

		pos = ftell(file);
		fseek(file, 0, SEEK_END);
		// Variable bitrate? + bitrate
		uint16_t	dwVarBitrate	= mpg123_get_xing_header(&xing_header,buf);
		if(dwVarBitrate)
		{
			num_frames = xing_header.frames;
			setBitrate(((xing_header.bytes * 8) / (tpf * xing_header.frames * 1000)));
		}
		else
		{
			num_frames = ((ftell(file) - pos - (id3_found ? 128 : 0)) / mpg123_compute_bpf(&frm)) + 1;
			setBitrate(tabsel_123[frm.lsf][frm.lay - 1][frm.bitrate_index]);
		}
		// Samplerate
		setSamplerate(mpg123_freqs[frm.sampling_frequency]);
		// Mode
		setMode(frm.mode);

		free(buf);
	}

	// Duration
	setSeconds(mpg123_get_song_time(file)/1000);

	fclose(file);
}

void cMediaInfo::importFromFile(const QString& szFileName)
{
	clear();

	struct id3_file*	lpID3File;
	struct id3_tag*		lpID3Tag;

	readFileInformation(szFileName);

	lpID3File	= id3_file_open(szFileName.toUtf8(), ID3_FILE_MODE_READONLY);
	if(!lpID3File)
		return;

	m_szFileName	= szFileName;
	lpID3Tag		= id3_file_tag(lpID3File);

	importInformation(lpID3Tag);

	id3_file_close(lpID3File);
}

void cMediaInfo::initFields()
{
	for(int z = 0;z < cID3Field::TAG_MAXFIELDS;z++)
		m_ID3FieldList.add(g_Fields[z].dwID, g_Fields[z].lpszShortName, g_Fields[z].lpszName);
}

void cMediaInfo::setFileName(const QString& szFileName)
{
	m_szFileName	= szFileName;
}

QString cMediaInfo::getFileName()
{
	return(m_szFileName);
}

void cMediaInfo::setFileCreated(const QDateTime& FileCreated)
{
	m_FileCreated	= FileCreated;
}

QDateTime cMediaInfo::getFileCreated()
{
	return(m_FileCreated);
}

void cMediaInfo::setFileModified(const QDateTime& FileModified)
{
	m_FileModified	= FileModified;
}

QDateTime cMediaInfo::getFileModified()
{
	return(m_FileModified);
}

void cMediaInfo::setFileSize(uint64_t dwFileSize)
{
	m_dwFileSize	= dwFileSize;
}

uint64_t cMediaInfo::getFileSize()
{
	return(m_dwFileSize);
}

QString cMediaInfo::getTAGName(uint16_t dwTag)
{
	if(dwTag >= cID3Field::TAG_MAXFIELDS)
		return("");
	return(m_ID3FieldList.getName(dwTag));
}

QString cMediaInfo::getTAGShortName(uint16_t dwTag)
{
	if(dwTag >= cID3Field::TAG_MAXFIELDS)
		return("");
	return(m_ID3FieldList.getShortName(dwTag));
}

void cMediaInfo::setBitrate(uint16_t dwBitrate)
{
	m_dwBitrate	= dwBitrate;
}

uint16_t cMediaInfo::getBitrate()
{
	return(m_dwBitrate);
}

void cMediaInfo::setSamplerate(uint16_t dwSamplerate)
{
	m_dwSamplerate	= dwSamplerate;
}

uint16_t cMediaInfo::getSamplerate()
{
	return(m_dwSamplerate);
}

void cMediaInfo::setChannels(uint16_t dwChannels)
{
	m_dwChannels	= dwChannels;
}

uint16_t cMediaInfo::getChannels()
{
	return(m_dwChannels);
}

void cMediaInfo::setSeconds(uint16_t dwSeconds)
{
	m_dwSeconds		= dwSeconds;
}

uint16_t cMediaInfo::getSeconds()
{
	return(m_dwSeconds);
}

void cMediaInfo::setMPEG(uint16_t dwMPEG)
{
	m_dwMPEG		= dwMPEG;
}

uint16_t cMediaInfo::getMPEG()
{
	return(m_dwMPEG);
}

void cMediaInfo::setVersion(uint16_t dwVersion)
{
	m_dwVersion		= dwVersion;
}

uint16_t cMediaInfo::getVersion()
{
	return(m_dwVersion);
}

void cMediaInfo::setLayerVersion(uint16_t dwLayerVersion)
{
	m_dwLayerVersion	= dwLayerVersion;
}

uint16_t cMediaInfo::getLayerVersion()
{
	return(m_dwLayerVersion);
}

void cMediaInfo::setMode(uint16_t dwMode)
{
	m_dwMode		= dwMode;
}

uint16_t cMediaInfo::getMode()
{
	return(m_dwMode);
}

void cMediaInfo::setValid(bool bValid)
{
	m_bValid		= bValid;
}

bool cMediaInfo::getValid()
{
	return(m_bValid);
}

void cMediaInfo::setTAG(uint16_t dwTag, uint16_t dwData)
{
	if(dwTag >= cID3Field::TAG_MAXFIELDS)
		return;

	m_ID3FieldList.setValue(dwTag, QVariant(dwData));
}

void cMediaInfo::setTAG(uint16_t dwTag, const QString& szData)
{
	if(dwTag >= cID3Field::TAG_MAXFIELDS)
		return;
	m_ID3FieldList.setValue(dwTag, QVariant(szData));
}

void cMediaInfo::setTAG(uint16_t dwTag, const QVariant& v)
{
	if(dwTag >= cID3Field::TAG_MAXFIELDS)
		return;
	m_ID3FieldList.setValue(dwTag, v);
}

QVariant cMediaInfo::getTAG(uint16_t dwTag)
{
	if(dwTag >= cID3Field::TAG_MAXFIELDS)
		return(QVariant());
	if(!m_ID3FieldList.getValue(dwTag).isValid())
		return(QVariant());
	return(m_ID3FieldList.getValue(dwTag));
}

uint16_t cMediaInfo::getTAGi(uint16_t dwTag)
{
	if(dwTag >= cID3Field::TAG_MAXFIELDS)
		return(0);
	if(!m_ID3FieldList.getValue(dwTag).isValid())
		return(0);
	return(m_ID3FieldList.getValue(dwTag).toInt());
}

QString cMediaInfo::getTAGs(uint16_t dwTag)
{
	if(dwTag >= cID3Field::TAG_MAXFIELDS)
		return(QString(""));
	if(!m_ID3FieldList.isValid(dwTag))
		return(QString(""));
	return(m_ID3FieldList.getValue(dwTag).toString());
}

bool cMediaInfo::getTAG(uint16_t dwTag, QString& szData)
{
	if(dwTag >= cID3Field::TAG_MAXFIELDS)
		return(false);
	if(!m_ID3FieldList.isValid(dwTag))
		return(false);
	szData	= m_ID3FieldList.getValue(dwTag).toString();
	return(true);
}

bool cMediaInfo::getTAG(uint16_t dwTag, QString& szName, QString& szData)
{
	if(dwTag >= cID3Field::TAG_MAXFIELDS)
		return(false);
	if(!m_ID3FieldList.isValid(dwTag))
		return(false);
	szName	= m_ID3FieldList.getName(dwTag);
	szData	= m_ID3FieldList.getValue(dwTag).toString();
	return(true);
}

bool cMediaInfo::getTAG(uint16_t dwTag, uint16_t& dwData)
{
	if(dwTag >= cID3Field::TAG_MAXFIELDS)
		return(false);
	if(!m_ID3FieldList.isValid(dwTag))
		return(false);
	dwData	= m_ID3FieldList.getValue(dwTag).toInt();
	return(true);
}

bool cMediaInfo::getTAG(uint16_t dwTag, QString& szName, uint16_t& dwData)
{
	if(dwTag >= cID3Field::TAG_MAXFIELDS)
		return(false);
	if(!m_ID3FieldList.isValid(dwTag))
		return(false);
	szName	= m_ID3FieldList.getName(dwTag);
	dwData	= m_ID3FieldList.getValue(dwTag).toInt();
	return(true);
}

bool cMediaInfo::getTAG(uint16_t dwTag, id3_byte_t* lpData, uint32_t& dwLen)
{
	if(dwTag >= cID3Field::TAG_MAXFIELDS)
		return(false);
	if(!m_ID3FieldList.isValid(dwTag))
		return(false);
	lpData	= (id3_byte_t*)m_ID3FieldList.getValue(dwTag).toByteArray().data();
	dwLen	= m_ID3FieldList.getValue(dwTag).toByteArray().length();
	return(true);
}

bool cMediaInfo::getTAG(uint16_t dwTag, QString& szName, id3_byte_t* lpData, uint32_t& dwLen)
{
	if(dwTag >= cID3Field::TAG_MAXFIELDS)
		return(false);
	if(!m_ID3FieldList.isValid(dwTag))
		return(false);
	szName	= m_ID3FieldList.getName(dwTag);
	lpData	= (id3_byte_t*)m_ID3FieldList.getValue(dwTag).toByteArray().data();
	dwLen	= m_ID3FieldList.getValue(dwTag).toByteArray().length();
	return(true);
}

bool cMediaInfo::getTAG(uint16_t dwTag, QPixmap& Pixmap)
{
	if(dwTag >= cID3Field::TAG_MAXFIELDS)
		return(false);
	if(!m_ID3FieldList.isValid(dwTag))
		return(false);

	QVariant	v	= m_ID3FieldList.getValue(dwTag);
	if(v.canConvert<QPixmap>())
	{
		Pixmap	= v.value<QPixmap>();
		return(true);
	}
	return(false);
}

bool cMediaInfo::getTAG(uint16_t dwTag, QString& szName, QPixmap& Pixmap)
{
	if(dwTag >= cID3Field::TAG_MAXFIELDS)
		return(false);
	if(!m_ID3FieldList.isValid(dwTag))
		return(false);
	szName	= m_ID3FieldList.getName(dwTag);

	QVariant	v	= m_ID3FieldList.getValue(dwTag);
	if(v.canConvert<QPixmap>())
	{
		Pixmap	= v.value<QPixmap>();
		return(true);
	}
	return(false);
}

bool cMediaInfo::getTAG(uint16_t dwTag, cPictureList& PictureList)
{
	if(dwTag >= cID3Field::TAG_MAXFIELDS)
		return(false);
	if(!m_ID3FieldList.isValid(dwTag))
		return(false);

	QVariant	v	= m_ID3FieldList.getValue(dwTag);
	if(v.canConvert<cPictureList*>())
	{
		cPictureList*	lpPictureList	= v.value<cPictureList*>();
		PictureList	= *lpPictureList;
		return(true);
	}
	return(false);
}

bool cMediaInfo::getTAG(uint16_t dwTag, QString& szName, cPictureList& PictureList)
{
	if(dwTag >= cID3Field::TAG_MAXFIELDS)
		return(false);
	if(!m_ID3FieldList.isValid(dwTag))
		return(false);
	szName	= m_ID3FieldList.getName(dwTag);

	QVariant	v	= m_ID3FieldList.getValue(dwTag);
	if(v.canConvert<cPictureList*>())
	{
		cPictureList*	lpPictureList	= v.value<cPictureList*>();
		PictureList	= *lpPictureList;
		return(true);
	}
	return(false);
}

uint16_t cMediaInfo::tagIDFromShortName(const QString& szTAG)
{
	for(int z = 0;z < cID3Field::TAG_MAXFIELDS;z++)
	{
		if(!szTAG.compare(g_Fields[z].lpszShortName, Qt::CaseInsensitive))
			return(z);
	}
	return(cID3Field::TAG_MAXFIELDS);
}

cMediaInfoList::cMediaInfoList()
{
}

cMediaInfo* cMediaInfoList::add(const QString& szFileName, QObject *parent)
{
	cMediaInfo*	lpMediaInfo	= this->get(szFileName);
	if(lpMediaInfo)
		return(lpMediaInfo);

	lpMediaInfo	= new cMediaInfo(parent);
	lpMediaInfo->setFileName(szFileName);
	this->append(lpMediaInfo);
	return(lpMediaInfo);
}

cMediaInfo* cMediaInfoList::add(const QString& szFileName, cMediaInfo* lpMediaInfo, QObject *parent)
{
	parent	= parent;

	cMediaInfo*	lpMediaInfo1	= this->get(szFileName);
	if(lpMediaInfo1)
		return(lpMediaInfo1);

	lpMediaInfo->setFileName(szFileName);
	QFileInfo	FileInfo(szFileName);
	lpMediaInfo->setFileCreated(FileInfo.created());
	lpMediaInfo->setFileModified(FileInfo.lastModified());
	lpMediaInfo->setFileSize(FileInfo.size());
	lpMediaInfo->setValid(true);

	this->append(lpMediaInfo);

	return(lpMediaInfo);
}

cMediaInfo* cMediaInfoList::get(const QString& szFileName)
{
	int	iIndex;

	for(iIndex = 0;iIndex < this->count();iIndex++)
	{
		if(!this->at(iIndex)->getFileName().compare(szFileName, Qt::CaseInsensitive))
			return(this->at(iIndex));
	}
	return(0);
}
