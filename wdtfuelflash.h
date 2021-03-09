#ifndef WDTFUELFLASH_H
#define WDTFUELFLASH_H

#include "c5grid.h"

class WdtFuelFlash : public C5Grid
{
    Q_OBJECT
public:
    WdtFuelFlash(const QIcon &icon, const QString &label, QWidget *parent = nullptr);

private slots:
    void newFuelType();
    void editFuelType();
    virtual bool tblDoubleClicked(int row, int column, const QList<QVariant> &values) override;
};

#endif // WDTFUELFLASH_H
