#ifndef C5FILTERAUTOINPUTFLASH_H
#define C5FILTERAUTOINPUTFLASH_H

#include "c5filterwidget.h"

namespace Ui {
class C5FilterAutoinputFlash;
}

class C5FilterAutoinputFlash : public C5FilterWidget
{
    Q_OBJECT

public:
    explicit C5FilterAutoinputFlash(QWidget *parent = nullptr);

    ~C5FilterAutoinputFlash();

    virtual QString condition() override;

    virtual QString conditionText() override;

    virtual QString filterString() override;

private:
    Ui::C5FilterAutoinputFlash *ui;
};

#endif // C5FILTERAUTOINPUTFLASH_H
