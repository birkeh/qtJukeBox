#ifndef CMUSICVIEWITEMDELEGATE_H
#define CMUSICVIEWITEMDELEGATE_H


#include <QStyledItemDelegate>


class cMusicViewItemDelegate : public QStyledItemDelegate
{
public:
	cMusicViewItemDelegate(QWidget *parent = 0) : QStyledItemDelegate(parent) {}
protected:
	void			paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	QSize			sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // CMUSICVIEWITEMDELEGATE_H
