#ifndef COMMON_H
#define COMMON_H


#define	MUSICLIST_ORI_ALBUM		0
#define	MUSICLIST_ORI_TRACK		1

#define	MUSICLIST_NEW_ALBUM		2
#define	MUSICLIST_NEW_TRACK		3


#include <QDebug>


//#define myDebug qDebug() << __FILE__ << "(" << __LINE__ << ") - " << __PRETTY_FUNCTION__ << ":"
#define myDebug qDebug() << __FILE__ << "(" << __LINE__ << ") - " << __FUNCTION__ << ":"


#endif // COMMON_H
