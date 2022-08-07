#ifndef FILTERAMOUNT2_H
#define FILTERAMOUNT2_H

#include "c5filterwidget.h"

namespace Ui {
class FilterAmount2;
}

class FilterAmount2 : public C5FilterWidget
{
    Q_OBJECT

public:
    explicit FilterAmount2(QWidget *parent = nullptr);
    ~FilterAmount2();
    virtual QString condition() override;
    virtual QString conditionText() override;
    virtual QString filterString() override;
    void setFuel(const QString &v);
    void setTicket(const QString &v);

private:
    Ui::FilterAmount2 *ui;
};

#endif // FILTERAMOUNT2_H
