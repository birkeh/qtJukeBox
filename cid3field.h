#ifndef cID3Field_H
#define cID3Field_H

#include <QString>
#include <QVariant>
#include <QPixmap>
#include <stdint.h>

#include "cpicture.h"


class cID3Field : public QObject
{
public:
	enum TAG
	{
		TAG_AudioEncryption,			TAG_AttachedPicture,						TAG_AudioSeekPointIndex,		TAG_Comments,							TAG_CommercialFrame,			TAG_EncryptionMethodRegistration,
		TAG_Equalisation,				TAG_EventTimingCodes,						TAG_GeneralEncapsulatedObject,	TAG_GroupIdentificationRegistration,	TAG_LinkedInformation,			TAG_MusicCDIdentifier,
		TAG_MPEGLocationLookupTable,	TAG_OwnershipFrame,							TAG_PrivateFrame,				TAG_PlayCounter,						TAG_Popularimeter,				TAG_PositionSynchronisationFrame,
		TAG_RecommendedBufferSize,		TAG_RelativeVolumeAdjustment,				TAG_Reverb,						TAG_SeekFrame,							TAG_SignatureFrame,				TAG_SynchronisedLyric,
		TAG_SynchronisedTempoCodes,		TAG_Album,									TAG_BeatsPerMinute,				TAG_Composer,							TAG_ContentType,				TAG_CopyrightMessage,
		TAG_EncodingTime,				TAG_PlaylistDelay,							TAG_OriginalReleaseTime,		TAG_RecordingTime,						TAG_ReleaseTime,				TAG_TaggingTime,
		TAG_EncodedBy,					TAG_Lyricist,								TAG_FileType,					TAG_InvolvedPeopleList,					TAG_ContentGroupDescription,	TAG_Title,
		TAG_Subtitle,					TAG_InitialKey,								TAG_Languages,					TAG_Length,								TAG_MusicianCreditsList,		TAG_MediaType,
		TAG_Mood,						TAG_OriginalAlbum,							TAG_OriginalFileName,			TAG_OriginalLyricist,					TAG_OriginalArtist,				TAG_FileOwner,
		TAG_LeadPerformer,				TAG_Band,									TAG_Conductor,					TAG_InterpretedBy,						TAG_PartOfASet,					TAG_ProducedNotice,
		TAG_Publisher,					TAG_TrackNumber,							TAG_InternetRadioStationName,	TAG_InternetRadioStationOwner,			TAG_AlbumSortOrder,				TAG_PerformerSortOrder,
		TAG_TitleSortOrder,				TAG_InternationalStandardRecordingCode,		TAG_Software_Hardware,			TAG_SetSubtitle,						TAG_UserDefinedTextInformation,	TAG_UniqueFileIdentifier,
		TAG_TermsOfUse,					TAG_UnsynchronizedLyric,					TAG_CommercialInformation,		TAG_LegalInformation,					TAG_OfficialAudioFileWebpage,	TAG_OfficialArtistWebpage,
		TAG_OfficialAudioSourceWebpage,	TAG_OfficialInternetRadioStationWebpage,	TAG_Payment,					TAG_PublishersOfficialWebpage,			TAG_UserDefinedURLLinkFrame,	TAG_MAXFIELDS,
	};

	explicit		cID3Field(uint16_t dwID = 0, const QString& szShortName = "", const QString& szName = "");
	~cID3Field();

	uint16_t		getID();

	void			setShortName(const QString& szShortName);
	QString			getShortName();

	void			setName(const QString& szName);
	QString			getName();

	void			setValue(const QVariant& Value);
	QVariant		getValue();

	bool			isValid();

	cID3Field&		operator=(const cID3Field& rhs);
	inline bool		operator== (const cID3Field& other) const
	{
		return(m_dwID == other.m_dwID);
	}

private:
	uint16_t		m_dwID;
	QString			m_szShortName;
	QString			m_szName;
	QVariant		m_Value;
	bool			m_bValid;
};

Q_DECLARE_METATYPE(cID3Field*)


class cID3FieldList : public QList<cID3Field *>
{
public:
	cID3FieldList();
	cID3Field*	add(uint16_t dwID, const QString& szShortName, const QString& szName, QObject *parent = 0);

	cID3Field*	get(uint16_t dwID);
	cID3Field*	get(const QString& szShortName);

	void		setValue(uint16_t dwID, const QVariant Value);
	QVariant	getValue(uint16_t dwID);

	QString		getName(uint16_t dwID);
	QString		getShortName(uint16_t dwID);

	bool		isValid(uint16_t dwID);
};

#endif // cID3Field_H
