#ifndef CDATABASE_H
#define CDATABASE_H

#include <QObject>
#include <QtSql>
#include <QSqlDatabase>
#include <QTableView>
#include <QSqlError>


class cDatabase : public QObject
{
	Q_OBJECT
public:
	explicit cDatabase(QObject *parent = 0);
	~cDatabase();

	QSqlDatabase	getDB();
signals:
	
public slots:

protected:
	QSqlDatabase	m_DB;
	qint16			m_iVersion;

	void		init();
	int			initDB();
};

#endif // CDATABASE_H
