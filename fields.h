#ifndef FIELDS_H
#define FIELDS_H


#include "cid3field.h"


typedef struct tagFIELDS
{
	uint16_t	dwID;
	const char*	lpszShortName;
	const char*	lpszName;
}	FIELDS, *LPFIELDS;


FIELDS	g_Fields[]	=
{
	{ cID3Field::TAG_AudioEncryption,						"AENC", "Audio encryption" },
	{ cID3Field::TAG_AttachedPicture,						"APIC", "Attached picture" },
	{ cID3Field::TAG_AudioSeekPointIndex,					"ASPI", "Audio seek point index" },
	{ cID3Field::TAG_Comments,								"COMM", "Comments" },
	{ cID3Field::TAG_CommercialFrame,						"COMR", "Commercial frame" },
	{ cID3Field::TAG_EncryptionMethodRegistration,			"ENCR", "Encryption method registration" },
	{ cID3Field::TAG_Equalisation,							"EQU2", "Equalisation (2)" },
	{ cID3Field::TAG_EventTimingCodes,						"ETCO", "Event timing codes" },
	{ cID3Field::TAG_GeneralEncapsulatedObject,				"GEOB", "General encapsulated object" },
	{ cID3Field::TAG_GroupIdentificationRegistration,		"GRID", "Group identification registration" },
	{ cID3Field::TAG_LinkedInformation,						"LINK", "Linked information" },
	{ cID3Field::TAG_MusicCDIdentifier,						"MCDI", "Music CD identifier" },
	{ cID3Field::TAG_MPEGLocationLookupTable,				"MLLT", "MPEG location lookup table" },
	{ cID3Field::TAG_OwnershipFrame,						"OWNE", "Ownership frame" },
	{ cID3Field::TAG_PrivateFrame,							"PRIV", "Private frame" },
	{ cID3Field::TAG_PlayCounter,							"PCNT", "Play counter" },
	{ cID3Field::TAG_Popularimeter,							"POPM", "Popularimeter" },
	{ cID3Field::TAG_PositionSynchronisationFrame,			"POSS", "Position synchronisation frame" },
	{ cID3Field::TAG_RecommendedBufferSize,					"RBUF", "Recommended buffer size" },
	{ cID3Field::TAG_RelativeVolumeAdjustment,				"RVA2", "Relative volume adjustment (2)" },
	{ cID3Field::TAG_Reverb,								"RVRB", "Reverb" },
	{ cID3Field::TAG_SeekFrame,								"SEEK", "Seek frame" },
	{ cID3Field::TAG_SignatureFrame,						"SIGN", "Signature frame" },
	{ cID3Field::TAG_SynchronisedLyric,						"SYLT", "Synchronised lyric/text" },
	{ cID3Field::TAG_SynchronisedTempoCodes,				"SYTC", "Synchronised tempo codes" },
	{ cID3Field::TAG_Album,									"TALB", "Album/Movie/Show title" },
	{ cID3Field::TAG_BeatsPerMinute,						"TBPM", "BPM (beats per minute)" },
	{ cID3Field::TAG_Composer,								"TCOM", "Composer" },
	{ cID3Field::TAG_ContentType,							"TCON", "Content type" },
	{ cID3Field::TAG_CopyrightMessage,						"TCOP", "Copyright message" },
	{ cID3Field::TAG_EncodingTime,							"TDEN", "Encoding time" },
	{ cID3Field::TAG_PlaylistDelay,							"TDLY", "Playlist delay" },
	{ cID3Field::TAG_OriginalReleaseTime,					"TDOR", "Original release time" },
	{ cID3Field::TAG_RecordingTime,							"TDRC", "Recording time" },
	{ cID3Field::TAG_ReleaseTime,							"TDRL", "Release time" },
	{ cID3Field::TAG_TaggingTime,							"TDTG", "Tagging time" },
	{ cID3Field::TAG_EncodedBy,								"TENC", "Encoded by" },
	{ cID3Field::TAG_Lyricist,								"TEXT", "Lyricist/Text writer" },
	{ cID3Field::TAG_FileType,								"TFLT", "File type" },
	{ cID3Field::TAG_InvolvedPeopleList,					"TIPL", "Involved people list" },
	{ cID3Field::TAG_ContentGroupDescription,				"TIT1", "Content group description" },
	{ cID3Field::TAG_Title,									"TIT2", "Title/songname/content description" },
	{ cID3Field::TAG_Subtitle,								"TIT3", "Subtitle/Description refinement" },
	{ cID3Field::TAG_InitialKey,							"TKEY", "Initial key" },
	{ cID3Field::TAG_Languages,								"TLAN", "Language(s)" },
	{ cID3Field::TAG_Length,								"TLEN", "Length" },
	{ cID3Field::TAG_MusicianCreditsList,					"TMCL", "Musician credits list" },
	{ cID3Field::TAG_MediaType,								"TMED", "Media type" },
	{ cID3Field::TAG_Mood,									"TMOO", "Mood" },
	{ cID3Field::TAG_OriginalAlbum,							"TOAL", "Original album/movie/show title" },
	{ cID3Field::TAG_OriginalFileName,						"TOFN", "Original filename" },
	{ cID3Field::TAG_OriginalLyricist,						"TOLY", "Original lyricist(s)/text writer(s)" },
	{ cID3Field::TAG_OriginalArtist,						"TOPE", "Original artist(s)/performer(s)" },
	{ cID3Field::TAG_FileOwner,								"TOWN", "File owner/licensee" },
	{ cID3Field::TAG_LeadPerformer,							"TPE1", "Lead performer(s)/Soloist(s)" },
	{ cID3Field::TAG_Band,									"TPE2", "Band/orchestra/accompaniment" },
	{ cID3Field::TAG_Conductor,								"TPE3", "Conductor/performer refinement" },
	{ cID3Field::TAG_InterpretedBy,							"TPE4", "Interpreted, remixed, or otherwise modified by" },
	{ cID3Field::TAG_PartOfASet,							"TPOS", "Part of a set" },
	{ cID3Field::TAG_ProducedNotice,						"TPRO", "Produced notice" },
	{ cID3Field::TAG_Publisher,								"TPUB", "Publisher" },
	{ cID3Field::TAG_TrackNumber,							"TRCK", "Track number/Position in set" },
	{ cID3Field::TAG_InternetRadioStationName,				"TRSN", "Internet radio station name" },
	{ cID3Field::TAG_InternetRadioStationOwner,				"TRSO", "Internet radio station owner" },
	{ cID3Field::TAG_AlbumSortOrder,						"TSOA", "Album sort order" },
	{ cID3Field::TAG_PerformerSortOrder,					"TSOP", "Performer sort order" },
	{ cID3Field::TAG_TitleSortOrder,						"TSOT", "Title sort order" },
	{ cID3Field::TAG_InternationalStandardRecordingCode,	"TSRC", "ISRC (international standard recording code)" },
	{ cID3Field::TAG_Software_Hardware,						"TSSE", "Software/Hardware and settings used for encoding" },
	{ cID3Field::TAG_SetSubtitle,							"TSST", "Set subtitle" },
	{ cID3Field::TAG_UserDefinedTextInformation,			"TXXX", "User defined text information frame" },
	{ cID3Field::TAG_UniqueFileIdentifier,					"UFID", "Unique file identifier" },
	{ cID3Field::TAG_TermsOfUse,							"USER", "Terms of use" },
	{ cID3Field::TAG_UnsynchronizedLyric,					"USLT", "Unsynchronised lyric/text transcription" },
	{ cID3Field::TAG_CommercialInformation,					"WCOM", "Commercial information" },
	{ cID3Field::TAG_LegalInformation,						"WCOP", "Copyright/Legal information" },
	{ cID3Field::TAG_OfficialAudioFileWebpage,				"WOAF", "Official audio file webpage" },
	{ cID3Field::TAG_OfficialArtistWebpage,					"WOAR", "Official artist/performer webpage" },
	{ cID3Field::TAG_OfficialAudioSourceWebpage,			"WOAS", "Official audio source webpage" },
	{ cID3Field::TAG_OfficialInternetRadioStationWebpage,	"WORS", "Official Internet radio station homepage" },
	{ cID3Field::TAG_Payment,								"WPAY", "Payment" },
	{ cID3Field::TAG_PublishersOfficialWebpage,				"WPUB", "Publishers official webpage" },
	{ cID3Field::TAG_UserDefinedURLLinkFrame,				"WXXX", "User defined URL link frame" },
};


#endif // FIELDS_H
