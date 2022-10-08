#ifndef WDTAMOUNT3FILTER_H
#define WDTAMOUNT3FILTER_H

#include "c5filterwidget.h"

namespace Ui {
class WdtAmount3Filter;
}

class WdtAmount3Filter : public C5FilterWidget
{
    Q_OBJECT

public:
    explicit WdtAmount3Filter(QWidget *parent = nullptr);
    ~WdtAmount3Filter();
    virtual QString condition() override;
    virtual QString conditionText() override;
    virtual QString filterString() override;
    virtual QMap<QString, QString> replaces() override;

private:
    Ui::WdtAmount3Filter *ui;
};

#endif // WDTAMOUNT3FILTER_H
