#ifndef MAINWINDOW_HXX
#define MAINWINDOW_HXX

#include <QMainWindow>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QInputDialog>
#include <QDate>
#include <QDebug>
#include <QColor>
#include <QList>
#include <QFileInfo>
#include <QDir>
#include <QOperatingSystemVersion>

#include <ctime>

#include "postgresql_connector.hxx"
#include "media_db_structs.hxx"
#include "sqllogging.hxx"
#include "about.hxx"
#include "preferences.hxx"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    const QString _version = "v2.1.2";
    const PostgreSQLConnector* getConnector() {return _connector;}

private slots:

    void on_lockEntry_stateChanged(int arg1);

    void on_updateButton_clicked();

    void on_actionAbout_triggered();

    void on_pushButtonDeleteID_clicked();

    void on_pushButtonDeleteTitle_clicked();

    void on_actionSQL_Logs_triggered();

    void on_actionPreferences_triggered();

private:
    Ui::MainWindow *ui;
    bool _is_locked = true;
    DVD _current_entry = {};
    QSettings* _settings;
    Preferences* _preferences;
    PostgreSQLConnector* _connector = nullptr;
    About* _about_window = new About(_version, this);
    SQLLogging* _sql_log_box = new SQLLogging(this);
    void _update_sql_log_box();
    bool _check_not_empty();
};
#endif // MAINWINDOW_HXX
