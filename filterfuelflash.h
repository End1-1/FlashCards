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

private:
    Ui::FilterFuelFlash *ui;
};

#endif // FILTERFUELFLASH_H
