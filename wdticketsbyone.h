#ifndef WDTICKETSBYONE_H
#define WDTICKETSBYONE_H

#include "c5grid.h"

class WdTicketsByOne : public C5Grid
{
    Q_OBJECT
public:
    WdTicketsByOne(const QIcon &icon, const QString &label, QWidget *parent = nullptr);
    void setFilterMode(int mode);
};

#endif // WDTICKETSBYONE_H
