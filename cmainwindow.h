#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H


#include "calbum.h"

#include <QMainWindow>

#include <QStandardItemModel>
#include <QItemSelection>

#include <QTreeView>

#include <QSqlDatabase>


namespace Ui
{
	class cMainWindow;
}

class cMainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit cMainWindow(QWidget *parent = 0);
	~cMainWindow();
	
private:
	Ui::cMainWindow*	ui;
	QSqlDatabase		m_DB;
	qint16				m_iVersion;

	QStandardItemModel*	m_lpMusicListModel;
	bool				m_bProcessing;

	cAlbumList			m_albumList;

	void				addFile(const QString& szFile);
	void				addPath(const QString& szPath);

	bool				initDB();
	void				loadDB();
	void				displayDB();

	void				onCustomContextMenuRequested(const QTreeView *lpTreeView, const QPoint &pos);
	void				onDoubleClicked(const QModelIndex &index);
	void				onPressed(const QModelIndex &index);
protected:
	void				initSettings();
private slots:
	void				onCustomContextMenuRequestedOriginal(const QPoint &pos);
	void				onDoubleClickedOriginal(const QModelIndex &index);
	void				onPressedOriginal(const QModelIndex &index);
	void				onExpandedOriginal(const QModelIndex &index);
	void				onCollapsedOriginal(const QModelIndex &index);
	void				onSelectionChangedOriginal(const QItemSelection &selected, const QItemSelection &deselected);
	void				onScrollbarValueChangedOriginal(int value);

	void				onCustomContextMenuRequestedNew(const QPoint &pos);
	void				onDoubleClickedNew(const QModelIndex &index);
	void				onPressedNew(const QModelIndex &index);
	void				onExpandedNew(const QModelIndex &index);
	void				onCollapsedNew(const QModelIndex &index);
	void				onSelectionChangedNew(const QItemSelection &selected, const QItemSelection &deselected);
	void				onScrollbarValueChangedNew(int value);
};

#endif // CMAINWINDOW_H
