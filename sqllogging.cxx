#include "sqllogging.hxx"
#include "ui_sqllogging.h"

SQLLogging::SQLLogging(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SQLLogging)
{
    ui->setupUi(this);
}

SQLLogging::~SQLLogging()
{
    delete ui;
}

void SQLLogging::setLogText(const QString& text)
{
    ui->sqlLogDisplay->setPlainText(text);
}
