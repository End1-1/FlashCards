#ifndef FILTERTICKETSBYONE_H
#define FILTERTICKETSBYONE_H

#include "c5filterwidget.h"

namespace Ui {
class FilterTicketsByOne;
}

class FilterTicketsByOne : public C5FilterWidget
{
    Q_OBJECT

public:
    explicit FilterTicketsByOne(QWidget *parent = nullptr);
    ~FilterTicketsByOne();
    virtual QString condition() override;
    void setDateMode(int mode);

private:
    Ui::FilterTicketsByOne *ui;
    int fMode;
};

#endif // FILTERTICKETSBYONE_H
