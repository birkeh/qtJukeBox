#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include <QMainWindow>

#include "cdatabase.h"


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

	void				addFile(const QString& szFile);
	void				addPath(const QString& szPath);
protected:
	void	initSettings();
};

#endif // CMAINWINDOW_H
