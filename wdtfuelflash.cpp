#include "wdtfuelflash.h"
#include "wdbfuelflash.h"

WdtFuelFlash::WdtFuelFlash(const QIcon &icon, const QString &label, QWidget *parent) :
    C5Grid(icon, label, parent)
{
    fColorColumn = "fcolor";
    fHiddenColumns.append("fcolor");
    buildReport("fuel_flash");
    addAction(fEditActions, ":/res/new.png", tr("New fuel type"), this, SLOT(newFuelType()));
    addAction(fEditActions, ":/res/edit.png", tr("Edit fuel type"), this, SLOT(editFuelType()));
}

void WdtFuelFlash::newFuelType()
{
    newElement<WdbFuelFlash>();
}

void WdtFuelFlash::editFuelType()
{
    editElement<WdbFuelFlash>();
}

bool WdtFuelFlash::tblDoubleClicked(int row, int column, const QList<QVariant> &values)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
    Q_UNUSED(values);
    editElement<WdbFuelFlash>();
    return true;
}
