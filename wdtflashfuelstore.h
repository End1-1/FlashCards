#ifndef WDTFLASHFUELSTORE_H
#define WDTFLASHFUELSTORE_H

#include "c5grid.h"

class WdtFlashFuelStore : public C5Grid
{
    Q_OBJECT
public:
    WdtFlashFuelStore(const QIcon &icon, const QString &label, QWidget *parent = nullptr);
};

#endif // WDTFLASHFUELSTORE_H
