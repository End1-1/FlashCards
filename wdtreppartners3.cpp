#include "wdtreppartners3.h"
#include "filterpartner3.h"
#include "c5tablemodel.h"

WdtRepPartners3::WdtRepPartners3(const QIcon &icon, const QString &label, QWidget *parent) :
    C5Grid(icon, label, parent)
{
    fColorColumn = "fcolor";
    fHiddenColumns.append("fcolor");
    fHiddenColumns.append("fpartnertrans");
    fColumnsSum.append(tr("Qty, partner"));
    fColumnsSum.append(tr("Litr, partner"));
    fColumnsSum.append(tr("Qty, used"));
    fColumnsSum.append(tr("Litr, used"));
    fColumnsSum.append(tr("Qty, total"));
    fColumnsSum.append(tr("Litr, total"));
    fFilterWidget = new FilterPartner3();
    fFilterWidget->restoreFilter(fFilterWidget);
    buildReport("rep_partner3");
    addAction(fEditActions, ":/back.png", tr("Rollback"), this, SLOT(rollback()));
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

void WdtRepPartners3::filterFuel()
{
    QAction *a = static_cast<QAction*>(sender());
    QString id = a->property("id").toString();
    static_cast<FilterPartner3*>(fFilterWidget)->setFuelFilter(id);
    fFilterWidget->saveFilter(fFilterWidget);
    buildQuery();
}

void WdtRepPartners3::filterTicket()
{
    QAction *a = static_cast<QAction*>(sender());
    QString id = a->property("id").toString();
    static_cast<FilterPartner3*>(fFilterWidget)->setTicketFilter(id);
    fFilterWidget->saveFilter(fFilterWidget);
    buildQuery();
}

void WdtRepPartners3::rollback()
{
    int r;
    if (currentRow(r)) {
        QString t = fModel->data(r, fModel->indexForColumnName("fpartnertrans"), Qt::EditRole).toString();
        if (t.isEmpty()) {
            C5Message::error(tr("Transaction is empty"));
            return;
        }
        if (C5Message::question(tr("Are you sure to rollback this transaction?")) != QDialog::Accepted) {
            return;
        }
        C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
        db[":fpartnertrans"] = t;
        db.exec("select count(fid) from cards where fpartnertrans=:fpartnertrans");
        db.nextRow();
        int total = db.getInt(0);
        db[":fpartnertrans"] = t;
        db.exec("update cards set fstate=1, fpartnertrans=null, fpartnerdate=null, fprice=null where fpartnertrans=:fpartnertrans and fstate=2");
        int updated = db.fAffectedRows;
        C5Message::info(QString("%1 - %2 <br>%3 - %4").arg(tr("Total")).arg(total).arg(tr("Rollback")).arg(updated));
    }
}
