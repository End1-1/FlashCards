#include "autoinputdoc.h"
#include "ui_autoinputdoc.h"
#include "c5database.h"
#include "config.h"
#include "c5message.h"
#include "moveconsts.h"

AutoinputDoc::AutoinputDoc() :
    Dialog(),
    ui(new Ui::AutoinputDoc)
{
    ui->setupUi(this);
    ui->wc->enumLineEdit();
    ui->tbl->setColumnWidths(ui->tbl->columnCount(), 0, 0, 300, 80, 120, 120);
}

AutoinputDoc::~AutoinputDoc()
{
    delete ui;
}

void AutoinputDoc::setDoc(int id)
{
    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
    db[":fid"] = id;
    db.exec("select * from autoinput_flash where fid=:fid");
    if (db.nextRow()) {
        ui->leCode->setInteger(db.getInt("fid"));
        ui->leDate->setDate(db.getDate("fdate"));
        ui->leTime->setText(db.getTime("ftime").toString("HH:mm:ss"));
        ui->leDateCreated->setDate(db.getDate("fcreated_date"));
        ui->leTimeCreated->setText(db.getTime("fcreated_time").toString("HH:mm:ss"));
        ui->wc->setKey(ui->leStation, db.getInt("fstation"));
        ui->wc->setKey(ui->leState, db.getInt("fstate"));
        ui->btnWrite->setEnabled(db.getInt("fstate") == 1);
        ui->btnWriteDraft->setEnabled(db.getInt("fstate") == 2);
        db[":fau"] = id;
        db.exec("select  m.fid, m.ffuel, f.fname, m.fqty, m.fqty2, m.fprice, m.ftotal "
                "from fuel_move m "
                "left join fuel_flash f on f.fid=m.ffuel "
                "where m.fau=:fau ");
        while (db.nextRow()) {
            int r = ui->tbl->addEmptyRow();
            ui->tbl->setInteger(r, 0, db.getInt("fid"));
            ui->tbl->setInteger(r, 1, db.getInt("ffuel"));
            ui->tbl->setString(r, 2, db.getString("fname"));
            C5LineEdit *l = ui->tbl->createLineEdit(r, 3);
            l->setValidator(new QDoubleValidator(1, 10000, 0));
            l->setDouble(db.getDouble("fqty"));
            l = ui->tbl->createLineEdit(r, 4);
            l->setValidator(new QDoubleValidator(1, 1000000, 2));
            l->setDouble(db.getDouble("fqty2"));
//            l = ui->tbl->createLineEdit(r, 5);
//            l->setValidator(new QDoubleValidator(1, 1000000, 2));
//            l->setDouble(db.getDouble("ftotal"));
        }
    }
}

AutoinputDoc &AutoinputDoc::newDoc()
{
    ui->leDate->setReadOnly(false);
    ui->leStation->setReadOnly(false);
    ui->leState->setInteger(1);
    ui->leTime->setText(QTime::currentTime().toString(FORMAT_TIME_TO_STR));
    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);

    db.exec("select  0 as fid, f.fid as ffuel, f.fname, 0, 0, 0, 0 "
            "from fuel f ");
    while (db.nextRow()) {
        int r = ui->tbl->addEmptyRow();
        ui->tbl->setInteger(r, 0, db.getInt("fid"));
        ui->tbl->setInteger(r, 1, db.getInt("ffuel"));
        ui->tbl->setString(r, 2, db.getString("fname"));
        C5LineEdit *l = ui->tbl->createLineEdit(r, 3);
        l->setValidator(new QDoubleValidator(1, 10000, 0));
        l->setDouble(db.getDouble("fqty"));
        l = ui->tbl->createLineEdit(r, 4);
        l->setValidator(new QDoubleValidator(1, 1000000, 2));
        l->setDouble(db.getDouble("fqty2"));
//            l = ui->tbl->createLineEdit(r, 5);
//            l->setValidator(new QDoubleValidator(1, 1000000, 2));
//            l->setDouble(db.getDouble("ftotal"));
    }
    return *this;
}

