#include "wdtcardtypes.h"
#include "wdbtickettype.h"
#include "dbdialog.h"

WdtCardTypes::WdtCardTypes(const QIcon &icon, const QString &label, QWidget *parent) :
    C5Grid(icon, label, parent)
{
    buildReport("cards_types");
    addAction(fEditActions, ":/res/new.png", tr("New ticket type"), this, SLOT(newCardType()));
    addAction(fEditActions, ":/res/edit.png", tr("Edit ticket type"), this, SLOT(editTicketType()));
}

void WdtCardTypes::newCardType()
{
    newElement<WdbTicketType>();
}

void WdtCardTypes::editTicketType()
{
    editElement<WdbTicketType>();
}

bool WdtCardTypes::tblDoubleClicked(int row, int column, const QList<QVariant> &values)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
    Q_UNUSED(values);
    editElement<WdbTicketType>();
    return true;
}
