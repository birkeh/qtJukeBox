#include "cpixmap.h"
#include "common.h"

#include <QBuffer>


cPixmap::cPixmap() :
	QPixmap()
{
}

cPixmap::cPixmap(const QByteArray& array, const QString& szFileName, const cPixmap::ImageType& imageType, const QString& szDescription) :
//	QPixmap(QPixmap::loadFromData(array)),
	m_szFileName(szFileName),
	m_ImageType(imageType),
	m_szDescription(szDescription)
{
	loadFromData(array);
}

QString	cPixmap::fileName()
{
	return(m_szFileName);
}

cPixmap::ImageType cPixmap::imageType()
{
	return(m_ImageType);
}

QString cPixmap::description()
{
	return(m_szDescription);
}

cPixmapList::cPixmapList()
{
}

cPixmap cPixmapList::add(const QByteArray &array, const QString& szFileName, const cPixmap::ImageType& imageType, const QString& szDescription)
{
	cPixmap	image(array, szFileName, imageType, szDescription);
	append(image);
	return(image);
}
