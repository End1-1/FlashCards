#ifndef WDTCARDTYPES_H
#define WDTCARDTYPES_H

#include "c5grid.h"

class WdtCardTypes : public C5Grid
{
    Q_OBJECT
public:
    WdtCardTypes(const QIcon &icon, const QString &label, QWidget *parent = nullptr);

private slots:
    void newCardType();
    void editTicketType();
    virtual bool tblDoubleClicked(int row, int column, const QList<QVariant> &values) override;
};

#endif // WDTCARDTYPES_H
