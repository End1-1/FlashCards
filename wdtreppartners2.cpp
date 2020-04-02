#include "wdtreppartners2.h"

WdtRepPartners2::WdtRepPartners2(const QIcon &icon, const QString &label, QWidget *parent) :
    C5Grid(icon, label, parent)
{
    fColumnsSum.append(tr("Qty, partner"));
    fColumnsSum.append(tr("Litr, partner"));
    fColumnsSum.append(tr("Qty, used"));
    fColumnsSum.append(tr("Litr, used"));
    buildReport("rep_partner2");
    addAction(fReportActions, ":/res/filter.png", tr("Filter"), this, SLOT(setSearchParameters()));
}
