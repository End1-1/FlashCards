#include "fuelflashmove.h"
#include "ui_fuelflashmove.h"
#include "C5Database.h"
#include "c5message.h"
#include "doctype.h"
#include "config.h"
#include "c5printing.h"
#include "c5printpreview.h"

FuelFlashMove::FuelFlashMove() :
    Dialog(),
    ui(new Ui::FuelFlashMove)
{
    ui->setupUi(this);
    ui->wc->enumLineEdit();
    ui->leDiscount->setValidator(new QDoubleValidator(0, 999999,2));
    ui->lePaid->setValidator(new QDoubleValidator(0, 999999,2));
    ui->leTotal->setValidator(new QDoubleValidator(0, 999999,2));
    ui->lePrice->setValidator(new QDoubleValidator(0, 99999999, 2));
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
    case 2:
        ui->leMove->setText(tr("Output"));
        break;
    }
}

FuelFlashMove &FuelFlashMove::setDoc(int id)
{
    fDocNumber = id;
    ui->gbPayment->setEnabled(false);
    ui->btnSave->setEnabled(false);
    ui->btnPrint->setEnabled(true);
    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
    db[":fdoc"] = id;
    db.exec("select * from fuel_move where fdoc=:fdoc");
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
    } else {
        C5Message::error(tr("Invalid document number"));
    }
    return *this;
}

void FuelFlashMove::on_btnCancel_clicked()
{
    reject();
}

void FuelFlashMove::on_btnSave_clicked()
{
    QString err;
    if (ui->tbl->rowCount() < 1) {
        err += tr("Empty document");
    }
    if (!err.isEmpty()) {
        C5Message::error(err);
        return;
    }
    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
    db.startTransaction();
    if (fTransaction.isEmpty()) {
        fTransaction = db.uuid();
        db[":fcompany"] = 1;
        db[":fdoctype"] = DOCTYPE_FLASHSALE;
        db[":fdatecreate"] = QDate::currentDate();
        db[":ftimecreate"] = QTime::currentTime();
        db[":ftransaction"] = fTransaction;
        db[":fpaymenttype"] = ui->cbPayment->currentIndex() + 1;
        db[":fcompany"] = 2;
        db[":finvoice"] = ui->leInvoiceNumber->text();
        fDocNumber = db.insert("docs", true);
        if (fDocNumber == 0) {
            db.rollback();
            C5Message::error(db.fLastError);
            return;
        }

        db[":fdate"] = QDate::currentDate();
        db[":fdoc"] = fDocNumber;
        db[":fpartner"] = ui->lePartner->getInteger();
        db[":fdebet"] = ui->lePaid->getDouble();
        db[":fcredit"] = ui->ltTotal->getDouble();
        db.insert("debts", false);

        db[":fid"] = fTransaction;
        db[":fuser"] = __userid;
        if (!db.insert("transactions")) {
            db.rollback();
            C5Message::error(db.fLastError);
            return;
        }
    }
    for (int i = 0; i < ui->tbl->rowCount(); i++) {
        db[":fstate"] = fMove;
        db[":fdoc"] = fDocNumber;
        db[":fsign"] = fMove == 1 ? 1 : -1;
        db[":ffuel"] = ui->tbl->getInteger(i, 0);
        db[":fdate"] = ui->leDate->date();
        db[":fpartner"] = ui->lePartner->getInteger();
        db[":fpaymenttype"] = ui->cbPayment->currentIndex();
        db[":fqty"] = ui->tbl->getDouble(i, 2);
        db[":fprice"] = ui->tbl->getDouble(i, 3);
        db[":fdiscount"] = ui->tbl->getDouble(i, 4);
        db[":ftotal"] = ui->tbl->getDouble(i, 5);
        db.insert("fuel_move");
        db[":fid"] = ui->tbl->getInteger(i, 0);
        db[":fqty"] = ui->tbl->getDouble(i, 2);
        db.exec("update fuel_flash set fqty=fqty-:fqty where fid=:fid");
    }
    db.commit();
    ui->btnSave->setEnabled(false);
    ui->btnPrint->setEnabled(true);
    C5Message::info(tr("Saved"));
}

void FuelFlashMove::on_leQty_textEdited(const QString &arg1)
{
    ui->leTotal->setDouble(arg1.toDouble() * ui->lePrice->getDouble());
}

void FuelFlashMove::on_lePrice_textEdited(const QString &arg1)
{
    ui->leInitialPrice->setDouble(arg1.toDouble() + ui->leDiscount->getDouble());
    ui->leTotal->setDouble(arg1.toDouble() * ui->leQty->getDouble());
}

