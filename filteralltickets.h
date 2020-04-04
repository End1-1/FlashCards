#ifndef FILTERALLTICKETS_H
#define FILTERALLTICKETS_H

#include "c5filterwidget.h"

namespace Ui {
class FilterAllTickets;
}

class FilterAllTickets : public C5FilterWidget
{
    Q_OBJECT

public:
    explicit FilterAllTickets(QWidget *parent = nullptr);
    ~FilterAllTickets();
    virtual QString condition() override;
    void setFuelFilter(const QString &v);
    void setTicketFilter(const QString &v);
    void setStateFilter(const QString &v);

private:
    Ui::FilterAllTickets *ui;
};

#endif // FILTERALLTICKETS_H
