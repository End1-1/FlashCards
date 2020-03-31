#include "wdtalltickets.h"
#include "filteralltickets.h"

WdtAllTickets::WdtAllTickets(const QIcon &icon, const QString &label, QWidget *parent) :
    C5Grid(icon, label, parent)
{
    fColumnsSum.append(tr("Qty"));
    fColumnsSum.append(tr("Litr"));
    buildReport("all_tickets");
    addAction(fReportActions, ":/res/filter.png", tr("Filter"), this, SLOT(setSearchParameters()));
    fFilterWidget = new FilterAllTickets();
}
