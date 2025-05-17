#ifndef WDTTOTALREPORT_H
#define WDTTOTALREPORT_H

#include "c5grid.h"

class WdtTotalReportFilter;

class WdtTotalReport : public C5Grid
{
    Q_OBJECT
public:
    WdtTotalReport(const QIcon &icon, const QString &label, QWidget *parent = nullptr);

public slots:
    virtual void buildQuery() override;

private:
    WdtTotalReportFilter *fFilter;
};

#endif // WDTTOTALREPORT_H
