#include "wdtreppartners2.h"
#include "filterpartner2.h"

WdtRepPartners2::WdtRepPartners2(const QIcon &icon, const QString &label, QWidget *parent) :
    C5Grid(icon, label, parent)
{
    fColorColumn = 8;
    fColumnsSum.append(tr("Qty, partner"));
    fColumnsSum.append(tr("Litr, partner"));
    fColumnsSum.append(tr("Qty, used"));
    fColumnsSum.append(tr("Litr, used"));
    fColumnsSum.append(tr("Qty, total"));
    fColumnsSum.append(tr("Litr, total"));
    fFilterWidget = new FilterPartner2();
    fFilterWidget->restoreFilter(fFilterWidget);
    buildReport("rep_partner2");
    addAction(fReportActions, ":/res/filter.png", tr("Filter"), this, SLOT(setSearchParameters()));
    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
    QMenu *mFuel = new QMenu(tr("Fuel"));
    db.exec("select * from fuel order by fid");
    while (db.nextRow()) {
        QAction *a = mFuel->addAction(QIcon(":/res/fuel.png"), db.getString("fname"), this, SLOT(filterFuel()));
        a->setProperty("id", db.getString("fid"));
    }
    fOtherMenu.append(mFuel);
}

void WdtRepPartners2::filterFuel()
{
    QAction *a = static_cast<QAction*>(sender());
    QString id = a->property("id").toString();
    static_cast<FilterPartner2*>(fFilterWidget)->setFuelFilter(id);
    fFilterWidget->saveFilter(fFilterWidget);
    buildQuery();
}
