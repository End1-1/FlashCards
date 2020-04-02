#include "wdtalltickets2.h"

WdtAllTickets2::WdtAllTickets2(const QIcon &icon, const QString &label, QWidget *parent) :
    C5Grid(icon, label, parent)
{
    fColumnsSum.append(tr("Qty, issue"));
    fColumnsSum.append(tr("Litr, issue"));
    fColumnsSum.append(tr("Qty, partner"));
    fColumnsSum.append(tr("Litr, partner"));
    fColumnsSum.append(tr("Qty, used"));
    fColumnsSum.append(tr("Litr, used"));
    buildReport("all_tickets2");
    addAction(fReportActions, ":/res/filter.png", tr("Filter"), this, SLOT(setSearchParameters()));

}
