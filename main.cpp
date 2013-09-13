#include <iostream>
#include <memory>

#include <QApplication>
#include <QTextStream>
#include <QDebug>
#include <QTextDocument>
#include <QPrinter>
#include <QTimer>
#include <QAxObject>
#include <QString>
#include <QStringList>
#include <QSizeF>
#include <QFont>
#include <QFile>

QList<QStringList> read_xls(const QString& filename)
{
	std::unique_ptr<QAxObject> excel(new QAxObject("Excel.Application", 0));
	QAxObject* workbooks = excel->querySubObject("Workbooks");
	QAxObject* workbook = workbooks->querySubObject("Open(const QString&)",
													filename);
	QAxObject* sheets = workbook->querySubObject("Worksheets");

	QList<QStringList> data;

	int count = sheets->dynamicCall("Count()").toInt();
	if (count == 0) {
		// There is no sheets
		return data;
	}

	QAxObject* sheet = sheets->querySubObject("Item(int)", 1);

	QAxObject* rows = sheet->querySubObject("Rows");
	int rowCount = rows->property("Count").toInt(); // always 65536
	QAxObject* cols = sheet->querySubObject("Columns");
	int colCount = cols->property("Count").toInt(); // always 256

	for (int row = 1; row <= rowCount; ++row) {
		QStringList dataRow;
		for (int col = 1; col <= colCount; ++col) {
			QAxObject* cell = 
				sheet->querySubObject("Cells")->querySubObject("Item(int, int)", row, col);
				QString value = cell->property("Value").toString();
				if (value.size()) {
					dataRow.push_back(value);
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

	workbook->dynamicCall("Close()");
	excel->dynamicCall("Quit()");

	return data;	
}

void print_pdf(const QString& filename, const QStringList& data, const QString& sheet)
{
	QString html;
	html += "<html>";
	html += "<body>";
	for (QStringList::const_iterator i = data.begin(); i != data.end(); ++i) {
		html += *i;
		html += "<br>";
	}
	html += "</body>";
	html += "</html>";

	QTextDocument td;
	/*
	QFont font;
	font.setPointSize(52);
	td.setDefaultFont(font);
	*/
	td.setDefaultStyleSheet(sheet);
	td.setHtml(html);
	QPrinter printer;
	printer.setOutputFormat(QPrinter:: PdfFormat);
	printer.setOutputFileName(filename);
//	printer.setPaperSize(QPrinter::Custom);
	printer.setPaperSize(QSizeF(400, 245), QPrinter::Millimeter);
	td.print(&printer);
}

QString read_stylesheet(const QString& filename)
{
	QFile stylesheet(filename);
	if (!stylesheet.open(QIODevice::ReadOnly)) {
		qDebug() << "Error open file: " << stylesheet.errorString();
		return QString();
	}

	QTextStream in(&stylesheet);
	QString sheet;
	while (!in.atEnd()) {
		sheet += in.readLine();
	}

	stylesheet.close();

	return sheet;
}

int main(int argc, char* argv[])
{
	QApplication qapp(argc, argv);
	
	QTextStream qout(stdout);


	QList<QStringList> data = read_xls("C:\\src\\cpp\\stikers\\example.xlsx");

	QString sheet = read_stylesheet("style.css");
	for (QList<QStringList>::iterator i = data.begin(); i != data.end(); ++i) {
		for (QStringList::iterator j = i->begin(); j != i->end(); ++j) {
			qout << *j << ", ";
		}
		qout << "\n";
		QString filename(*i->begin() + ".pdf");
		print_pdf(filename, *i, sheet);
	}

	return 0;
}

