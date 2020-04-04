#include "wdtalltickets.h"
#include "filteralltickets.h"

WdtAllTickets::WdtAllTickets(const QIcon &icon, const QString &label, QWidget *parent) :
    C5Grid(icon, label, parent)
{
    fColumnsSum.append(tr("Qty"));
    fColumnsSum.append(tr("Litr"));
    buildReport("all_tickets");
    addAction(fReportActions, ":/res/filter.png", tr("Filter"), this, SLOT(setSearchParameters()));
    addAction(fReportActions, ":/res/back.png", tr("Reset filter"), this, SLOT(resetSearchParameters()));
    fFilterWidget = new FilterAllTickets();
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
    QMenu *mState = new QMenu(tr("State"));
    db.exec("SELECT * FROM cards_state");
    while (db.nextRow()) {
        QAction *a = mState->addAction(QIcon(":/res/fuel.png"), db.getString("fname"), this, SLOT(filterState()));
        a->setProperty("id", db.getString("fid"));
    }
    fOtherMenu.append(mFuel);
    fOtherMenu.append(mTickets);
    fOtherMenu.append(mState);
}

void WdtAllTickets::filterFuel()
{
    QAction *a = static_cast<QAction*>(sender());
    QString id = a->property("id").toString();
    static_cast<FilterAllTickets*>(fFilterWidget)->setFuelFilter(id);
    buildQuery();
}

void WdtAllTickets::filterTicket()
{
    QAction *a = static_cast<QAction*>(sender());
    QString id = a->property("id").toString();
    static_cast<FilterAllTickets*>(fFilterWidget)->setTicketFilter(id);
    buildQuery();
}

void WdtAllTickets::filterState()
{
    QAction *a = static_cast<QAction*>(sender());
    QString id = a->property("id").toString();
    static_cast<FilterAllTickets*>(fFilterWidget)->setStateFilter(id);
    buildQuery();
}
