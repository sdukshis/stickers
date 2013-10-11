#include <string>

#include <QApplication>
#include <QDir>

#include "StickerWidget.h"

StickerWidget::StickerWidget(QWidget* parent):
    QWidget(parent) 
{

    this->setMinimumSize(WINDOW_MIN_WIDTH, WINDOW_MIN_HEIGHT);

    mainLayout = new QVBoxLayout();

    inputFileLayout = new QHBoxLayout();
    inputFileButton = new QPushButton("&Input");
    connect(inputFileButton, SIGNAL(clicked()), this, SLOT(chooseInput()));
    inputFileLine = new QLineEdit();

    inputFileLayout->addWidget(inputFileButton);
    inputFileLayout->addWidget(inputFileLine);

    outputDirLayout = new QHBoxLayout();
    outputDirButton = new QPushButton("&Output");
    connect(outputDirButton, SIGNAL(clicked()), this, SLOT(chooseOutput()));
    outputDirLine = new QLineEdit();

    outputDirLayout->addWidget(outputDirButton);
    outputDirLayout->addWidget(outputDirLine);

    progressEdit = new QTextEdit();
    progressEdit->setReadOnly(true);

    runLayout = new QHBoxLayout();
    runButton = new QPushButton("&Run");
    connect(runButton, SIGNAL(clicked()), this, SLOT(run()));
    runLayout->addWidget(runButton);
    runLayout->addStretch();

    mainLayout->addLayout(inputFileLayout);
    mainLayout->addLayout(outputDirLayout);
    mainLayout->addWidget(progressEdit);
    mainLayout->addLayout(runLayout);

    this->setLayout(mainLayout);

}

StickerWidget::~StickerWidget()
{

}

void StickerWidget::chooseInput()
{

    QString newInputFileName =  
    QFileDialog::getOpenFileName(this, tr("Choose File"),
                                 qApp->applicationDirPath(),
                                 tr("Excel (*.xls *.xlsx);;CSV (*.csv);;All (*.*)"));
    if (newInputFileName != "") {
        inputFileName = newInputFileName;
        this->inputFileLine->setText(inputFileName);
        this->printMessage(QString(tr("You choose ")) + inputFileName + QString(" as input file"));
    }
}

void StickerWidget::chooseOutput()
{
    QString newOutputDirName =  
    QFileDialog::getExistingDirectory(this, tr("Choose directory"),
                                 qApp->applicationDirPath(),
                                 QFileDialog::ShowDirsOnly);
    if (newOutputDirName != "") {
        outputDirName = newOutputDirName;
        this->outputDirLine->setText(outputDirName);
        this->printMessage(QString(tr("You choose ")) + outputDirName + QString(" as output dir"));
    }
}

void StickerWidget::run()
{
    printMessage(QString("Working..."));
    QList<QStringList> table = read_xls(this->inputFileLine->text());
    QList<Sticker> stickers = createStickers(table);
    paintAllSticker(stickers);
}


QList<QStringList> StickerWidget::read_xls(const QString& filename)
{
    printMessage(QString("Openning Excel Application"));
    QAxObject* excel(new QAxObject("Excel.Application", 0));
    QAxObject* workbooks = excel->querySubObject("Workbooks");
    printMessage(QString("Openning ") + filename);
    
    QAxObject* workbook = workbooks->querySubObject("Open(const QString&)",
                                                    filename);
    QAxObject* sheets = workbook->querySubObject("Worksheets");

    QList<QStringList> data;

    int count = sheets->dynamicCall("Count()").toInt();
    if (count == 0) {
        // There is no sheets
        return data;
    }

    printMessage(QString("Openning first sheet in ") + filename);
    QAxObject* sheet = sheets->querySubObject("Item(int)", 1);

    QAxObject* rows = sheet->querySubObject("Rows");
    int rowCount = rows->property("Count").toInt(); // always 65536
    QAxObject* cols = sheet->querySubObject("Columns");
    int colCount = cols->property("Count").toInt(); // always 256

    printMessage(QString("Reading table..."));
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
    printMessage(QString("Read ") + QString(data.size()) + QString(" rows"));
    printMessage(QString("Closing ") + filename);
    sheets->clear(); 
    delete sheets;
    workbook->dynamicCall("Close()");
    workbook->clear();
    delete workbook;
    workbooks->dynamicCall("Close()");
    workbooks->clear();
    delete workbooks;
    printMessage(QString("Quiting Excel"));
    excel->dynamicCall("Quit()");
    delete excel;

    return data;    
}

QList<Sticker> StickerWidget::createStickers(const QList<QStringList>& table)
{
    QList<Sticker> stickers;
    for (QList<QStringList>::const_iterator i = ++table.begin(); i != table.end(); ++i) {
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

    return stickers;
}

int StickerWidget::paintAllSticker(const QList<Sticker>& stickers)
{
    std::string output =  outputDirName.toStdString() + "/";
    QString prev_dir = QDir::currentPath();
    QDir::setCurrent(outputDirName + "/");
    for (QList<Sticker>::const_iterator i = stickers.begin(); i != stickers.end(); ++i) {
        if (socle_painter(*i, "")) {
            printMessage(QString("Fail to paint ") + QString(i->toString().c_str()) + QString( "Socle.pdf"));
        } else {
            printMessage(QString("Paint ") + QString(i->toString().c_str()) + QString( "Socle.pdf OK"));
        }
        if (cup_painter(*i, "")) {
            printMessage(QString("Fail to paint ") + QString(i->toString().c_str()) + QString( "Cup.pdf"));
        } else {
            printMessage(QString("Paint ") + QString(i->toString().c_str()) + QString( "Cup.pdf OK"));
        }           
    }
    QDir::setCurrent(prev_dir);
    return 0;
}

void StickerWidget::printMessage(const QString& msg)
{
    QTextDocument* textDocument = this->progressEdit->document();
    QString text = textDocument->toPlainText();
    text += msg + "\n";
    textDocument->setPlainText(text);
    this->update(0, 0, this->width(), this->height());
}
