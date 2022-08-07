#include "wdtflashautoinput.h"
#include "c5filterautoinputflash.h"
#include "autoinputdoc.h"

WdtFlashAutoinput::WdtFlashAutoinput(const QIcon &icon, const QString &label, QWidget *parent) :
    C5Grid(icon, label, parent)
{
    fColorColumn = "fcolor";
    fHiddenColumns.append("fcolor");
    fColumnsSum.append(tr("Qty"));
    fColumnsSum.append(tr("Amount"));
    fFilterWidget = new C5FilterAutoinputFlash();
    fFilterWidget->restoreFilter(fFilterWidget);
    addAction(fReportActions, ":/res/filter.png", tr("Filter"), this, SLOT(setSearchParameters()));
    addAction(fReportActions, ":/res/back.png", tr("Reset filter"), this, SLOT(resetSearchParameters()));
    addAction(fEditActions, ":/res/edit.png", tr("Edit"), this, SLOT(editDoc()));
    buildReport("rep_flash_autoinput");
}

void WdtFlashAutoinput::editDoc()
{
    int id = rowId();
    if (id == 0) {
        return;
    }
    AutoinputDoc ffm;
    ffm.setDoc(id);
    ffm.exec();
}

bool WdtFlashAutoinput::tblDoubleClicked(int row, int column, const QList<QVariant> &values)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
    if (values.count() == 0) {
        return false;
    }
    AutoinputDoc ffm;
    ffm.setDoc(values.at(0).toInt());
    ffm.exec();
    return true;
}
