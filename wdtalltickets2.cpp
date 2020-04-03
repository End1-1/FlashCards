#include "wdtalltickets2.h"
#include "filteralltickets2.h"

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
    addAction(fReportActions, ":/res/back.png", tr("Reset filter"), this, SLOT(resetSearchParameters()));
    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
    db.exec("select * from fuel order by fid");
    while (db.nextRow()) {
        QAction *a = addAction(fReportActions, ":/res/fuel.png", db.getString("fname"), this, SLOT(filterFuel()));
        a->setProperty("id", db.getString("fid"));
    }
    fFilterWidget = new FilterAllTickets2();
}

void WdtAllTickets2::filterFuel()
{
    QAction *a = static_cast<QAction*>(sender());
    QString id = a->property("id").toString();
    static_cast<FilterAllTickets2*>(fFilterWidget)->setFuelFilter(id);
    buildQuery();
}
