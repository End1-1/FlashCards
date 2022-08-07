#include "wdtfuelprices.h"
#include "dlgfuelprices.h"
#include "c5tablemodel.h"

WdtFuelPrices::WdtFuelPrices(const QIcon &icon, const QString &label, QWidget *parent) :
    C5Grid(icon, label, parent)
{
    buildReport("fuelprices");
    addAction(fEditActions, ":/res/new.png", tr("Enter new prices"), this, SLOT(newPrices()));
    addAction(fEditActions, ":/res/edit.png", tr("Edit prices"), this, SLOT(editPrices()));
}

void WdtFuelPrices::newPrices()
{
    DlgFuelPrices(QDate::currentDate(), this).exec();
}

void WdtFuelPrices::editPrices()
{
    QModelIndexList ml = fTableView->selectionModel()->selectedIndexes();
    if (ml.count() == 0) {
        C5Message::info(tr("Nothing was selected"));
        return;
    }
    int row = ml.at(0).row();
    QDate d = fModel->data(row, 0, Qt::EditRole).toDate();
    DlgFuelPrices(d, this).exec();
}
