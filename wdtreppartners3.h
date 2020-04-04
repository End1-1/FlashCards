#ifndef WDTREPPARTNERS3_H
#define WDTREPPARTNERS3_H

#include "c5grid.h"

class WdtRepPartners3 : public C5Grid
{
    Q_OBJECT
public:
    WdtRepPartners3(const QIcon &icon, const QString &label, QWidget *parent = nullptr);

private slots:
    void filterFuel();
    void filterTicket();
};

#endif // WDTREPPARTNERS3_H
