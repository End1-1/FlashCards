#ifndef WDBFUEL_H
#define WDBFUEL_H

#include "wdbwidget.h"

namespace Ui {
class WdbFuel;
}

class WdbFuel : public WdbWidget
{
    Q_OBJECT

public:
    explicit WdbFuel(QWidget *parent = nullptr);
    ~WdbFuel();
    virtual QString tableName() const;

private slots:
    void on_btnSetColor_clicked();
    void on_leColor_textChanged(const QString &arg1);

private:
    Ui::WdbFuel *ui;
};

#endif // WDBFUEL_H
