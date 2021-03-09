#include "wdtamount1.h"
#include "filteramount1.h"

WdtAmount1::WdtAmount1(const QIcon &icon, const QString &label, QWidget *parent) :
    C5Grid(icon, label, parent)
{
    fColorColumn = "fcolor";
    fHiddenColumns.append("fcolor");
    fColumnsSum.append(tr("Qty\ntotal"));
    fColumnsSum.append(tr("Amount\ntotal"));
    fColumnsSum.append(tr("Qty\nremain"));
    fColumnsSum.append(tr("Amount\nremain"));
    fFilterWidget = new FilterAmount1();
    fFilterWidget->restoreFilter(fFilterWidget);
    buildReport("rep_amount1");
    addAction(fReportActions, ":/res/filter.png", tr("Filter"), this, SLOT(setSearchParameters()));
    addAction(fReportActions, ":/res/back.png", tr("Reset filter"), this, SLOT(resetSearchParameters()));

    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
    QMenu *mFuel = new QMenu(tr("Fuel"));
    db.exec("select * from fuel order by fid");
    while (db.nextRow()) {
        QAction *a = mFuel->addAction(QIcon(":/res/fuel.png"), db.getString("fname"), this, SLOT(filterFuel()));
        a->setProperty("id", db.getString("fid"));
    }
    fOtherMenu.append(mFuel);
}

void WdtAmount1::filterFuel()
{
    QAction *a = static_cast<QAction*>(sender());
    QString id = a->property("id").toString();
    static_cast<FilterAmount1*>(fFilterWidget)->setFuel(id);
    fFilterWidget->saveFilter(fFilterWidget);
    buildQuery();
}
