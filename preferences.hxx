#ifndef PREFERENCES_HXX
#define PREFERENCES_HXX

#include <QMainWindow>
#include <QSettings>
#include <QMessageBox>

namespace Ui {
class Preferences;
}

class Preferences : public QMainWindow
{
    Q_OBJECT

public:
    explicit Preferences(QWidget *parent = nullptr, QSettings* settings = nullptr);
    ~Preferences();
    void update_values();

private slots:
    void on_applyButton_clicked();

private:
    Ui::Preferences *ui;
    QWidget* _parent = nullptr;
    QSettings* _settings = nullptr;
};

#endif // PREFERENCES_HXX
