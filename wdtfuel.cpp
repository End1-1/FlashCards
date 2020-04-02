#include "wdtfuel.h"
#include "dbdialog.h"
#include "wdbfuel.h"

WdtFuel::WdtFuel(const QIcon &icon, const QString &label, QWidget *parent) :
    C5Grid(icon, label, parent)
{
    buildReport("fuel");
    addAction(fEditActions, ":/res/new.png", tr("New fuel type"), this, SLOT(newFuelType()));
    addAction(fEditActions, ":/res/edit.png", tr("Edit fuel type"), this, SLOT(editFuelType()));
}

void WdtFuel::newFuelType()
{
    newElement<WdbFuel>();
}

void WdtFuel::editFuelType()
{
    editElement<WdbFuel>();
}

bool WdtFuel::tblDoubleClicked(int row, int column, const QList<QVariant> &values)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
    Q_UNUSED(values);
    editElement<WdbFuel>();
    return true;
}