#ifndef WDTAMOUNT1_H
#define WDTAMOUNT1_H

#include "c5grid.h"

class WdtAmount1 : public C5Grid
{
    Q_OBJECT
public:
    WdtAmount1(const QIcon &icon, const QString &label, QWidget *parent = nullptr);

private slots:
    void filterFuel();
};

#endif // WDTAMOUNT1_H
