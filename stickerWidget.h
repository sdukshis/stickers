#ifndef STICKER_WIDGET_H
#define STICKER_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QTextStream>
#include <QDebug>
#include <QAxObject>
#include <QString>
#include <QStringList>

#include "sticker.h"
#include "painter.h"

const int WINDOW_MIN_WIDTH  = 600;
const int WINDOW_MIN_HEIGHT = 800;

class StickerWidget: public QWidget {
Q_OBJECT
public:
    StickerWidget(QWidget* parent = 0);
    virtual ~StickerWidget();
public slots:
    void chooseInput();
    void chooseOutput();
    void run();

private:
    QList<QStringList> read_xls(const QString& filename);
    QList<Sticker> createStickers(const QList<QStringList>& table);
    int paintAllSticker(const QList<Sticker>& stickers);
    void printMessage(const QString& msg);

    QString inputFileName;
    QString outputDirName;
    QVBoxLayout *mainLayout;

    QHBoxLayout *inputFileLayout;
    QPushButton *inputFileButton;
    QLineEdit* inputFileLine;

    QHBoxLayout *outputDirLayout;
    QPushButton *outputDirButton;
    QLineEdit* outputDirLine;

    QTextEdit* progressEdit;

    QHBoxLayout* runLayout;
    QPushButton* runButton;    

};

#endif