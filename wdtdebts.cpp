#include "wdtdebts.h"
#include "filterdebts.h"
#include "dlgdebtpayment.h"
#include "wdtdebtshistory.h"

WdtDebts::WdtDebts(const QIcon &icon, const QString &label, QWidget *parent) :
    C5Grid(icon, label, parent)
{
    fColumnsSum.append(tr("Begin balance"));
    fColumnsSum.append(tr("Debet"));
    fColumnsSum.append(tr("Credit"));
    fColumnsSum.append(tr("Final balance"));
    fFilterWidget = new FilterDebts();
    fFilterWidget->restoreFilter(fFilterWidget);
    fFilterDebts = static_cast<FilterDebts*>(fFilterWidget);
    if (__userrole != 2) {
        addAction(fEditActions, ":/res/edit.png", tr("Debt repayment"), this, SLOT(editDoc()));
    }
    addAction(fReportActions, ":/res/filter.png", tr("Filter"), this, SLOT(setSearchParameters()));
    buildReport("debts");
}

void WdtDebts::buildReport(const QString &name)
{
    fDb[":fid"] = name;
    fDb.exec("select fquery from sql_query where fid=:fid");
    if (fDb.nextRow()) {
        fSimpleQuery = true;
        fSqlQuery = fDb.getString("fquery");
    }
    fSqlQuery.replace("%w1%", fFilterDebts->c1()).replace("%w2%", fFilterDebts->c2());
    buildQuery();
}

void WdtDebts::editDoc()
{
    DlgDebtPayment(this).exec();
}

void WdtDebts::setSearchParameters()
{
    C5Grid::setSearchParameters();
    fDb[":fid"] = "debts";
    fDb.exec("select fquery from sql_query where fid=:fid");
    if (fDb.nextRow()) {
        fSimpleQuery = true;
        fSqlQuery = fDb.getString("fquery");
    }
    fSqlQuery.replace("%w1%", fFilterDebts->c1()).replace("%w2%", fFilterDebts->c2());
    buildQuery();
}
