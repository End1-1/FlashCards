#ifndef WDTFUELPRICES_H
#define WDTFUELPRICES_H

#include "c5grid.h"

class WdtFuelPrices : public C5Grid
{
    Q_OBJECT
public:
    WdtFuelPrices(const QIcon &icon, const QString &label, QWidget *parent = nullptr);

private slots:
    void newPrices();
    void editPrices();
};

#endif // WDTFUELPRICES_H
