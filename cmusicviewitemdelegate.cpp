#include "cmusicviewitemdelegate.h"

#include <QPainter>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>
#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QTreeView>


void cMusicViewItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem & option, const QModelIndex &index) const
{
	QStyleOptionViewItem options = option;
	initStyleOption(&options, index);

	int			iLevel		= 0;
	QModelIndex	index1		= index;

	while(index1.parent().isValid())
	{
		iLevel++;
		index1	= index1.parent();
	}

	QTreeView*	lpTreeView	= (QTreeView*)parent();
	int			indentation	= 20;
	if(lpTreeView)
		indentation	= lpTreeView->indentation();

	indentation	*= iLevel;

	painter->save();

	QTextDocument doc;

	doc.setHtml(QString("%1").arg(options.text));

	options.text = "";
	options.widget->style()->drawControl(QStyle::CE_ItemViewItem, &options, painter);

	QRect	clip;

	// shift text right to make icon visible
	if(index.column() == 1)
	{
		painter->translate(options.rect.left()+indentation, options.rect.top());
		clip	= QRect(0, 0, options.rect.width()+indentation, options.rect.height());
	}
	else
	{
		painter->translate(options.rect.left(), options.rect.top());
		clip	= QRect(0, 0, options.rect.width(), options.rect.height());
	}

	painter->setClipRect(clip);

	QAbstractTextDocumentLayout::PaintContext ctx;

	ctx.clip = clip;
	doc.documentLayout()->draw(painter, ctx);

	painter->restore();
}

QSize cMusicViewItemDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
	QStyleOptionViewItem options = option;
	initStyleOption(&options, index);

	QTextDocument doc;
	doc.setHtml(options.text);
	doc.setTextWidth(options.rect.width());
	return QSize(doc.idealWidth(), doc.size().height());
}
