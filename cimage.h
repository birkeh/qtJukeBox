#ifndef CIMAGE_H
#define CIMAGE_H

#include <QImage>
#include <QList>
#include <QMetaType>


class cImage : public QImage
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
	explicit	cImage();
	explicit	cImage(const QByteArray& array, const QString& szFileName, const cImage::ImageType& imageType, const QString& szDescription);

protected:
	QString		m_szFileName;
	ImageType	m_ImageType;
	QString		m_szDescription;
signals:

public slots:

};

Q_DECLARE_METATYPE(cImage)

class cImageList : public QList<cImage *>
{
public:
	cImageList();
	cImage*	add(const QByteArray& array, const QString& szFileName, const cImage::ImageType& imageType, const QString& szDescription);
};

#endif // CIMAGE_H
