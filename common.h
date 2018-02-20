#ifndef COMMON_H
#define COMMON_H


#include <QDebug>


#define myDebug qDebug() << __FILE__ << "(" << __LINE__ << ") - " << __PRETTY_FUNCTION__ << ":"


#endif // COMMON_H
