#include "about.hxx"
#include "ui_about.h"

About::About(const QString& version, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    ui->labelVersion->setText(version);
}

About::~About()
{
    delete ui;
}

void About::setVersion(const QString& version)
{
       ui->labelVersion->setText(version);
}

void About::setDescription(QString &desc)
{
    if(desc.isEmpty())
    {
        desc = "DVD Media";
    }
    ui->descriptionLabel->setText(desc+" Database Search Engine");
}
