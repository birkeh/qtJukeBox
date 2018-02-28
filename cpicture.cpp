#include "cpicture.h"


cPicture::cPicture()
{
}

cPicture::cPicture(const QPixmap& Pixmap, uint16_t dwType, const QString& szDescription) :
		m_Pixmap(Pixmap), m_dwType(dwType), m_szDescription(szDescription)
{
}

void cPicture::setPixmap(const QPixmap& Pixmap)
{
	m_Pixmap	= Pixmap;
}

QPixmap cPicture::getPixmap()
{
	return(m_Pixmap);
}

void cPicture::setType(uint16_t dwType)
{
	m_dwType	= dwType;
}

uint16_t cPicture::getType()
{
	return(m_dwType);
}

void cPicture::setDescription(const QString& szDescription)
{
	m_szDescription	= szDescription;
}

QString cPicture::getDescription()
{
	return(m_szDescription);
}

QString cPicture::type(uint16_t dwType)
{
	switch(dwType)
	{
	case TYPE_OTHER:
		return("Other");
	case TYPE_ICON:
		return("32x32 pixels 'file icon' (PNG only)");
	case TYPE_OTHER_ICON:
		return("Other file icon");
	case TYPE_COVER_FRONT:
		return("Cover (front)");
	case TYPE_COVER_BACK:
		return("Cover (back)");
	case TYPE_LEAFLET:
		return("Leaflet page");
	case TYPE_MEDIA:
		return("Media (e.g. label side of CD)");
	case TYPE_LEAD_ARTIST:
		return("Lead artist/lead performer/soloist");
	case TYPE_ARTIST:
		return("Artist/performer");
	case TYPE_CONDUCTOR:
		return("Conductor");
	case TYPE_BAND:
		return("Band/Orchestra");
	case TYPE_COMPOSER:
		return("Composer");
	case TYPE_LYRICIST:
		return("Lyricist/text writer");
	case TYPE_RECORDING_LOCATION:
		return("Recording Location");
	case TYPE_DURING_RECORDING:
		return("During recording");
	case TYPE_DURING_PERFORMANCE:
		return("During performance");
	case TYPE_SCREEN_CAPTURE:
		return("Movie/video screen capture");
	case TYPE_COLOURED_FISH:
		return("A bright coloured fish");
	case TYPE_ILLUSTRATION:
		return("Illustration");
	case TYPE_BAND_LOGOTYPE:
		return("Band/artist logotype");
	case TYPE_PUBLISHER_LOGOTYPE:
		return("Publisher/Studio logotype");
	default:
		return("Unknown");
	}
}

uint16_t	 cPicture::type(const QString& szType)
{
	if(!szType.compare("Other", Qt::CaseInsensitive))
		return(TYPE_OTHER);
	if(!szType.compare("32x32 pixels 'file icon' (PNG only)", Qt::CaseInsensitive))
		return(TYPE_ICON);
	if(!szType.compare("Other file icon", Qt::CaseInsensitive))
		return(TYPE_OTHER_ICON);
	if(!szType.compare("Cover (front)", Qt::CaseInsensitive))
		return(TYPE_COVER_FRONT);
	if(!szType.compare("Cover (back)", Qt::CaseInsensitive))
		return(TYPE_COVER_BACK);
	if(!szType.compare("Leaflet page", Qt::CaseInsensitive))
		return(TYPE_LEAFLET);
	if(!szType.compare("Media (e.g. label side of CD)", Qt::CaseInsensitive))
		return(TYPE_MEDIA);
	if(!szType.compare("Lead artist/lead performer/soloist", Qt::CaseInsensitive))
		return(TYPE_LEAD_ARTIST);
	if(!szType.compare("Artist/performer", Qt::CaseInsensitive))
		return(TYPE_ARTIST);
	if(!szType.compare("Conductor", Qt::CaseInsensitive))
		return(TYPE_CONDUCTOR);
	if(!szType.compare("Band/Orchestra", Qt::CaseInsensitive))
		return(TYPE_BAND);
	if(!szType.compare("Composer", Qt::CaseInsensitive))
		return(TYPE_COMPOSER);
	if(!szType.compare("Lyricist/text writer", Qt::CaseInsensitive))
		return(TYPE_LYRICIST);
	if(!szType.compare("Recording Location", Qt::CaseInsensitive))
		return(TYPE_RECORDING_LOCATION);
	if(!szType.compare("During recording", Qt::CaseInsensitive))
		return(TYPE_DURING_RECORDING);
	if(!szType.compare("During performance", Qt::CaseInsensitive))
		return(TYPE_DURING_PERFORMANCE);
	if(!szType.compare("Movie/video screen capture", Qt::CaseInsensitive))
		return(TYPE_SCREEN_CAPTURE);
	if(!szType.compare("A bright coloured fish", Qt::CaseInsensitive))
		return(TYPE_COLOURED_FISH);
	if(!szType.compare("Illustration", Qt::CaseInsensitive))
		return(TYPE_ILLUSTRATION);
	if(!szType.compare("Band/artist logotype", Qt::CaseInsensitive))
		return(TYPE_BAND_LOGOTYPE);
	if(!szType.compare("Publisher/Studio logotype", Qt::CaseInsensitive))
		return(TYPE_PUBLISHER_LOGOTYPE);
	return(9999);
}

cPicture& cPicture::operator=(const cPicture& rhs)
{
	m_Pixmap		= rhs.m_Pixmap;
	m_dwType		= rhs.m_dwType;
	m_szDescription	= rhs.m_szDescription;
	return(*this);
}

cPictureList::cPictureList()
{
}

cPicture cPictureList::add(const QPixmap& Pixmap, uint16_t dwType, const QString& szDescription)
{
	cPicture	picture(Pixmap, dwType, szDescription);
	this->append(picture);
	return(picture);
}
