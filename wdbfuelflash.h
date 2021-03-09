#ifndef WDBFUELFLASH_H
#define WDBFUELFLASH_H

#include "wdbwidget.h"

namespace Ui {
class WdbFuelFlash;
}

class WdbFuelFlash : public WdbWidget
{
    Q_OBJECT

public:
    explicit WdbFuelFlash(QWidget *parent = nullptr);
    ~WdbFuelFlash();
    virtual QString tableName() const;

private slots:
    void on_btnSetColor_clicked();
    void on_leColor_textChanged(const QString &arg1);

private:
    Ui::WdbFuelFlash *ui;
};

#endif // WDBFUELFLASH_H
