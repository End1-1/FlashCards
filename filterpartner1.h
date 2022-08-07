#ifndef FILTERPARTNER1_H
#define FILTERPARTNER1_H

#include "c5filterwidget.h"

namespace Ui {
class FilterPartner1;
}

class FilterPartner1 : public C5FilterWidget
{
    Q_OBJECT

public:
    explicit FilterPartner1(QWidget *parent = nullptr);
    ~FilterPartner1();
    virtual QString condition() override;
    virtual QString conditionText() override;
    virtual QString filterString() override;
    void setTicketFilter(const QString &v);

private:
    Ui::FilterPartner1 *ui;
};

#endif // FILTERPARTNER1_H
