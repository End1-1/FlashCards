#include "wdtfuelflashmove.h"
#include "fuelflashmove.h"
#include "filterfuelflash.h"

WdtFuelFlashMove::WdtFuelFlashMove(const QIcon &icon, const QString &label, QWidget *parent) :
    C5Grid(icon, label, parent)
{
    fColorColumn = "fcolor";
    fHiddenColumns.append("fcolor");
    fColumnsSum.append(tr("Qty"));
    fColumnsSum.append(tr("Amount"));
    fFilterWidget = new FilterFuelFlash();
    fFilterWidget->restoreFilter(fFilterWidget);
    addAction(fEditActions, ":/res/new.png", tr("New input"), this, SLOT(newDocInput()));
    addAction(fEditActions, ":/res/new.png", tr("New output"), this, SLOT(newDocOutput()));
    addAction(fEditActions, ":/res/edit.png", tr("Edit"), this, SLOT(editDoc()));
    addAction(fReportActions, ":/res/filter.png", tr("Filter"), this, SLOT(setSearchParameters()));
    addAction(fReportActions, ":/res/back.png", tr("Reset filter"), this, SLOT(resetSearchParameters()));
    buildReport("fuel_flash_report");
}

void WdtFuelFlashMove::newDocInput()
{
    FuelFlashMove ffm;
    ffm.setMove(1);
    ffm.exec();
}

void WdtFuelFlashMove::newDocOutput()
{
    FuelFlashMove ffm;
    ffm.setMove(-1);
    ffm.exec();
}

void WdtFuelFlashMove::editDoc()
{
    int id = rowId();
    if (id == 0) {
        return;
    }
    FuelFlashMove ffm;
    ffm.setDoc(id);
    ffm.exec();
}

bool WdtFuelFlashMove::tblDoubleClicked(int row, int column, const QList<QVariant> &values)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
    if (values.count() == 0) {
        return false;
    }
    FuelFlashMove ffm;
    ffm.setDoc(values.at(0).toInt());
    ffm.exec();
    return true;
}
