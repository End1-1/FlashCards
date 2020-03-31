#ifndef WDTPARTNERS_H
#define WDTPARTNERS_H

#include "c5grid.h"

class WdtPartners : public C5Grid
{
    Q_OBJECT
public:
    WdtPartners(const QIcon &icon, const QString &label, QWidget *parent = nullptr);

private slots:
    void newPartner();
    void editPartner();
    virtual bool tblDoubleClicked(int row, int column, const QList<QVariant> &values) override;
};

#endif // WDTPARTNERS_H
