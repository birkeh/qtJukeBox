#include "cid3field.h"


cID3Field::cID3Field(uint16_t dwID, const QString& szShortName, const QString& szName) :
	m_dwID(dwID),
	m_szShortName(szShortName),
	m_szName(szName),
	m_bValid(false)
{
}

cID3Field::~cID3Field()
{
	if(m_Value.canConvert<cPictureList*>())
	{
		cPictureList*	lpPictureList	= m_Value.value<cPictureList*>();
		delete lpPictureList;
	}
}

uint16_t cID3Field::getID()
{
	return(m_dwID);
}

void cID3Field::setShortName(const QString& szShortName)
{
	m_bValid	= false;

	if(szShortName.length())
	{
		m_szShortName	= szShortName;
		if(m_Value.isValid() && m_szName.length())
			m_bValid	= true;
	}
}

QString cID3Field::getShortName()
{
	return(m_szShortName);
}

void cID3Field::setName(const QString& szName)
{
	m_bValid	= false;

	if(szName.length())
	{
		m_szName	= szName;
		if(m_Value.isValid() && m_szShortName.length())
			m_bValid	= true;
	}
}

QString cID3Field::getName()
{
	return(m_szName);
}

void cID3Field::setValue(const QVariant& Value)
{
	m_bValid	= false;
	if(!Value.isValid())
		return;
	m_Value		= Value;
	if(m_szName.length() && m_szShortName.length())
		m_bValid	= true;
}

QVariant cID3Field::getValue()
{
	if(m_bValid)
		return(m_Value);
	return(QVariant());
}

bool cID3Field::isValid()
{
	return(m_bValid);
}

cID3Field& cID3Field::operator=(const cID3Field& rhs)
{
	m_dwID		= rhs.m_dwID;
	m_szName	= rhs.m_szName;
	m_Value		= rhs.m_Value;
	m_bValid	= rhs.m_bValid;

	return(*this);
}

cID3FieldList::cID3FieldList()
{
}

cID3Field* cID3FieldList::add(uint16_t dwID, const QString& szShortName, const QString& szName, QObject *parent)
{
	cID3Field*	lpID3Field	= get(dwID);
	if(lpID3Field)
		return(lpID3Field);

	lpID3Field	= new cID3Field(dwID, szShortName, szName);
	this->append(lpID3Field);
	return(lpID3Field);
}

cID3Field* cID3FieldList::get(uint16_t dwID)
{
	int	iIndex;

	for(iIndex = 0;iIndex < this->count();iIndex++)
	{
		if(this->at(iIndex)->getID() == dwID)
			return(this->at(iIndex));
	}
	return(0);
}

cID3Field* cID3FieldList::get(const QString& szShortName)
{
	int	iIndex;

	for(iIndex = 0;iIndex < this->count();iIndex++)
	{
		if(this->at(iIndex)->getShortName() == szShortName)
			return(this->at(iIndex));
	}
	return(0);
}

void cID3FieldList::setValue(uint16_t dwID, const QVariant Value)
{
	cID3Field*	lpID3Field	= this->get(dwID);
	if(lpID3Field)
		lpID3Field->setValue(Value);
}

QVariant cID3FieldList::getValue(uint16_t dwID)
{
	cID3Field*	lpID3Field	= this->get(dwID);
	if(lpID3Field)
		return(lpID3Field->getValue());
	return(QVariant());
}

QString cID3FieldList::getName(uint16_t dwID)
{
	cID3Field*	lpID3Field	= this->get(dwID);
	if(lpID3Field)
		return(lpID3Field->getName());
	return("");
}

QString cID3FieldList::getShortName(uint16_t dwID)
{
	cID3Field*	lpID3Field	= this->get(dwID);
	if(lpID3Field)
		return(lpID3Field->getShortName());
	return("");
}

bool cID3FieldList::isValid(uint16_t dwID)
{
	cID3Field*	lpID3Field	= this->get(dwID);
	if(lpID3Field)
		return(lpID3Field->isValid());
	return(false);
}
