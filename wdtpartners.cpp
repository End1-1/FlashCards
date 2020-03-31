#include "wdtpartners.h"
#include "dbdialog.h"
#include "wdbpartner.h"

WdtPartners::WdtPartners(const QIcon &icon, const QString &label, QWidget *parent) :
    C5Grid(icon, label, parent)
{
    buildReport("partners");
    addAction(fEditActions, ":/res/new.png", tr("New partner"), this, SLOT(newPartner()));
    addAction(fEditActions, ":/res/edit.png", tr("Edit partner"), this, SLOT(editPartner()));
}

void WdtPartners::newPartner()
{
    DbDialog::createDialog<WdbPartner>();
}

void WdtPartners::editPartner()
{
    int id = rowId();
    DbDialog::createDialog<WdbPartner>(id);
}

bool WdtPartners::tblDoubleClicked(int row, int column, const QList<QVariant> &values)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
    Q_UNUSED(values);
    int id = rowId();
    DbDialog::createDialog<WdbPartner>(id);
    return true;
}
