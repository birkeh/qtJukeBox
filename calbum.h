#ifndef CALBUM_H
#define CALBUM_H


#include <QMetaType>
#include <QString>
#include <QList>


class cAlbum
{
public:
	cAlbum(const QString& szAlbum, const QString& szLeadArtist);

	void			setAlbum(const QString& szAlbum);
	QString			album();

	void			setLeadArtist(const QString& szLeadArtist);
	QString			leadArtist();
private:
	QString			m_szAlbum;
	QString			m_szLeadArtist;
};

Q_DECLARE_METATYPE(cAlbum*)

class cAlbumList : public QList<cAlbum*>
{
public:
	cAlbum*			add(const QString& szAlbum, const QString &szLeadArtist);
};


#endif // CALBUM_H
