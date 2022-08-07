#ifndef FILTERFUELFLASH_H
#define FILTERFUELFLASH_H

#include "c5filterwidget.h"

namespace Ui {
class FilterFuelFlash;
}

class FilterFuelFlash : public C5FilterWidget
{
    Q_OBJECT

public:
    explicit FilterFuelFlash(QWidget *parent = nullptr);
    ~FilterFuelFlash();
    virtual QString condition() override;
    virtual QString conditionText() override;
    virtual QString filterString() override;

private:
    Ui::FilterFuelFlash *ui;
};

#endif // FILTERFUELFLASH_H
