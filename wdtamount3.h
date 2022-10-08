#ifndef WDTAMOUNT3_H
#define WDTAMOUNT3_H

#include "c5grid.h"

class WdtAmount3 : public C5Grid
{
    Q_OBJECT
public:
    WdtAmount3(const QIcon &icon, const QString &label, QWidget *parent = nullptr);
};

#endif // WDTAMOUNT3_H
