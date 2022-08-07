#ifndef FILTERDEBTS_H
#define FILTERDEBTS_H

#include "c5filterwidget.h"

namespace Ui {
class FilterDebts;
}

class FilterDebts : public C5FilterWidget
{
    Q_OBJECT

public:
    explicit FilterDebts(QWidget *parent = nullptr);
    ~FilterDebts();
    virtual QString condition();
    virtual QString conditionText();
    virtual QString filterString();
    QString c1() const;
    QString c2() const;

private:
    Ui::FilterDebts *ui;
};

#endif // FILTERDEBTS_H
