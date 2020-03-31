#include "registerticketdialog.h"
#include "ui_registerticketdialog.h"
#include "c5database.h"
#include "config.h"
#include "c5message.h"

RegisterTicketDialog::RegisterTicketDialog() :
    Dialog(),
    ui(new Ui::RegisterTicketDialog)
{
    ui->setupUi(this);
    ui->leNumberStart->setValidator(new QIntValidator());
    ui->leNumberEnd->setValidator(new QIntValidator());
    ui->wc->enumLineEdit();
    ui->progressBar->setVisible(false);
}

RegisterTicketDialog::~RegisterTicketDialog()
{
    delete ui;
}

void RegisterTicketDialog::on_btnRegister_clicked()
{
    QString err;
    if (ui->leTicket->getInteger() == 0) {
        err += tr("The type of ticked cannot be null.") + "<br>";
    }
    if (ui->leCount->getInteger() < 1) {
        err += tr("Count of the tickets is zero.") + "<br>";
    }
    if (!err.isEmpty()) {
        C5Message::error(err);
        return;
    }
    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
    db.startTransaction();
    QString transaction = db.uuid();
    int start = ui->leNumberStart->getInteger();
    int end = ui->leNumberEnd->getInteger();
    ui->progressBar->setVisible(true);
    ui->progressBar->setMaximum(end);
    for (int i = start; i < end + 1; i++) {
        QString code = QString("%1%2%3")
                .arg(ui->deDate->date().toString("yyyyMMyy"))
                .arg(ui->leTicket->getInteger(), 2, 10, QChar('0'))
                .arg(i, ui->sbDigits->value(), 10, QChar('0'));
        db[":fstate"] = 1;
        db[":ftransaction"] = transaction;
        db[":fissuedate"] = ui->deDate->date();
        db[":fregisterdate"] = QDate::currentDate();
        db[":fregistertime"] = QTime::currentTime();
        db[":ftype"] = ui->leTicket->getInteger();
        db[":fpartner"] = ui->lePartner->getInteger();
        db[":fcode"] = code;
        if (ui->lePartner->getInteger() > 0) {
            db[":fpartnerdate"] = QDate::currentDate();
            db[":fstate"] = 2;
        }
        if (!db.insert("cards", false)) {
            db.rollback();
            C5Message::error(db.fLastError);
            return;
        }
        ui->progressBar->setValue(i);
        qApp->processEvents();
    }
    db.commit();
    ui->btnRegister->setEnabled(false);
    ui->leTicket->setReadOnly(true);
    ui->lePartner->setReadOnly(true);
    ui->deDate->setReadOnly(true);
    ui->leNumberEnd->setReadOnly(true);
    ui->leNumberStart->setReadOnly(true);
    ui->sbDigits->setReadOnly(true);
    C5Message::info(tr("Registered"));
}

void RegisterTicketDialog::on_btnCancel_clicked()
{
    reject();
}

void RegisterTicketDialog::on_leNumberStart_textEdited(const QString &arg1)
{
    ui->leCount->setInteger(ui->leNumberEnd->getInteger() - arg1.toInt() + 1);
}

void RegisterTicketDialog::on_leNumberEnd_textEdited(const QString &arg1)
{
    ui->leCount->setInteger(arg1.toInt() - ui->leNumberStart->getInteger() + 1);
}
