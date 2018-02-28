#ifndef cPicture_H
#define cPicture_H

#include <QString>
#include <QVariant>
#include <QPixmap>
#include <stdint.h>


class cPicture
{
public:
	enum TYPE
	{
		TYPE_OTHER,
		TYPE_ICON,
		TYPE_OTHER_ICON,
		TYPE_COVER_FRONT,
		TYPE_COVER_BACK,
		TYPE_LEAFLET,
		TYPE_MEDIA,
		TYPE_LEAD_ARTIST,
		TYPE_ARTIST,
		TYPE_CONDUCTOR,
		TYPE_BAND,
		TYPE_COMPOSER,
		TYPE_LYRICIST,
		TYPE_RECORDING_LOCATION,
		TYPE_DURING_RECORDING,
		TYPE_DURING_PERFORMANCE,
		TYPE_SCREEN_CAPTURE,
		TYPE_COLOURED_FISH,
		TYPE_ILLUSTRATION,
		TYPE_BAND_LOGOTYPE,
		TYPE_PUBLISHER_LOGOTYPE,
	};

	explicit		cPicture();
	explicit		cPicture(const QPixmap& Pixmap, uint16_t dwType, const QString& szDescription);

	void			setPixmap(const QPixmap& Pixmap);
	QPixmap			getPixmap();

	void			setType(uint16_t dwType);
	uint16_t		getType();

	void			setDescription(const QString& szDescription);
	QString			getDescription();

	cPicture&		operator=(const cPicture& rhs);

	static QString	type(uint16_t dwType);
	static uint16_t	type(const QString& szType);
protected:
	QPixmap			m_Pixmap;
	uint16_t		m_dwType;
	QString			m_szDescription;

signals:

public slots:

};

Q_DECLARE_METATYPE(cPicture)


class cPictureList : public QList<cPicture>
{
public:
	cPictureList();
	cPicture	add(const QPixmap& Pixmap, uint16_t dwType, const QString& szDescription);
};

Q_DECLARE_METATYPE(cPictureList*)

#endif // cPicture_H
