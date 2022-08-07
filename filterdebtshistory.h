#ifndef FILTERDEBTSHISTORY_H
#define FILTERDEBTSHISTORY_H

#include "c5filterwidget.h"

namespace Ui {
class FilterDebtsHistory;
}

class FilterDebtsHistory : public C5FilterWidget
{
    Q_OBJECT

public:
    explicit FilterDebtsHistory(QWidget *parent = nullptr);
    ~FilterDebtsHistory();
    virtual QString condition();
    virtual QString conditionText();
    virtual QString filterString();
    void setFilter(const QDate &d1, const QDate &d2, int partner);

private:
    Ui::FilterDebtsHistory *ui;
};

#endif // FILTERDEBTSHISTORY_H
