#ifndef CPIXMAP_H
#define CPIXMAP_H

#include <QPixmap>
#include <QList>
#include <QMetaType>


class cPixmap : public QPixmap
{
public:
	enum	ImageType
	{
		Other				= 0x00,
		FileIcon			= 0x01,
		OtherFileIcon		= 0x02,
		FrontCover			= 0x03,
		BackCover			= 0x04,
		LeafletPage			= 0x05,
		Media				= 0x06,
		LeadArtist			= 0x07,
		Artist				= 0x08,
		Conductor			= 0x09,
		Band				= 0x0A,
		Composer			= 0x0B,
		Lyricist			= 0x0C,
		RecordingLocation	= 0x0D,
		DuringRecording		= 0x0E,
		DuringPerformance	= 0x0F,
		MovieScreenCapture	= 0x10,
		ColouredFish		= 0x11,
		Illustration		= 0x12,
		BandLogo			= 0x13,
		PublisherLogo		= 0x14
	};
	cPixmap();
	cPixmap(const QByteArray& array, const QString& szFileName, const cPixmap::ImageType& imageType, const QString& szDescription);

	QString		fileName();
	ImageType	imageType();
	QString		description();
protected:
	QString		m_szFileName;
	ImageType	m_ImageType;
	QString		m_szDescription;

signals:

public slots:

};

Q_DECLARE_METATYPE(cPixmap)

class cPixmapList : public QList<cPixmap>
{
public:
	cPixmapList();
	cPixmap add(const QByteArray& array, const QString& szFileName, const cPixmap::ImageType& imageType, const QString& szDescription);
};

#endif // CPIXMAP_H
