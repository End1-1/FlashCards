#include "wdtamount2.h"
#include "filteramount2.h"

WdtAmount2::WdtAmount2(const QIcon &icon, const QString &label, QWidget *parent) :
    C5Grid(icon, label, parent)
{
    fColorColumn = 6;
    fColumnsSum.append(tr("Qty"));
    fColumnsSum.append(tr("Amount"));
    fFilterWidget = new FilterAmount2();
    fFilterWidget->restoreFilter(fFilterWidget);
    buildReport("rep_amount2");
    addAction(fReportActions, ":/res/filter.png", tr("Filter"), this, SLOT(setSearchParameters()));
    addAction(fReportActions, ":/res/back.png", tr("Reset filter"), this, SLOT(resetSearchParameters()));

    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
    QMenu *mFuel = new QMenu(tr("Fuel"));
    db.exec("select * from fuel order by fid");
    while (db.nextRow()) {
        QAction *a = mFuel->addAction(QIcon(":/res/fuel.png"), db.getString("fname"), this, SLOT(filterFuel()));
        a->setProperty("id", db.getString("fid"));
    }
    QMenu *mTickets = new QMenu(tr("Tickets"));
    db.exec("select * from cards_types order by ffuel, fname");
    while (db.nextRow()) {
        QAction *a = mTickets->addAction(QIcon(":/res/fuel.png"), db.getString("fname"), this, SLOT(filterTicket()));
        a->setProperty("id", db.getString("fid"));
    }
    fOtherMenu.append(mFuel);
    fOtherMenu.append(mTickets);
}

void WdtAmount2::filterFuel()
{
    QAction *a = static_cast<QAction*>(sender());
    QString id = a->property("id").toString();
    static_cast<FilterAmount2*>(fFilterWidget)->setFuel(id);
    fFilterWidget->saveFilter(fFilterWidget);
    buildQuery();
}

void WdtAmount2::filterTicket()
{
    QAction *a = static_cast<QAction*>(sender());
    QString id = a->property("id").toString();
    static_cast<FilterAmount2*>(fFilterWidget)->setTicket(id);
    fFilterWidget->saveFilter(fFilterWidget);
    buildQuery();
}
