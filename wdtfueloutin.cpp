#include "wdtfueloutin.h"
#include "filterfuelinout.h"

WdtFuelOutIn::WdtFuelOutIn(const QIcon &icon, const QString &label, QWidget *parent) :
    C5Grid(icon, label, parent)
{
    fColumnsSum.append(tr("Input"));
    fColumnsSum.append(tr("Output"));
    fFilterWidget = new FilterFuelInOut();
    fFilterWidget->restoreFilter(fFilterWidget);
    addAction(fReportActions, ":/res/filter.png", tr("Filter"), this, SLOT(setSearchParameters()));
    buildReport("fueloutinput");
}
