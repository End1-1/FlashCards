#include "wdtflashfuelstore.h"
#include "filterflashfuelstore.h"

WdtFlashFuelStore::WdtFlashFuelStore(const QIcon &icon, const QString &label, QWidget *parent) :
    C5Grid(icon, label, parent)
{
    fColorColumn = "fcolor";
    fHiddenColumns.append("fcolor");
    fColumnsSum.append(tr("Qty"));
    fColumnsSum.append(tr("Amount"));
    fFilterWidget = new FilterFlashFuelStore();
    fFilterWidget->restoreFilter(fFilterWidget);
    addAction(fReportActions, ":/res/filter.png", tr("Filter"), this, SLOT(setSearchParameters()));
    addAction(fReportActions, ":/res/back.png", tr("Reset filter"), this, SLOT(resetSearchParameters()));
    buildReport("fuel_flash_store");
}
