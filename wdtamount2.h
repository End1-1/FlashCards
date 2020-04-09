#ifndef WDTAMOUNT2_H
#define WDTAMOUNT2_H

#include "c5grid.h"

class WdtAmount2 : public C5Grid
{
    Q_OBJECT
public:
    WdtAmount2(const QIcon &icon, const QString &label, QWidget *parent = nullptr);

private slots:
    void filterFuel();
    void filterTicket();
};

#endif // WDTAMOUNT2_H
