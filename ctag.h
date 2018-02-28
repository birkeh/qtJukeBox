#ifndef CTAG_H
#define CTAG_H

#include <QString>
#include <QStringList>
#include <QList>
#include <QMetaType>


class cTAG
{
public:
	explicit	cTAG();
	explicit	cTAG(const QString& szTAG, const QString& szValue);
	explicit	cTAG(const QString& szTAG, const QStringList& szValue);

	QString		tag();
	QStringList	valueList();
protected:
	QString		m_szTAG;
	QStringList	m_szValue;
signals:
	
public slots:
	
};

Q_DECLARE_METATYPE(cTAG)

class cTAGList : public QList<cTAG *>
{
public:
	cTAGList();
	cTAG*		add(const QString& szTAG, const QString& szValue);
	cTAG*		add(const QString& szTAG, const QStringList& szValue);
};

#endif // CTAG_H
