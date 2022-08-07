#ifndef FILTERFUELINOUT_H
#define FILTERFUELINOUT_H

#include "c5filterwidget.h"

namespace Ui {
class FilterFuelInOut;
}

class FilterFuelInOut : public C5FilterWidget
{
    Q_OBJECT

public:
    explicit FilterFuelInOut(QWidget *parent = nullptr);
    ~FilterFuelInOut();
    virtual QString condition();
    virtual QString conditionText();
    virtual QString filterString();

private:
    Ui::FilterFuelInOut *ui;
};

#endif // FILTERFUELINOUT_H
