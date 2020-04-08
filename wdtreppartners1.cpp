#include "wdtreppartners1.h"
#include "filterpartner1.h"

WdtRepPartners1::WdtRepPartners1(const QIcon &icon, const QString &label, QWidget *parent) :
    C5Grid(icon, label, parent)
{
    fColorColumn = 9;
    fColumnsSum.append(tr("Qty, partner"));
    fColumnsSum.append(tr("Litr, partner"));
    fColumnsSum.append(tr("Qty, used"));
    fColumnsSum.append(tr("Litr, used"));
    fColumnsSum.append(tr("Qty, total"));
    fColumnsSum.append(tr("Litr, total"));
    fFilterWidget = new FilterPartner1();
    fFilterWidget->restoreFilter(fFilterWidget);
    buildReport("rep_partner1");
    addAction(fReportActions, ":/res/filter.png", tr("Filter"), this, SLOT(setSearchParameters()));
    addAction(fReportActions, ":/res/back.png", tr("Reset filter"), this, SLOT(resetSearchParameters()));
    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
    QMenu *mTickets = new QMenu(tr("Tickets"));
    db.exec("select * from cards_types order by ffuel, fname");
    while (db.nextRow()) {
        QAction *a = mTickets->addAction(QIcon(":/res/fuel.png"), db.getString("fname"), this, SLOT(filterFuel()));
        a->setProperty("id", db.getString("fid"));
    }
    fOtherMenu.append(mTickets);
}

void WdtRepPartners1::filterFuel()
{
    QAction *a = static_cast<QAction*>(sender());
    QString id = a->property("id").toString();
    static_cast<FilterPartner1*>(fFilterWidget)->setTicketFilter(id);
    fFilterWidget->saveFilter(fFilterWidget);
    buildQuery();
}