void AutoinputDoc::on_btnRemove_clicked()
{
    if (C5Message::question(tr("Are you sure to remove this document?")) == QDialog::Accepted) {
        C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
        db[":fau"] = ui->leCode->getInteger();
        db.exec("delete from fuel_move where fau=:fau");
        db[":fid"] = ui->leCode->getInteger();
        db.exec("delete from autoinput_flash where fid=:fid");
        C5Message::info(tr("Deleted"));
        accept();
    }
}

void AutoinputDoc::on_btnWrite_clicked()
{
    write(2, 1);
}

void AutoinputDoc::on_btnWriteDraft_clicked()
{
    write(1, 3);
}

void AutoinputDoc::write(int docstate, int movestate)
{
    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);

    if (ui->leCode->getInteger() == 0) {
        db[":fstation"] = ui->leStation->getInteger();
        db[":fdate"] = ui->leDate->date();
        db[":ftime"] = QTime::currentTime();
        db[":fcreated_date"] = QDate::currentDate();
        db[":fcreated_time"] = QTime::currentTime();
        db[":fstate"] = 1;
        db[":fflag"] = 1;
        db[":ffile"] = db.uuid();
        ui->leCode->setInteger(db.insert("autoinput_flash"));
        ui->leStation->setReadOnly(true);
        ui->leDate->setReadOnly(true);

        for (int i = 0; i < ui->tbl->rowCount(); i++) {
            db[":fau"] = ui->leCode->getInteger();
            db[":fstate"] = MOVE_INPUT_DRAFT;
            db[":ffuel"] = ui->tbl->getInteger(i, 1);
            db[":fdate"] = ui->leDate->date();
            db[":fpartner"] = 0;
            db[":fpaymenttype"] = 1;
            db[":fsign"] = 1;
            db[":fqty"] = 0;
            db[":fqty2"] = 0;
            db[":fprice"] = 0;
            db[":ftotal"] = 0;
            ui->tbl->setInteger(i, 0, db.insert("fuel_move"));
        }
    }


    db[":fid"] = ui->leCode->getInteger();
    db[":fstate"] = docstate;
    db.exec("update autoinput_flash set fstate=:fstate where fid=:fid");
    db[":fau"] = ui->leCode->getInteger();
    db[":fstate"] = movestate;
    db.exec("update fuel_move set fstate=:fstate where fau=:fau");
    for (int i = 0; i < ui->tbl->rowCount(); i++) {
        db[":fid"] = ui->tbl->getInteger(i, 0);
        if (ui->tbl->lineEdit(i, 3)->getDouble() < 0.001 && ui->tbl->lineEdit(i, 4)->getDouble() < 0.001) {
            db.exec("delete from fuel_move where fid=:fid");
        } else {
            db[":fqty"] = ui->tbl->lineEdit(i, 3)->getDouble();
            db[":fqty2"] = ui->tbl->lineEdit(i, 4)->getDouble();
            db.exec("update fuel_move set fqty=:fqty, fqty2=:fqty2 where fid=:fid");
        }

        db[":fid"] = ui->tbl->getInteger(i, 1);
        db[":fqty"] = docstate == 2 ? ui->tbl->lineEdit(i, 3)->getDouble() : ui->tbl->lineEdit(i, 3)->getDouble() * -1;
        db.exec("update fuel_flash set fqty=fqty+:fqty where fid=:fid");
        db[":fid"] = ui->tbl->getInteger(i, 1);
        db[":fqty"] = docstate == 2 ? ui->tbl->lineEdit(i, 4)->getDouble() : ui->tbl->lineEdit(i, 4)->getDouble() * -1;
        db.exec("update fuel set fqty=fqty+:fqty where fid=:fid");

    }
    C5Message::info(tr("Saved"));
    accept();
}


