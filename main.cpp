#include <QApplication>
#include <locale>

#include "stickerWidget.h"



int main(int argc, char* argv[])
{
	std::locale("");
	QStringList paths = QCoreApplication::libraryPaths();
    paths.append(".");
    QCoreApplication::setLibraryPaths(paths);

	QApplication qapp(argc, argv);

	StickerWidget *widget = new StickerWidget();

	widget->show();
	
	return qapp.exec();
}

