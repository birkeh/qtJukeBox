#ifndef CSTRING_H
#define CSTRING_H

#include <QString>
#include <QStringList>
#include <QList>
#include <QMetaType>


class cStringTime
{
public:
	explicit	cStringTime();
	explicit	cStringTime(const qint32& iTime, const QString& szString);
protected:
	qint32		m_iTime;
	QString		m_szString;
signals:

public slots:
};

Q_DECLARE_METATYPE(cStringTime)

class cStringTimeList : public QList<cStringTime *>
{
public:
	cStringTimeList();
	cStringTime*	add(const qint32& iTime, const QString& szString);
};


class cString21
{
public:
	explicit	cString21();
	explicit	cString21(const QStringList& szLeft, const QString& szRight);
protected:
	QStringList	m_szLeft;
	QString		m_szRight;
signals:

public slots:

};

Q_DECLARE_METATYPE(cString21)

class cString21List : public QList<cString21 *>
{
public:
	cString21List();
	cString21*	add(const QStringList& szLeft, const QString& szRight);
};

#endif // CSTRING_H
