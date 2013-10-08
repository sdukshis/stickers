#include <iostream>

#include <QApplication>
#include <QTextStream>
#include <QDebug>
#include <QAxObject>
#include <QString>
#include <QStringList>

#include "sticker.h"
#include "painter.h"

QTextStream qout(stdout);

QList<QStringList> read_xls(const QString& filename)
{
	qout << "Openning Excel Application\n"; qout.flush();
	QAxObject* excel(new QAxObject("Excel.Application", 0));
	QAxObject* workbooks = excel->querySubObject("Workbooks");
	qout << "Openning '" << filename << "'\n"; qout.flush();
	QAxObject* workbook = workbooks->querySubObject("Open(const QString&)",
													filename);
	QAxObject* sheets = workbook->querySubObject("Worksheets");

	QList<QStringList> data;

	int count = sheets->dynamicCall("Count()").toInt();
	if (count == 0) {
		// There is no sheets
		return data;
	}

	qout << "Openning first sheet in '" << filename << "'\n"; qout.flush();
	QAxObject* sheet = sheets->querySubObject("Item(int)", 1);

	QAxObject* rows = sheet->querySubObject("Rows");
	int rowCount = rows->property("Count").toInt(); // always 65536
	QAxObject* cols = sheet->querySubObject("Columns");
	int colCount = cols->property("Count").toInt(); // always 256

	qout << "Reading table...\n"; qout.flush();
	for (int row = 1; row <= rowCount; ++row) {
		QStringList dataRow;
		for (int col = 1; col <= colCount; ++col) {
			QAxObject* cell = 
				sheet->querySubObject("Cells")->querySubObject("Item(int, int)", row, col);
				QString value = cell->property("Value").toString();
				if (value.size()) {
					dataRow.push_back(value.trimmed());
				} else {
					break;
				}
		}
		if (dataRow.size()) {
			data.push_back(dataRow);
		} else {
			break;
		}
	}
	qout << "Read " << data.size() << " rows\n"; qout.flush();
	qout << "Closing '" << filename << "'\n"; qout.flush();
	sheets->clear(); 
	delete sheets;
	workbook->dynamicCall("Close()");
	workbook->clear();
	delete workbook;
	workbooks->dynamicCall("Close()");
	workbooks->clear();
	delete workbooks;
	qout << "Quiting excel\n"; qout.flush();
	excel->dynamicCall("Quit()");
	delete excel;

	return data;	
}


int main(int argc, char* argv[])
{
	if (argc < 2) {
		qout << "Usage: stickers.exe filename\n"; qout.flush();
		return -1;
	}

	QStringList paths = QCoreApplication::libraryPaths();
    paths.append(".");
    QCoreApplication::setLibraryPaths(paths);

	QApplication qapp(argc, argv);
	

	QList<QStringList> data = read_xls(argv[1]);

	QList<Sticker> stickers;
	for (QList<QStringList>::iterator i = ++data.begin(); i != data.end(); ++i) {
		Sticker sticker(i->value(0).toStdString());

	    sticker.setLogo(i->value(0).toStdString());
	    sticker.setArticle(i->value(1).toStdString());
	    sticker.setType(i->value(2).toStdString());
	    sticker.setNumber(i->value(3).toInt());
	    sticker.setVoltage(i->value(4).toStdString());
	    sticker.setIP(i->value(5).toStdString());
	    sticker.setFlame(i->value(6).toInt());
	    sticker.setLedType(i->value(7).toInt());
	    sticker.setProtection(i->value(8).toInt());

	    stickers.push_back(sticker);
	}

	qout << "Start printing...\n"; qout.flush();
	for (QList<Sticker>::iterator i = stickers.begin(); i != stickers.end(); ++i) {
	    if (socle_painter(*i)) {
	        qout << "Fail to paint " << QString(i->toString().c_str()) <<" Socle.pdf\n"; qout.flush();
	    } else {
	        qout << "Paint " << QString(i->toString().c_str()) << " Socle.pdf OK" << "\n"; qout.flush();
	    }
	    if (cup_painter(*i)) {
	        qout << "Fail to paint " << QString(i->toString().c_str()) << "Cup.pdf\n"; qout.flush();
	    } else {
	        qout << "Paint " << QString(i->toString().c_str()) << " Cup.pdf OK" << "\n"; qout.flush();
	    }			
	}
	qout << "Finish\n";

	return 0;
}

