#include "cimage.h"
#include "common.h"


cImage::cImage()
{
}

cImage::cImage(const QImage& image, const QString& szFileName, const cImage::ImageType& imageType, const QString& szDescription) :
	m_image(image),
	m_szFileName(szFileName),
	m_ImageType(imageType),
	m_szDescription(szDescription)
{
}

cImage::cImage(const QByteArray& array, const QString& szFileName, const cImage::ImageType& imageType, const QString& szDescription) :
	m_image(QImage::fromData(array)),
	m_szFileName(szFileName),
	m_ImageType(imageType),
	m_szDescription(szDescription)
{
}

cImageList::cImageList()
{
}

cImage* cImageList::add(const QImage& image, const QString& szFileName, const cImage::ImageType& imageType, const QString& szDescription)
{
	cImage*	lpNew	= new cImage(image, szFileName, imageType, szDescription);

	if(!lpNew)
		return(0);
	append(lpNew);
	return(lpNew);
}

cImage* cImageList::add(const QByteArray& array, const QString& szFileName, const cImage::ImageType& imageType, const QString& szDescription)
{
	cImage*	lpNew	= new cImage(array, szFileName, imageType, szDescription);

	if(!lpNew)
		return(0);
	append(lpNew);
	return(lpNew);
}
