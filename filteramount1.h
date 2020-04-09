#ifndef FILTERAMOUNT1_H
#define FILTERAMOUNT1_H

#include "c5filterwidget.h"

namespace Ui {
class FilterAmount1;
}

class FilterAmount1 : public C5FilterWidget
{
    Q_OBJECT

public:
    explicit FilterAmount1(QWidget *parent = nullptr);
    ~FilterAmount1();
    virtual QString condition();
    void setFuel(const QString &v);
private:
    Ui::FilterAmount1 *ui;
};

#endif // FILTERAMOUNT1_H
