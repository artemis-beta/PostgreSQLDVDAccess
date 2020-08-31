#include "mainwindow.hxx"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lockEntry->setChecked(true);
    ui->searchResults->setColumnCount(5);
    ui->searchResults->setColumnWidth(0, 60);
    ui->searchResults->setColumnWidth(1, 400);
    ui->searchResults->setColumnWidth(2, 150);
    ui->searchResults->setColumnWidth(3, 150);
    ui->searchResults->setColumnWidth(4, 100);
    ui->searchResults->setStyleSheet(QString::fromUtf8("QTableWidget:disabled"
                                                       "{color:black}"));
    QTableWidgetItem* _header = new QTableWidgetItem("ID", 0);
    _header->setTextColor("black");
    QTableWidgetItem* _header_1 = new QTableWidgetItem("Title", 0);
    _header_1->setTextColor("black");
    QTableWidgetItem* _header_2 = new QTableWidgetItem("Genre", 0);
    _header_2->setTextColor("black");
    QTableWidgetItem* _header_3 = new QTableWidgetItem("Location", 0);
    _header_3->setTextColor("black");
    QTableWidgetItem* _header_4 = new QTableWidgetItem("Last Updated", 0);
    _header_4->setTextColor("black");
    ui->searchResults->setHorizontalHeaderItem(0, _header);
    ui->searchResults->setHorizontalHeaderItem(1, _header_1);
    ui->searchResults->setHorizontalHeaderItem(2, _header_2);
    ui->searchResults->setHorizontalHeaderItem(3, _header_3);
    ui->searchResults->setHorizontalHeaderItem(4, _header_4);
    ui->comboBoxLocation->setCurrentIndex(0);

    QOperatingSystemVersion::OSType current_os = QOperatingSystemVersion::current().type();

    QString _config_file;

    switch(current_os)
    {
        case QOperatingSystemVersion::MacOS:
            _config_file = QDir::homePath()+"/Library/Application\ Support/psql_media_db_config.ini";
            break;
        case QOperatingSystemVersion::Windows:
            _config_file = QDir::homePath()+"/AppData/psql_media_db_config.ini";
            break;
        default:
            _config_file = QDir::homePath()+"/Documents/psql_media_db_config.ini";
    }

    _settings = new QSettings(_config_file, QSettings::IniFormat);
    if(!QFileInfo(_config_file).exists())
    {
        _settings->setValue("login/port", 5432);
        _settings->setValue("login/server_address", "192.168.1.81");
        _settings->setValue("login/password", "password");
        _settings->setValue("login/username", "user");
        _settings->setValue("login/database_name", "database");
        _settings->setValue("extra/description", "");
    }

    QString _desc = _settings->value("extra/description", "").toString();

    _about_window->setDescription(_desc);

    _preferences = new Preferences(this, _settings);
    _connector = new PostgreSQLConnector(_settings);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_lockEntry_stateChanged(int arg1)
{
    _is_locked = bool(arg1);
    ui->notesEdit->setEnabled(!_is_locked);
    ui->modeIndicator->setText((_is_locked) ? "Search Mode" : "Edit Mode");
    ui->NotesOrSearch->setText((_is_locked) ? "Results" : "Notes");
    ui->notesEdit->setVisible(!_is_locked);
    ui->notesEdit->setEnabled(!_is_locked);
    ui->searchResults->setVisible(_is_locked);
    ui->genreEdit->setVisible(!_is_locked);
    ui->comboBoxLocation->setVisible(!_is_locked);
    ui->comboBoxLocation->setCurrentIndex(0);
    ui->searchResults->clear();
    ui->titleEdit->clear();
    ui->genreEdit->clear();
    ui->notesEdit->clear();
    ui->updateButton->setText((_is_locked) ? "Search" : "Update");
    ui->genreLabel->setVisible(!_is_locked);
    ui->locationLabel->setVisible(!_is_locked);
    ui->hint->setVisible(_is_locked);
    ui->spinBoxID->setVisible(!_is_locked);
    ui->spinBoxID->clear();
    ui->labelID->setVisible(!_is_locked);
    ui->pushButtonDeleteID->setVisible(!_is_locked);
    ui->pushButtonDeleteID->setEnabled(!_is_locked);
    ui->pushButtonDeleteTitle->setVisible(!_is_locked);
    ui->pushButtonDeleteTitle->setEnabled(!_is_locked);
}

