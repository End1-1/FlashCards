#ifndef FILTERALLTICKETS2_H
#define FILTERALLTICKETS2_H

#include "c5filterwidget.h"

namespace Ui {
class FilterAllTickets2;
}

class FilterAllTickets2 : public C5FilterWidget
{
    Q_OBJECT

public:
    explicit FilterAllTickets2(QWidget *parent = nullptr);
    ~FilterAllTickets2();
    virtual QString condition() override;
    void setFuelFilter(const QString &v);

private:
    Ui::FilterAllTickets2 *ui;
};

#endif // FILTERALLTICKETS2_H
