#ifndef FILTERPARTNER3_H
#define FILTERPARTNER3_H

#include "c5filterwidget.h"

namespace Ui {
class FilterPartner3;
}

class FilterPartner3 : public C5FilterWidget
{
    Q_OBJECT

public:
    explicit FilterPartner3(QWidget *parent = nullptr);
    ~FilterPartner3();
    virtual QString condition() override;
    void setFuelFilter(const QString &v);
    void setTicketFilter(const QString &v);

private:
    Ui::FilterPartner3 *ui;
};

#endif // FILTERPARTNER3_H