void MainWindow::on_updateButton_clicked()
{
    if(_is_locked)
    {
        ui->searchResults->clear();
        ui->searchResults->setRowCount(0);
        QTableWidgetItem* _header = new QTableWidgetItem("ID", 0);
        _header->setTextColor("black");
        QTableWidgetItem* _header_1 = new QTableWidgetItem("Title", 0);
        _header_1->setTextColor("black");
        QTableWidgetItem* _header_2 = new QTableWidgetItem("Genre", 0);
        _header_2->setTextColor("black");
        QTableWidgetItem* _header_3 = new QTableWidgetItem("Location", 0);
        _header_3->setTextColor("black");
        QTableWidgetItem* _header_4 = new QTableWidgetItem("Last Updated", 0);
        _header_4->setTextColor("black");
        ui->searchResults->setHorizontalHeaderItem(0, _header);
        ui->searchResults->setHorizontalHeaderItem(1, _header_1);
        ui->searchResults->setHorizontalHeaderItem(2, _header_2);
        ui->searchResults->setHorizontalHeaderItem(3, _header_3);
        ui->searchResults->setHorizontalHeaderItem(4, _header_4);
        QString _title = ui->titleEdit->text(),
                _genre = ui->genreEdit->text(),
                _location = ui->comboBoxLocation->currentText(),
                _notes = ui->notesEdit->toPlainText();

        QList<DVD> _results = _connector->query_dvd(query_type::SEARCH_LIKE, {QString("title"), _title});

        QString _out_str = "Title\tGenre\tLocation\tLast Updated\n";

        for(auto dvd : _results)
        {
             ui->searchResults->insertRow(ui->searchResults->rowCount());
             QTableWidgetItem* _new_service_item = new QTableWidgetItem(QString::number(dvd.ID), 0);
             _new_service_item->setFlags(_new_service_item->flags() ^ Qt::ItemIsEditable);
             ui->searchResults->setItem(ui->searchResults->rowCount()-1, 0, _new_service_item);
             QTableWidgetItem* _new_service_item1 = new QTableWidgetItem(dvd.Title, 0);
             _new_service_item1->setFlags(_new_service_item1->flags() ^ Qt::ItemIsEditable);
             ui->searchResults->setItem(ui->searchResults->rowCount()-1, 1, _new_service_item1);
             QTableWidgetItem* _new_service_item2 = new QTableWidgetItem(dvd.Genre, 0);
             _new_service_item2->setFlags(_new_service_item2->flags() ^ Qt::ItemIsEditable);
             ui->searchResults->setItem(ui->searchResults->rowCount()-1, 2, _new_service_item2);
             QTableWidgetItem* _new_service_item3 = new QTableWidgetItem(dvd.Location, 0);
             _new_service_item3->setFlags(_new_service_item3->flags() ^ Qt::ItemIsEditable);
             ui->searchResults->setItem(ui->searchResults->rowCount()-1, 3, _new_service_item3);
             QString _l_updated = (dvd.LastUpdated.tm_mday < 10) ? "0" : "";
                     _l_updated += QString::number(dvd.LastUpdated.tm_mday);
                     _l_updated += "-";
                     _l_updated += (dvd.LastUpdated.tm_mon < 9) ? "0" : "";
                     _l_updated += QString::number(dvd.LastUpdated.tm_mon+1)+"-"+QString::number(dvd.LastUpdated.tm_year+1900);
             QTableWidgetItem* _new_service_item4 = new QTableWidgetItem(_l_updated, 0);
             _new_service_item4->setFlags(_new_service_item4->flags() ^ Qt::ItemIsEditable);
             ui->searchResults->setItem(ui->searchResults->rowCount()-1, 4, _new_service_item4);
        }
    }

    else
    {
        bool _new_entry =  _connector->query_dvd(query_type::SEARCH_EXACT, {QString("title"), ui->titleEdit->text()}).length() == 0;
        std::time_t time = std::time(0);
        std::tm* now = localtime(&time);

        QString _l_updated = (now->tm_mday < 10) ? "0" : "";
                _l_updated += QString::number(now->tm_mday);
                _l_updated += "-";
                _l_updated += (now->tm_mon < 9) ? "0" : "";
                _l_updated += QString::number(now->tm_mon+1)+"-"+QString::number(now->tm_year+1900);

        if(ui->spinBoxID->value() != 0)
        {
            QStringList _cols = {};
            QStringList _vals = {};

            if(!ui->titleEdit->text().isEmpty())
            {
                _cols.push_back("title");
                _vals.push_back(ui->titleEdit->text());
            }

            if(!ui->genreEdit->text().isEmpty())
            {
                _cols.push_back("genre");
                _vals.push_back(ui->genreEdit->text());
            }

            if(!ui->comboBoxLocation->currentIndex() == 0)
            {
                _cols.push_back("location");
                _vals.push_back(ui->comboBoxLocation->currentText());
            }

            if(!ui->notesEdit->toPlainText().isEmpty())
            {
                _cols.push_back("notes");
                _vals.push_back(ui->notesEdit->toPlainText());
            }

            _cols.push_back("updated");
            _vals.push_back(_l_updated);

            _connector->query_dvd(query_type::UPDATE, _cols, _vals, "id", QString::number(ui->spinBoxID->value()));
            ui->lockEntry->setChecked(true);
            ui->updateButton->click();
        }

        else if(!ui->titleEdit->text().isEmpty() && !_new_entry)
        {
            QStringList _cols = {};
            QStringList _vals = {};

            if(!ui->genreEdit->text().isEmpty())
            {
                _cols.push_back("genre");
                _vals.push_back(ui->genreEdit->text());
            }

            if(ui->comboBoxLocation->currentIndex() == 0)
            {
                QMessageBox::critical(this, QObject::tr("Invalid Input"), QObject::tr("No location specified."));
                return;
            }

            _cols.push_back("location");
            _vals.push_back(ui->comboBoxLocation->currentText());

            if(!ui->notesEdit->toPlainText().isEmpty())
            {
                _cols.push_back("notes");
                _vals.push_back(ui->notesEdit->toPlainText());
            }

            _cols.push_back("updated");
            _vals.push_back(_l_updated);

            _connector->query_dvd(query_type::UPDATE, _cols, _vals, "title", ui->titleEdit->text());
            ui->lockEntry->setChecked(true);
            ui->updateButton->click();
        }

        else
        {
            QStringList _cols = {};
            QStringList _vals = {};

            _cols.push_back("title");
            _vals.push_back(ui->titleEdit->text());

            if(!ui->genreEdit->text().isEmpty())
            {
                _cols.push_back("genre");
                _vals.push_back(ui->genreEdit->text());
            }

            if(ui->comboBoxLocation->currentIndex() == 0)
            {
                QMessageBox::critical(this, QObject::tr("Invalid Input"), QObject::tr("No location specified."));
                return;
            }

            _cols.push_back("location");
            _vals.push_back(ui->comboBoxLocation->currentText());

            if(!ui->notesEdit->toPlainText().isEmpty())
            {
                _cols.push_back("notes");
                _vals.push_back(ui->notesEdit->toPlainText());
            }

            _cols.push_back("updated");
            _vals.push_back(_l_updated);

            _connector->query_dvd(query_type::INSERT, _cols, _vals);
            ui->lockEntry->setChecked(true);
            ui->updateButton->click();
        }

        ui->titleEdit->clear();
        ui->genreEdit->clear();
        ui->notesEdit->clear();
     }

}

