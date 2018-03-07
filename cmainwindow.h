#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H


#include "cdatabase.h"

#include "calbum.h"
#include "ctrack.h"

#include <QMainWindow>

#include <QStandardItemModel>
#include <QItemSelection>

#include <QTreeView>


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
	cDatabase*			m_lpDB;
	QStandardItemModel*	m_lpMusicListModel;
	bool				m_bProcessing;

	cAlbumList			m_albumList;

	void				addFile(const QString& szFile);
	void				addPath(const QString& szPath);

	void				loadDB();
	void				displayDB();

protected:
	void				initSettings();
private slots:
	void				onCustomContextMenuRequested(const QPoint &pos);
	void				onDoubleClicked(const QModelIndex &index);
	void				onPressed(const QModelIndex &index);
};

#endif // CMAINWINDOW_H
