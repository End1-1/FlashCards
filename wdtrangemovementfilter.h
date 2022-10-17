#ifndef WDTRANGEMOVEMENTFILTER_H
#define WDTRANGEMOVEMENTFILTER_H

#include "c5filterwidget.h"

namespace Ui {
class WdtRangeMovementFilter;
}

class WdtRangeMovementFilter : public C5FilterWidget
{
    Q_OBJECT

public:
    explicit WdtRangeMovementFilter(QWidget *parent = nullptr);
    ~WdtRangeMovementFilter();
    virtual QString condition() override;
    virtual QString conditionText() override;
    virtual QString filterString() override;
    virtual QMap<QString, QString> replaces();
    QDate date1() const;
    QDate date2() const;
    int mode();

private:
    Ui::WdtRangeMovementFilter *ui;
};

#endif // WDTRANGEMOVEMENTFILTER_H
