#ifndef CMEDIAINFO_H
#define CMEDIAINFO_H


#include <QObject>
#include <QLabel>
#include <QDateTime>
#include <stdint.h>
#include "libid3tag/id3tag.h"
#include "cid3field.h"


#define WITH_PICTURE


class cMediaInfo : public QObject
{
Q_OBJECT
public:
	explicit		cMediaInfo(QObject *parent = 0);
	~cMediaInfo();

	void			clear();

	void			importFromFile(const QString& szFileName);

	void			setFileName(const QString& szFileName);
	QString			getFileName();

	void			setFileCreated(const QDateTime& FileCreated);
	QDateTime		getFileCreated();

	void			setFileModified(const QDateTime& FileModified);
	QDateTime		getFileModified();

	void			setFileSize(uint64_t dwFileSize);
	uint64_t		getFileSize();

	QString			getTAGName(uint16_t dwTag);
	QString			getTAGShortName(uint16_t dwTag);

	void			setBitrate(uint16_t dwBitrate);
	uint16_t		getBitrate();

	void			setSamplerate(uint16_t dwSamplerate);
	uint16_t		getSamplerate();

	void			setChannels(uint16_t dwChannels);
	uint16_t		getChannels();

	void			setSeconds(uint16_t dwSeconds);
	uint16_t		getSeconds();

	void			setMPEG(uint16_t dwMPEG);
	uint16_t		getMPEG();

	void			setVersion(uint16_t dwVersion);
	uint16_t		getVersion();

	void			setLayerVersion(uint16_t dwLayerVersion);
	uint16_t		getLayerVersion();

	void			setMode(uint16_t dwMode);
	uint16_t		getMode();

	void			setValid(bool bValid);
	bool			getValid();

	void			setTAG(uint16_t dwTag, uint16_t dwData);
	void			setTAG(uint16_t dwTag, const QString& szData);
	void			setTAG(uint16_t dwTag, const QVariant& v);
	QVariant		getTAG(uint16_t dwTag);
	uint16_t		getTAGi(uint16_t dwTag);
	QString			getTAGs(uint16_t dwTag);
	bool			getTAG(uint16_t dwTag, QString& szData);
	bool			getTAG(uint16_t dwTag, QString& szName, QString& szData);
	bool			getTAG(uint16_t dwTag, uint16_t& dwData);
	bool			getTAG(uint16_t dwTag, QString& szName, uint16_t& dwData);
	bool			getTAG(uint16_t dwTag, id3_byte_t* dwData, uint32_t& dwLen);
	bool			getTAG(uint16_t dwTag, QString& szName, id3_byte_t* dwData, uint32_t& dwLen);
	bool			getTAG(uint16_t dwTag, QPixmap& Pixmap);
	bool			getTAG(uint16_t dwTag, QString& szName, QPixmap& Pixmap);
	bool			getTAG(uint16_t dwTag, cPictureList& PictureList);
	bool			getTAG(uint16_t dwTag, QString& szName, cPictureList& PictureList);

	static uint16_t	tagIDFromShortName(const QString& szTAG);
protected:
	QString			m_szFileName;
	QDateTime		m_FileCreated;
	QDateTime		m_FileModified;
	uint64_t		m_dwFileSize;
	cID3FieldList	m_ID3FieldList;
	uint16_t		m_dwBitrate;
	uint16_t		m_dwSamplerate;
	uint16_t		m_dwChannels;
	uint16_t		m_dwSeconds;
	uint16_t		m_dwMPEG;
	uint16_t		m_dwVersion;
	uint16_t		m_dwLayerVersion;
	uint16_t		m_dwMode;
	bool			m_bValid;

	void			readFileInformation(const QString& szFileName);
	void			importInformation(struct id3_tag* lpTag);
	uint16_t		tagID(char* lpszTagName);
	QString			tag2String(struct id3_frame* lpID3Frame, uint16_t dwField);
	QString			tag2LyricsString(struct id3_frame* lpID3Frame);

	void			initFields();
signals:

public slots:
};

Q_DECLARE_METATYPE(cMediaInfo*);

class cMediaInfoList : public QList<cMediaInfo *>
{
public:
	cMediaInfoList();

	cMediaInfo*	add(const QString& szFileName, QObject *parent = 0);
	cMediaInfo*	add(const QString& szFileName, cMediaInfo* lpMediaInfo, QObject *parent = 0);
	cMediaInfo*	get(const QString& szFileName);

protected:
	void		initFields();
};

#endif // CMEDIAINFO_H
