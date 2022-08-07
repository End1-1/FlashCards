#ifndef WDTDEBTSHISTORY_H
#define WDTDEBTSHISTORY_H

#include "c5grid.h"

class FilterDebtsHistory;

class WdtDebtsHistory : public C5Grid
{
    Q_OBJECT
public:
    WdtDebtsHistory(const QIcon &icon, const QString &label, QWidget *parent = nullptr);
    void setFilter(const QDate &d1, const QDate &d2, int partner);

private:
    FilterDebtsHistory *fFilterHistory;
};

#endif // WDTDEBTSHISTORY_H
