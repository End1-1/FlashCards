#include "wdtdebtshistory.h"
#include "filterdebtshistory.h"

WdtDebtsHistory::WdtDebtsHistory(const QIcon &icon, const QString &label, QWidget *parent) :
    C5Grid(icon, label, parent)
{
    fColumnsSum.append(tr("Debet"));
    fColumnsSum.append(tr("Credit"));
    fColumnsSum.append(tr("Difference"));
    fFilterWidget = new FilterDebtsHistory();
    fFilterWidget->restoreFilter(fFilterWidget);
    fFilterHistory = static_cast<FilterDebtsHistory*>(fFilterWidget);
    addAction(fReportActions, ":/res/filter.png", tr("Filter"), this, SLOT(setSearchParameters()));
    buildReport("debtshistory");
}

void WdtDebtsHistory::setFilter(const QDate &d1, const QDate &d2, int partner)
{

}
