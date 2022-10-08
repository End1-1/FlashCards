#include "wdtamount3.h"
#include "wdtamount3filter.h"

WdtAmount3::WdtAmount3(const QIcon &icon, const QString &label, QWidget *parent) :
    C5Grid(icon, label, parent)
{
    fColorColumn = "fcolor";
    fHiddenColumns.append("fcolor");
    fColumnsSum.append(tr("Qty before"));
    fColumnsSum.append(tr("Amount before"));
    fColumnsSum.append(tr("Qty input"));
    fColumnsSum.append(tr("Amount input"));
    fColumnsSum.append(tr("Qty outpout"));
    fColumnsSum.append(tr("Amount output"));
    fColumnsSum.append(tr("Qty remain"));
    fColumnsSum.append(tr("Amount remain"));
    fFilterWidget = new WdtAmount3Filter();
    fFilterWidget->restoreFilter(fFilterWidget);
    buildReport("rep_range");
    addAction(fReportActions, ":/res/filter.png", tr("Filter"), this, SLOT(setSearchParameters()));
    addAction(fReportActions, ":/res/back.png", tr("Reset filter"), this, SLOT(resetSearchParameters()));

//    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
//    QMenu *mFuel = new QMenu(tr("Fuel"));
//    db.exec("select * from fuel order by fid");
//    while (db.nextRow()) {
//        QAction *a = mFuel->addAction(QIcon(":/res/fuel.png"), db.getString("fname"), this, SLOT(filterFuel()));
//        a->setProperty("id", db.getString("fid"));
//    }
//    fOtherMenu.append(mFuel);
}