bool MainWindow::_check_not_empty()
{
    return false;
}

void MainWindow::on_actionAbout_triggered()
{
    _about_window->show();
}

void MainWindow::on_pushButtonDeleteID_clicked()
{
    bool _delete;
    int ID = QInputDialog::getInt(this, tr("Delete by ID"), tr("Enter ID: "), 0, 0, 99999, 1, &_delete);
    if(_delete && !ID <= 0)
    {
        _connector->query_dvd(query_type::DELETE, {}, {}, "id", QString::number(ID));
    }
}

void MainWindow::on_pushButtonDeleteTitle_clicked()
{
    bool _delete;
    QString title = QInputDialog::getText(this, tr("Delete by Title"), tr("Enter Title: "), QLineEdit::Normal, QString(), &_delete);
    if(_delete && !title.isEmpty())
    {
        _connector->query_dvd(query_type::DELETE, {}, {}, "title", title);
    }
}

void MainWindow::_update_sql_log_box()
{
    QString _out = "";

    for(auto statement : _connector->getSQLLog())
    {
        _out += statement + "\n";
    }

    _sql_log_box->setLogText(_out);
}

void MainWindow::on_actionSQL_Logs_triggered()
{
    _update_sql_log_box();
    _sql_log_box->show();
}

void MainWindow::on_actionPreferences_triggered()
{
    _preferences->show();
    _preferences->update_values();
}
