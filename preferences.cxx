#include "preferences.hxx"
#include "ui_preferences.h"

Preferences::Preferences(QWidget *parent, QSettings* settings) :
    QMainWindow(parent),
    ui(new Ui::Preferences),
    _parent(parent),
    _settings(settings)
{
    ui->setupUi(this);
}

Preferences::~Preferences()
{
    delete ui;
}

void Preferences::update_values()
{
    const QString _addr = _settings->value("login/server_address", "r").toString();
    const QString _pwd = _settings->value("login/password", "r").toString();
    const QString _dbname = _settings->value("login/database_name", "r").toString();
    const QString _user = _settings->value("login/username", "").toString();
    const int _port = _settings->value("login/port", "r").toInt();

    ui->portBox->setValue(_port);
    ui->pwdEdit->setText(_pwd);
    ui->dbnameEdit->setText(_dbname);
    ui->serverEdit->setText(_addr);
    ui->userEdit->setText(_user);
}

void Preferences::on_applyButton_clicked()
{
    const int _port = ui->portBox->value();
    const QString _pwd = ui->pwdEdit->text();
    const QString _dbname = ui->dbnameEdit->text();
    const QString _addr = ui->serverEdit->text();
    const QString _user = ui->userEdit->text();

    _settings->setValue("login/port", _port);
    _settings->setValue("login/server_address", _addr);
    _settings->setValue("login/password", _pwd);
    _settings->setValue("login/username", _user);
    _settings->setValue("login/database_name", _dbname);

    QMessageBox::information(0, "Settings Updated", "Updated Config");
}
