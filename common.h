#ifndef COMMON_H
#define COMMON_H


#include <QDebug>


#define	MUSICLIST_ORI_ALBUM		0
#define	MUSICLIST_ORI_TRACK		1

#define	MUSICLIST_NEW_ALBUM		2
#define	MUSICLIST_NEW_TRACK		3


#define SORT_ALBUM_ASC			0x0001
#define SORT_ALBUM_DESC			0x0002
#define SORT_BAND_ASC			0x0004
#define SORT_BAND_DESC			0x0008

#define SORT_TRACK_ASC			0x0010
#define SORT_TRACK_DESC			0x0020


#define USERROLE_TYPE			(Qt::UserRole+1)
#define USERROLE_VALUE			(Qt::UserRole+2)

#define USERROLE_TYPE_BAND		1
#define USERROLE_TYPE_ALBUM		2
#define	USERROLE_TYPE_TRACK		3


#define myDebug qDebug() << __FILE__ << "(" << __LINE__ << ") - " << __PRETTY_FUNCTION__ << ":"


#endif // COMMON_H
