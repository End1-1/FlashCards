#ifndef WDTFUELOUTIN_H
#define WDTFUELOUTIN_H

#include "c5grid.h"

class WdtFuelOutIn : public C5Grid
{
    Q_OBJECT
public:
    WdtFuelOutIn(const QIcon &icon, const QString &label, QWidget *parent = nullptr);
};

#endif // WDTFUELOUTIN_H
