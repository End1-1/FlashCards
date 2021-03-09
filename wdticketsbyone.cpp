#include "wdticketsbyone.h"
#include "filterticketsbyone.h"

WdTicketsByOne::WdTicketsByOne(const QIcon &icon, const QString &label, QWidget *parent ) :
    C5Grid(icon, label, parent)
{
    fColorColumn = "fcolor";
    fHiddenColumns.append("fcolor");
    fColumnsSum.append(tr("Qty").toLower());
    fColumnsSum.append(tr("Sold amount").toLower());
    fColumnsSum.append(tr("Back amount").toLower());
    fColumnsSum.append(tr("Difference").toLower());
    fFilterWidget = new FilterTicketsByOne();
    fFilterWidget->restoreFilter(fFilterWidget);
    addAction(fReportActions, ":/res/filter.png", tr("Filter"), this, SLOT(setSearchParameters()));
    addAction(fReportActions, ":/res/back.png", tr("Reset filter"), this, SLOT(resetSearchParameters()));
}

void WdTicketsByOne::setFilterMode(int mode)
{
    static_cast<FilterTicketsByOne*>(fFilterWidget)->setDateMode(mode);
    buildReport("all_tickets3");
}
