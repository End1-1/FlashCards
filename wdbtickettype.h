#ifndef WDBTICKETTYPE_H
#define WDBTICKETTYPE_H

#include "wdbwidget.h"

namespace Ui {
class WdbTicketType;
}

class WdbTicketType : public WdbWidget
{
    Q_OBJECT

public:
    explicit WdbTicketType(QWidget *parent = nullptr);
    ~WdbTicketType();
    virtual QString tableName() const override;

private:
    Ui::WdbTicketType *ui;
};

#endif // WDBTICKETTYPE_H
