#include "postgresql_connector.hxx"

QList<DVD> PostgreSQLConnector::query_dvd(const query_type& type, const QList<QString>& statements, const QList<QString>& statements_2, const QString& key_col, const QString& key_col_val)
{
    const QString _addr = _settings->value("login/server_address", "").toString();
    const QString _pwd = _settings->value("login/password", "").toString();
    const QString _dbname = _settings->value("login/database_name", "").toString();
    const QString _user = _settings->value("login/username", "").toString();
    const int _port = _settings->value("login/port", "").toInt();
    const QString _table_name = _settings->value("login/table_name", "").toString();

    soci::session* _session;
    QList<DVD> _dvd_results = {};

    try {
        _session = new soci::session(soci::postgresql, "host="+_addr.toStdString()+\
                                   " port="+std::to_string(_port)+\
                                   " user="+_user.toStdString()+\
                                   " password="+_pwd.toStdString()+\
                                   " dbname="+_dbname.toStdString());
    } catch (soci::soci_error) {
        QMessageBox::critical(0, "Invalid Database Credentials", "Invalid database access info:"
                                 "\nport: "+QString::number(_port)+
                                 "\nusername: "+_user+
                                 "\npassword: <hidden>"
                                 "\ndatabase name: "+_dbname);
        delete _session;
        return _dvd_results;
    }

    QString _sql_command = "";
    const QString _preparation = "SELECT * FROM "+_table_name;

    try {
         soci::rowset<soci::row> rs = (_session->prepare << _preparation.toStdString());
    } catch (soci::soci_error) {
        QMessageBox::critical(0, "Invalid Table", "Table "+_table_name+" not found.");
        delete _session;
        return _dvd_results;
    }

    soci::rowset<soci::row> rs = (_session->prepare << _preparation.toStdString());

    switch(type)
    {
        case query_type::SEARCH_EXACT:
            _sql_command = _search_table_exact(_table_name, statements[0], statements[1]);
            qDebug() << _sql_command;
            _log.push_back(_sql_command);
            rs = (_session->prepare << _sql_command.toStdString());
            break;
        case query_type::SEARCH_LIKE:
            _sql_command = _search_table_like(_table_name, statements[0], statements[1]);
            qDebug() << _sql_command;
            rs = (_session->prepare << _sql_command.toStdString());
            _log.push_back(_sql_command);
            break;
        case query_type::INSERT:
            _sql_command = _insert_into(_table_name, statements, statements_2);
            qDebug() << _sql_command;
            rs = (_session->prepare << _sql_command.toStdString());
            _log.push_back(_sql_command);
            return _dvd_results;
            break;
        case query_type::UPDATE:
            _sql_command = _update_entry_by_column(_table_name, key_col, key_col_val, statements, statements_2);
            qDebug() << _sql_command;
            rs = (_session->prepare << _sql_command.toStdString());
            _log.push_back(_sql_command);
            return _dvd_results;
            break;
        case query_type::DELETE:
            _sql_command = _delete_entry(_table_name, key_col, key_col_val);
            qDebug() << _sql_command;
            rs = (_session->prepare << _sql_command.toStdString());
            _log.push_back(_sql_command);
            return _dvd_results;
            break;
        default:
           return _dvd_results;
           break;

    }

    for(soci::rowset<soci::row>::const_iterator it = rs.begin(); it != rs.end(); ++it)
    {
        soci::row const& row = *it;

        QList<int> _null_cols = {};

        DVD _temp = {-1, "NULL", "NULL", -1, "NULL", "NULL", -1};

        for(std::size_t i{0}; i < row.size(); ++i)
        {
            if (row.get_indicator(i) == soci::i_null)
            {
                _null_cols.push_back(i);
            }
        }

        if(std::find(_null_cols.begin(), _null_cols.end(), 0) == _null_cols.end()) _temp.ID = row.get<int>(0);
        if(std::find(_null_cols.begin(), _null_cols.end(), 1) == _null_cols.end()) _temp.Title = QString::fromStdString(row.get<std::string>(1));
        if(std::find(_null_cols.begin(), _null_cols.end(), 2) == _null_cols.end()) _temp.Genre = QString::fromStdString(row.get<std::string>(2));
        if(std::find(_null_cols.begin(), _null_cols.end(), 3) == _null_cols.end()) _temp.Length = row.get<int>(3);
        if(std::find(_null_cols.begin(), _null_cols.end(), 4) == _null_cols.end()) _temp.Location = QString::fromStdString(row.get<std::string>(4));
        if(std::find(_null_cols.begin(), _null_cols.end(), 5) == _null_cols.end()) _temp.Notes = QString::fromStdString(row.get<std::string>(5));
        if(std::find(_null_cols.begin(), _null_cols.end(), 6) == _null_cols.end()) _temp.LastUpdated = row.get<std::tm>(6);
        _dvd_results.push_back(_temp);
    }

    return _dvd_results;

}

QString PostgreSQLConnector::_insert_into(const QString& table_name, const QStringList columns, const QStringList values)
{
    QString _out = QString("INSERT INTO ")+table_name+QString(" (");
    for(std::size_t i{0}; i < columns.size(); ++i)
    {
        _out += columns[i];
        _out += (i < columns.size()-1) ? ", " : ") ";
    }
    _out += QString(" VALUES (");
    for(std::size_t i{0}; i < values.size(); ++i)
    {
        _out += QString("'")+values[i]+QString("'");
        _out += (i < values.size()-1) ? ", " : ");";
    }
    return _out;
}

QString PostgreSQLConnector::_update_entry_by_column(const QString& table_name,const QString& key_col, const QString& key_col_val, const QStringList columns, const QStringList values)
{
    QString _out = QString("UPDATE ")+table_name+QString(" SET ");
    for(std::size_t i{0}; i < columns.size(); ++i)
    {
        _out += columns[i] + QString(" = '") + values[i] + QString("'");
        if(i != columns.size()-1) _out += QString(", ");
        _out += QString(" ");
    }
    if(key_col == "id") _out += QString(" WHERE id = '") + key_col_val + QString("';");
    else _out += QString(" WHERE UPPER(")+key_col+QString(") = UPPER('") + key_col_val + QString("');");

    return _out;
}
