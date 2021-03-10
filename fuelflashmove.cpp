#include "fuelflashmove.h"
#include "ui_fuelflashmove.h"
#include "C5Database.h"
#include "c5message.h"
#include "config.h"

FuelFlashMove::FuelFlashMove() :
    Dialog(),
    ui(new Ui::FuelFlashMove)
{
    ui->setupUi(this);
    ui->wc->enumLineEdit();
}

FuelFlashMove::~FuelFlashMove()
{
    delete ui;
}

void FuelFlashMove::setMove(int m)
{
    fMove = m;
    switch (fMove) {
    case 1:
        ui->leMove->setText(tr("Input"));
        break;
    case -1:
        ui->leMove->setText(tr("Output"));
        break;
    }
}

void FuelFlashMove::setDoc(int id)
{
    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
    db[":fid"] = id;
    db.exec("select * from fuel_move where fid=:fid");
    if (db.nextRow()) {
        ui->leCode->setInteger(id);
        ui->leDate->setDate(db.getDate("fdate"));
        ui->wc->setKey(ui->leFuel, db.getInt("ffuel"));
        ui->wc->setKey(ui->lePartner, db.getInt("fpartner"));
        ui->leQty->setDouble(db.getDouble("fqty"));
        ui->lePrice->setDouble(db.getDouble("fprice"));
        ui->leTotal->setDouble(db.getDouble("ftotal"));
        ui->cbPayment->setCurrentIndex(db.getInt("fpaymenttype"));
        setMove(db.getInt("fstate"));
    }
}

void FuelFlashMove::on_btnCancel_clicked()
{
    reject();
}

void FuelFlashMove::on_btnSave_clicked()
{
    QString err;
    if (ui->leQty->getDouble() < 0.001) {
        err +=  "<br>" + tr("Quantity is not defined");
    }
    if (ui->leFuel->getInteger() == 0) {
        err += "<br>" + tr("Fuel type is not defined");
    }
    if (!err.isEmpty()) {
        C5Message::error(err);
        return;
    }
    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
    db[":fstate"] = fMove;
    db[":ffuel"] = ui->leFuel->getInteger();
    db[":fdate"] = ui->leDate->date();
    db[":fpartner"] = ui->lePartner->getInteger();
    db[":fpaymenttype"] = ui->cbPayment->currentIndex();
    db[":fqty"] = ui->leQty->getDouble();
    db[":fprice"] = ui->lePrice->getDouble();
    db[":ftotal"] = ui->leTotal->getDouble();
    if (ui->leCode->getInteger() == 0) {
        ui->leCode->setInteger(db.insert("fuel_move"));
    } else {
        db.update("fuel_move", where_id(ui->leCode->text()));
    }
    C5Message::info(tr("Saved"));
}

void FuelFlashMove::on_leQty_textEdited(const QString &arg1)
{
    ui->leTotal->setDouble(arg1.toDouble() * ui->lePrice->getDouble());
}

void FuelFlashMove::on_lePrice_textEdited(const QString &arg1)
{
    ui->leTotal->setDouble(arg1.toDouble() * ui->leQty->getDouble());
}

void FuelFlashMove::on_leTotal_textEdited(const QString &arg1)
{
    if (ui->leQty->getDouble() > 0.001) {
        ui->lePrice->setDouble(arg1.toDouble() / ui->leQty->getDouble());
    }
}
