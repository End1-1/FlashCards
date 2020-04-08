#ifndef FILTERPARTNER2_H
#define FILTERPARTNER2_H

#include "c5filterwidget.h"

namespace Ui {
class FilterPartner2;
}

class FilterPartner2 : public C5FilterWidget
{
    Q_OBJECT

public:
    explicit FilterPartner2(QWidget *parent = nullptr);
    ~FilterPartner2();
    virtual QString condition() override;
    void setFuelFilter(const QString &v);

private:
    Ui::FilterPartner2 *ui;
};

#endif // FILTERPARTNER2_H
