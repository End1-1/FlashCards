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

private:
    Ui::WdbFuel *ui;
};

#endif // WDBFUEL_H
