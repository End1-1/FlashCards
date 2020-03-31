#ifndef WDTALLTICKETS_H
#define WDTALLTICKETS_H

#include "c5grid.h"

class WdtAllTickets : public C5Grid
{
    Q_OBJECT
public:
    WdtAllTickets(const QIcon &icon, const QString &label, QWidget *parent = nullptr);
};

#endif // WDTALLTICKETS_H
