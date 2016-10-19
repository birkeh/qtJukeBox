#include "cmainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
	QCoreApplication::setOrganizationName("WIN-DESIGN");
	QCoreApplication::setOrganizationDomain("windesign.at");
	QCoreApplication::setApplicationName("qtJukeBox");

	QApplication a(argc, argv);
	cMainWindow w;

	w.show();
	
	return a.exec();
}
