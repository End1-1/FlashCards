#include "dlgfuelprices.h"
#include "ui_dlgfuelprices.h"
#include "C5Database.h"
#include "config.h"
#include "c5message.h"

DlgFuelPrices::DlgFuelPrices(const QDate &d, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgFuelPrices)
{
    ui->setupUi(this);
    ui->tbl->setColumnWidths(4, 0, 0, 300, 120);
    ui->leDate->setDate(d);
    loadPrices(d);
}

DlgFuelPrices::~DlgFuelPrices()
{
    delete ui;
}

void DlgFuelPrices::loadPrices(const QDate &d)
{
    ui->tbl->clearContents();
    ui->tbl->setRowCount(0);
    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
    db[":fdate"] = d;
    QString sql = "SELECT fp.fid, f.fid as ffuelid, f.fname, fp.fprice FROM fuel f LEFT JOIN fuel_prices fp ON fp.ffuel=f.fid AND fp.fdate=:fdate";
    db.exec(sql);
    while (db.nextRow()) {
        int r = ui->tbl->addEmptyRow();
        ui->tbl->setInteger(r, 0, db.getInt("fid"));
        ui->tbl->setInteger(r, 1, db.getInt("ffuelid"));
        ui->tbl->setString(r, 2, db.getString("fname"));
        auto *l = ui->tbl->createLineEdit(r, 3);
        l->setValidator(new QDoubleValidator(0, 999999, 0));
        l->setDouble(db.getDouble("fprice"));
    }
}

void DlgFuelPrices::on_btnDateForward_clicked()
{
    ui->leDate->setDate(ui->leDate->date().addDays(1));
    loadPrices(ui->leDate->date());
}

void DlgFuelPrices::on_btnDateBack_clicked()
{
    ui->leDate->setDate(ui->leDate->date().addDays(-1));
    loadPrices(ui->leDate->date());
}

void DlgFuelPrices::on_btnSave_clicked()
{
    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
    for (int i = 0; i < ui->tbl->rowCount(); i++) {
        db[":fdate"] = ui->leDate->date();
        db[":ffuel"] = ui->tbl->getInteger(i, 1);
        db[":fprice"] = ui->tbl->lineEdit(i, 3)->getDouble();
        if (ui->tbl->getInteger(i, 0) == 0) {
            ui->tbl->setInteger(i, 0, db.insert("fuel_prices", true));
        } else {
            db.update("fuel_prices", where_id(ui->tbl->getInteger(i, 0)));
        }
    }
    C5Message::info(tr("Saved"));
}

void DlgFuelPrices::on_btnCopyFromPreviouseDay_clicked()
{
    QDate d = ui->leDate->date().addDays(-1);
    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
    QString sql = "select ffuel, fprice from fuel_prices where fdate=:fdate";
    db[":fdate"] = d;
    db.exec(sql);
    while (db.nextRow()) {
        for (int i = 0; i < ui->tbl->rowCount(); i++) {
            if (ui->tbl->getInteger(i, 1) == db.getInt("ffuel")) {
                ui->tbl->lineEdit(i, 3)->setDouble(db.getDouble("fprice"));
            }
        }
    }
}
