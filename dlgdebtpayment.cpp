#include "dlgdebtpayment.h"
#include "ui_dlgdebtpayment.h"
#include "c5message.h"
#include "config.h"
#include "C5Database.h"
#include "doctype.h"

DlgDebtPayment::DlgDebtPayment(QWidget *parent) :
    Dialog(parent),
    ui(new Ui::DlgDebtPayment)
{
    ui->setupUi(this);
    ui->wc->enumLineEdit();
}

DlgDebtPayment::~DlgDebtPayment()
{
    delete ui;
}

void DlgDebtPayment::on_btnCancel_clicked()
{
    reject();
}

void DlgDebtPayment::on_btnSave_clicked()
{
    if (ui->lePartner->getInteger() == 0) {
        C5Message::error(tr("Patner must be define"));
        return;
    }
    if (ui->lePayment->getInteger() == 0) {
        C5Message::error(tr("Payment type must be defined"));
        return;
    }
    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
    db.startTransaction();
    fTransaction = db.uuid();
    db[":fdoctype"] = DOCTYPE_DEBTPAY;
    db[":fdatecreate"] = ui->leDocDate->date();
    db[":ftimecreate"] = QTime::currentTime();
    db[":ftransaction"] = fTransaction;
    db[":fpaymenttype"] = ui->lePayment->getInteger();
    db[":finvoice"] = ui->leInvoiceNumber->text();
    fDocNumber = db.insert("docs", true);
    if (fDocNumber == 0) {
        db.rollback();
        C5Message::error(db.fLastError);
        return;
    }
    db[":fid"] = fTransaction;
    db[":fuser"] = __userid;
    if (!db.insert("transactions")) {
        db.rollback();
        C5Message::error(db.fLastError);
        return;
    }
    db[":fdate"] = ui->leDocDate->date();
    db[":fdoc"] = fDocNumber;
    db[":fpartner"] = ui->lePartner->getInteger();
    db[":fdebet"] = ui->lePrice->getDouble();
    db[":fcredit"] = 0;
    if (!db.insert("debts")) {
        db.rollback();
        C5Message::error(db.fLastError);
        return;
    }
    db.commit();
    ui->btnSave->setVisible(false);
    C5Message::info(tr("Saved"));
}
