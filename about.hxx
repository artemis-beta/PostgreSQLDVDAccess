#ifndef ABOUT_HXX
#define ABOUT_HXX

#include <QDialog>

namespace Ui {
class About;
}

class About : public QDialog
{
    Q_OBJECT

public:
    explicit About(const QString& version, QWidget *parent = nullptr);
    ~About();
    void setVersion(const QString& version);
    void setDescription(QString& desc);

private:
    Ui::About *ui;
};

#endif // ABOUT_HXX