void FuelFlashMove::on_leTotal_textEdited(const QString &arg1)
{
    if (ui->leQty->getDouble() > 0.001) {
        ui->lePrice->setDouble(arg1.toDouble() / ui->leQty->getDouble());
        ui->leInitialPrice->setDouble(arg1.toDouble() + ui->leDiscount->getDouble());
    }
}

void FuelFlashMove::on_btnPrint_clicked()
{
    if (fDocNumber == 0) {
        C5Message::error(tr("Document not saved."));
        return;
    }
    C5Printing p;
    p.setSceneParams(2800, 2000, QPrinter::Landscape);
    QFont font(font());
    int fontbase = 26;
    font.setPointSize(fontbase);
    //font.setBold(true);
    p.setFont(font);
    int baseleft = 100;
    int sl = 1400 + baseleft;
    int cl = (2700 / 2) / 2;
    int cr = (2700 / 2) + cl;
    p.setFontSize(fontbase + 4);
    p.setFontBold(true);
    p.ltext(tr("<<ARMPETROL>> LTD"), baseleft);
    p.ltext(tr("<<ARMPETROL>> LTD"), baseleft + sl);
    p.br();
    p.setFontSize(fontbase);
    p.setFontBold(false);
    p.ctextof(tr("DEAL"), cl);
    p.ctextof(tr("DEAL"), cr);
    p.br();
    p.ctextof(tr("ACCEPTANCE - DELIVERY"), cl);
    p.ctextof(tr("ACCEPTANCE - DELIVERY"), cr);
    p.br();
    QString strTemp = QString("%1 %2").arg(tr("Date"), ui->leDate->text());
    if (!ui->leInvoiceNumber->isEmpty()) {
        strTemp += QString(" %1 %2").arg(tr("Invoice number"), ui->leInvoiceNumber->text());
    }
    p.ctextof(strTemp, cl);
    p.ctextof(strTemp, cr);
    p.br();
    p.ltext(tr("Receiver:"), baseleft);
    p.ltext(tr("Receiver:"), baseleft + sl);
    p.line(baseleft + 300, p.fTop + p.fLineHeight, 1200, p.fTop + p.fLineHeight);
    p.line(baseleft + sl + 300, p.fTop + p.fLineHeight, sl + 1200, p.fTop + p.fLineHeight);
    p.ltext(ui->lePartnerName->text(), baseleft + 450);
    p.ltext(ui->lePartnerName->text(), baseleft + sl + 450);
    p.br();
    p.ltext(tr("Payment type:"), baseleft);
    p.ltext(tr("Payment type:"), baseleft + sl);
    p.line(baseleft + 300, p.fTop + p.fLineHeight, 1200, p.fTop + p.fLineHeight);
    p.line(baseleft + sl + 300, p.fTop + p.fLineHeight, baseleft + sl + 1200, p.fTop + p.fLineHeight);
    p.ltext(ui->cbPayment->currentText(), baseleft + 450);
    p.ltext(ui->cbPayment->currentText(), baseleft + sl + 450);
    p.br();
    p.ltext(tr("Paid:"), baseleft);
    p.ltext(tr("Paid:"), baseleft + sl);
    p.line(baseleft + 300, p.fTop + p.fLineHeight, 1200, p.fTop + p.fLineHeight);
    p.line(baseleft + sl + 300, p.fTop + p.fLineHeight, sl + 1200, p.fTop + p.fLineHeight);
    p.ltext(ui->lePaid->text(), 350);
    p.ltext(ui->lePaid->text(), sl + 350);
    p.br();
    p.br();

    QList<qreal> points;
    points << baseleft << 100 << 300 << 150 << 150 << 150 << 150 << 250;
    QList<qreal> points2 = points;
    points2[0] = points2[0] + sl;
    QStringList vals;
    vals << tr("NN") << tr("Name") << tr("Unit") << tr("Quantity") << tr("Price") <<  tr("Discount") << tr("Amount");
    p.tableText(points, vals, p.fLineHeight + 30);
    p.tableText(points2, vals, p.fLineHeight + 30);
    p.br(p.fLineHeight + 20);

    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
    db[":fdoc"] = fDocNumber;
    db.exec("SELECT f.fname, m.fdiscount as fpricediscount, m.fprice, m.fqty, m.ftotal "
            "FROM fuel_move m "
            "left join fuel_flash f on f.fid=m.ffuel "
            "WHERE m.fdoc=:fdoc ");
    int r = 1;
    double total = 0;
    while (db.nextRow()) {
        vals.clear();
        vals << QString::number(r++)
             << db.getString("fname")
             << tr("Litr")
             << float_str(db.getDouble("fqty"), 2)
             << float_str(db.getDouble("fprice"), 2)
             << float_str(db.getDouble("fpricediscount"), 2)
             << float_str(db.getDouble("ftotal"), 2);
        p.tableText(points, vals, p.fLineHeight + 30);
        p.tableText(points2, vals, p.fLineHeight + 30);
        p.br(p.fLineHeight + 20);

        total += db.getDouble("ftotal");
    }
    points.clear();
    points << baseleft + 700 << 300 << 250;
    points2 = points;
    points2[0] = points2[0] + sl;
    vals.clear();
    vals << tr("Total") << float_str(total, 2);
    p.tableText(points, vals, p.fLineHeight + 30);
    p.tableText(points2, vals, p.fLineHeight + 20);
    p.br(p.fLineHeight + 20);
    p.br();
    p.br();
    p.br(p.fLineHeight + 20);

    p.ltext(tr("Receiver:"), baseleft);
    p.ltext(tr("Receiver:"), baseleft + sl);
    p.ltext(tr("Deliver:"), baseleft + 750);
    p.ltext(tr("Deliver:"), baseleft + sl + 750);
    p.line(baseleft, p.fTop + p.fLineHeight, 500, p.fTop + p.fLineHeight);
    p.line(baseleft + sl, p.fTop + p.fLineHeight, sl + 500, p.fTop + p.fLineHeight);
    p.line(baseleft + 750, p.fTop + p.fLineHeight, 1200, p.fTop + p.fLineHeight);
    p.line(baseleft + sl + 750, p.fTop + p.fLineHeight, sl + 1200, p.fTop + p.fLineHeight);
    p.br();
    p.ltext(tr("(signature)"), baseleft + 200);
    p.ltext(tr("(signature)"), baseleft + sl + 200);
    p.ltext(tr("(signature)"), baseleft + 900);
    p.ltext(tr("(signature)"), baseleft + sl + 900);
    p.br();
    p.br();
    p.ltext(QString("%1 %2").arg(tr("Printed:"), QDateTime::currentDateTime().toString(FORMAT_DATETIME_TO_STR)), baseleft);
    p.ltext(QString("%1 %2").arg(tr("Printed:"), QDateTime::currentDateTime().toString(FORMAT_DATETIME_TO_STR)), sl + baseleft);

    C5PrintPreview pp(&p, this);
    pp.exec();
}

