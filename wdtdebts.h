#ifndef WDTDEBTS_H
#define WDTDEBTS_H

#include "c5grid.h"

class FilterDebts;

class WdtDebts : public C5Grid
{
    Q_OBJECT
public:
    WdtDebts(const QIcon &icon, const QString &label, QWidget *parent = nullptr);
    virtual void buildReport(const QString &name) override;

private:
    FilterDebts *fFilterDebts;

private slots:
    void editDoc();
    virtual void setSearchParameters() override;
};

#endif // WDTDEBTS_H
