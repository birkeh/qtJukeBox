#include "ctag.h"


cTAG::cTAG()
{
	m_szTAG	= "";
	m_szValue.clear();
}

cTAG::cTAG(const QString& szTAG, const QString& szValue)
{
	m_szTAG		= szTAG;
	m_szValue	= szValue.split("\n");
}

cTAG::cTAG(const QString& szTAG, const QStringList& szValue)
{
	m_szTAG		= szTAG;
	m_szValue	= szValue;
}

QString cTAG::tag()
{
	return(m_szTAG);
}

QStringList cTAG::valueList()
{
	return(m_szValue);
}

cTAGList::cTAGList()
{
}

cTAG* cTAGList::add(const QString& szTAG, const QString& szValue)
{
	cTAG*	lpTag	= new cTAG(szTAG, szValue);
	append(lpTag);
	return(lpTag);
}

cTAG* cTAGList::add(const QString& szTAG, const QStringList& szValue)
{
	cTAG*	lpTag	= new cTAG(szTAG, szValue);
	append(lpTag);
	return(lpTag);
}