void FuelFlashMove::on_leInitialPrice_textChanged(const QString &arg1)
{
    ui->lePrice->setDouble(arg1.toDouble() - ui->leDiscount->getDouble());
    on_lePrice_textEdited(ui->lePrice->text());
}

void FuelFlashMove::on_leDiscount_textChanged(const QString &arg1)
{
    ui->lePrice->setDouble(ui->leInitialPrice->getDouble() - arg1.toDouble());
    on_lePrice_textEdited(ui->lePrice->text());
}

void FuelFlashMove::on_btnAddRow_clicked()
{
    int r = ui->tbl->addEmptyRow();
    ui->tbl->setInteger(r, 0, ui->leFuel->getInteger());
    ui->tbl->setString(r, 1, ui->leFuelName->text());
    ui->tbl->setDouble(r, 2, ui->leQty->getDouble());
    ui->tbl->setDouble(r, 3, ui->leInitialPrice->getDouble());
    ui->tbl->setDouble(r, 4, ui->leDiscount->getDouble());
    ui->tbl->setDouble(r, 5, (ui->leInitialPrice->getDouble() - ui->leDiscount->getDouble()) * ui->leQty->getDouble());
    countTotal();
    ui->leFuel->clear();
    ui->leInitialPrice->clear();
    ui->leQty->clear();
    ui->lePrice->clear();
    ui->leDiscount->clear();
    ui->leTotal->clear();
}

void FuelFlashMove::countTotal()
{
    double t = 0;
    for (int i = 0; i < ui->tbl->rowCount(); i++) {
        t += ui->tbl->getDouble(i, 5);
    }
    ui->ltTotal->setDouble(t);
}

void FuelFlashMove::on_btnRemoveRow_clicked()
{
    int r = ui->tbl->currentRow();
    if (r < 0) {
        return;
    }
    ui->tbl->removeRow(r);
    countTotal();
}
