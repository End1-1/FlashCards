#ifndef WDTREPPARTNERS1_H
#define WDTREPPARTNERS1_H

#include "c5grid.h"

class WdtRepPartners1 : public C5Grid
{
    Q_OBJECT
public:
    WdtRepPartners1(const QIcon &icon, const QString &label, QWidget *parent = nullptr);

private slots:
    void filterFuel();
};

#endif // WDTREPPARTNERS1_H
