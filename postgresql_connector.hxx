#ifndef POSTGRESQLCONNECTOR_HXX
#define POSTGRESQLCONNECTOR_HXX

#include "soci/soci.h"
#include "soci-postgresql.h"
#include "soci/use-type.h"

#include "media_db_structs.hxx"

#include <string>

#include <QString>
#include <QList>
#include <QMessageBox>
#include <vector>
#include <QDebug>
#include <QSettings>

enum class query_type
{
    INSERT,
    UPDATE,
    SEARCH_EXACT,
    SEARCH_LIKE,
    DELETE
};

class PostgreSQLConnector
{
    private:
        const QSettings* _settings = nullptr;
        soci::session _session;
        QStringList _log = {};
        QString _search_table_exact(const QString& table_name, const QString& column, const QString& search_term)
        {
            return QString("SELECT * FROM ")+table_name+QString(" WHERE UPPER(")+column+QString(") = UPPER('")+search_term+QString("');");
        }
        QString _search_table_like(const QString& table_name, const QString& column, const QString& search_term)
        {
            return QString("SELECT * FROM ")+table_name+QString(" WHERE UPPER(")+column+QString(") LIKE UPPER('%")+search_term+QString("%');");
        }

        QString _insert_into(const QString& table_name, const QStringList columns, const QStringList values);

        QString _delete_entry(const QString& table_name, const QString& key_col, const QString& key_val)
        {
            if(key_col == "id") return QString("DELETE FROM ")+table_name+QString(" WHERE id = '")+key_val+QString("';");
            return QString("DELETE FROM ")+table_name+QString(" WHERE UPPER(")+key_col+QString(") = UPPER('")+key_val+QString("');");
        }

        QString _update_entry_by_column(const QString& table_name,const QString& key_col, const QString& key_col_val, const QStringList columns, const QStringList values);

    public:
        PostgreSQLConnector(const QSettings* settings) : _settings(settings) {}

        QList<DVD> query_dvd(const query_type& type, const QList<QString>& statements, const QList<QString>& statements_2={}, const QString& key_col="", const QString& key_col_val="");
        void addSQLLogEntry(const QString& entry){_log.push_back(entry);}
        QStringList getSQLLog() const {return _log;}
};

#endif // POSTGRESQLCONNECTOR_HXX
