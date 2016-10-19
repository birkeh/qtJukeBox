#include "cimage.h"


cImage::cImage() :
	QImage()
{
}

cImage::cImage(const QByteArray& array, const QString& szFileName, const cImage::ImageType& imageType, const QString& szDescription) :
	QImage(QImage::fromData(array)),
	m_szFileName(szFileName),
	m_ImageType(imageType),
	m_szDescription(szDescription)
{
}

cImageList::cImageList()
{
}

cImage* cImageList::add(const QByteArray& array, const QString& szFileName, const cImage::ImageType& imageType, const QString& szDescription)
{
	cImage*	lpNew	= new cImage(array, szFileName, imageType, szDescription);
	append(lpNew);
	return(lpNew);
}
