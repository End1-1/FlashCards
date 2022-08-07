#include "wdtsaledocs.h"
#include "filterdocs.h"
#include "doctype.h"
#include "passticketsdialog.h"
#include "fuelflashmove.h"

WdtSaleDocs::WdtSaleDocs(const QIcon &icon, const QString &label, QWidget *parent) :
    C5Grid(icon, label, parent)
{
    fColumnsSum.append(tr("Amount"));
    fFilterWidget = new FilterDocs();
    fFilterWidget->restoreFilter(fFilterWidget);
    addAction(fEditActions, ":/res/edit.png", tr("Edit"), this, SLOT(editDoc()));
    addAction(fReportActions, ":/res/filter.png", tr("Filter"), this, SLOT(setSearchParameters()));
    buildReport("docsbypayment");
}

void WdtSaleDocs::editDoc()
{
    int id;
    if ((id = rowId()) == 0) {
        return;
    }
    callEditor(QString::number(id));
}

void WdtSaleDocs::callEditor(const QString &id)
{
    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
    db[":fid"] = id.toInt();
    db.exec("select * from docs where fid=:fid");
    if (!db.nextRow()) {
        C5Message::error(tr("Invalid document number"));
        return;
    }
    switch (db.getInt("fdoctype")) {
    case DOCTYPE_PASSTICKET:
        PassTicketsDialog(id.toInt()).exec();
        break;
    case DOCTYPE_FLASHSALE:
        FuelFlashMove().setDoc(id.toInt()).exec();
        break;
    }
}
