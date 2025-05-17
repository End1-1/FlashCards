#ifndef WDTTOTALREPORTFILTER_H
#define WDTTOTALREPORTFILTER_H

#include "c5filterwidget.h"

namespace Ui {
class WdtTotalReportFilter;
}

class WdtTotalReportFilter : public C5FilterWidget
{
    Q_OBJECT

public:
    explicit WdtTotalReportFilter(QWidget *parent = nullptr);
    ~WdtTotalReportFilter();
    virtual QString condition() override;
    virtual QString conditionText();
    virtual QString filterString();

    QDate d1();
    QDate d2();

private:
    Ui::WdtTotalReportFilter *ui;
};

#endif // WDTTOTALREPORTFILTER_H
