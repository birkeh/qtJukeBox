#include "cstring.h"


cStringTime::cStringTime()
{
}

cStringTime::cStringTime(const qint32& iTime, const QString& szString) :
	m_iTime(iTime),
	m_szString(szString)
{
}

qint32 cStringTime::time()
{
	return(m_iTime);
}

QString cStringTime::string()
{
	return(m_szString);
}

cStringTimeList::cStringTimeList()
{
}

cStringTime* cStringTimeList::add(const qint32& iTime, const QString& szString)
{
	cStringTime*	lpNew	= new cStringTime(iTime, szString);
	append(lpNew);
	return(lpNew);
}

QString cStringTimeList::join()
{
	QStringList	szList;
	QString		str;

	for(int x = 0;x < count();x++)
	{
		str	= QString::number(at(x)->time()) + "|" + at(x)->string();
		szList.append(str);
	}
	return(szList.join("||"));
}

cString21::cString21() :
	m_szLeft(QStringList()),
	m_szRight("")
{
}

cString21::cString21(const QStringList& szLeft, const QString& szRight) :
	m_szLeft(szLeft),
	m_szRight(szRight)
{
}

cString21List::cString21List()
{
}

cString21* cString21List::add(const QStringList& szLeft, const QString& szRight)
{
	cString21*	lpNew	= new cString21(szLeft, szRight);
	append(lpNew);
	return(lpNew);
}
