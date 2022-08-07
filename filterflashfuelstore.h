#ifndef FILTERFLASHFUELSTORE_H
#define FILTERFLASHFUELSTORE_H

#include "c5filterwidget.h"

namespace Ui {
class FilterFlashFuelStore;
}

class FilterFlashFuelStore : public C5FilterWidget
{
    Q_OBJECT

public:
    explicit FilterFlashFuelStore(QWidget *parent = nullptr);
    ~FilterFlashFuelStore();
    virtual QString condition() override;
    virtual QString conditionText() override;
    virtual QString filterString() override;

private:
    Ui::FilterFlashFuelStore *ui;
};

#endif // FILTERFLASHFUELSTORE_H
