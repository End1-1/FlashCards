#ifndef WDTFUEL_H
#define WDTFUEL_H

#include "c5grid.h"


class WdtFuel : public C5Grid
{
    Q_OBJECT
public:
    WdtFuel(const QIcon &icon, const QString &label, QWidget *parent = nullptr);

private slots:
    void newFuelType();
    void editFuelType();
    virtual bool tblDoubleClicked(int row, int column, const QList<QVariant> &values) override;
};

#endif // WDTFUEL_H
