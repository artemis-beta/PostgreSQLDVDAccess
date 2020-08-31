#ifndef SQLLOGGING_HXX
#define SQLLOGGING_HXX

#include <QDialog>

namespace Ui {
class SQLLogging;
}

class SQLLogging : public QDialog
{
    Q_OBJECT

public:
    explicit SQLLogging(QWidget *parent = nullptr);
    ~SQLLogging();
    void setLogText(const QString& text);

private:
    Ui::SQLLogging *ui;
};

#endif // SQLLOGGING_HXX
