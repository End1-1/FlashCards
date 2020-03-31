#include "registeruseddialog.h"
#include "ui_registeruseddialog.h"
#include "c5database.h"
#include "config.h"
#include "c5message.h"
#include <QShortcut>

RegisterUsedDialog::RegisterUsedDialog() :
    Dialog(),
    ui(new Ui::RegisterUsedDialog)
{
    ui->setupUi(this);
    ui->wc->enumLineEdit();
    QShortcut *sh = new QShortcut(QKeySequence(Qt::Key_Control, Qt::Key_Enter), this);
    connect(sh, SIGNAL(activated()), this, SLOT(on_btnWrite_clicked()));
    sh = new QShortcut(QKeySequence(Qt::Key_Control, Qt::Key_Return), this);
    connect(sh, SIGNAL(activated()), this, SLOT(on_btnWrite_clicked()));
    fTicketId = 0;
}

RegisterUsedDialog::~RegisterUsedDialog()
{
    delete ui;
}

void RegisterUsedDialog::on_btnClose_clicked()
{
    reject();
}

void RegisterUsedDialog::on_btnWrite_clicked()
{
    if (fTicketId == 0) {
        C5Message::error(tr("Please, enter the code of the ticked"));
        return;
    }
    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
    db[":fcode"] = ui->leCode->text();
    db[":fstate"] = 3;
    db[":fdate"] = QDate::currentDate();
    db.exec("update cards set fstate=:fstate, frefunddate=:fdate where fcode=:fcode");
    ui->leCode->clear();
    ui->wc->setKey(ui->leTicket, "");
    ui->wc->setKey(ui->lePartner, "");
    ui->leCode->setFocus();
    ui->btnWrite->setEnabled(false);
}

void RegisterUsedDialog::on_leCode_returnPressed()
{
    getTicketByCode();
}

void RegisterUsedDialog::getTicketByCode()
{
    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
    db[":fcode"] = ui->leCode->text();
    db.exec("select * from cards where fcode=:fcode");
    if (db.nextRow()) {
        if (db.getInt("fstate") != 2) {
            QString err;
            switch (db.getInt("fstate")) {
            case 1:
                err = tr("Ticket was issued, but not registered to partner");
                break;
            case 3:
                err = tr("Ticket already marked as used");
                break;
            default:
                err = tr("Unknow ticket state.");
                break;
            }
            C5Message::error(err);
            return;
        }
        ui->btnWrite->setEnabled(true);
        fTicketId = db.getInt("fid");
        ui->wc->setKey(ui->leTicket, db.getString("ftype"));
        ui->wc->setKey(ui->lePartner, db.getString("fpartner"));
        ui->deIssue->setDate(db.getDate("fissuedate"));
        ui->dePassed->setDate(db.getDate("fpartnerdate"));
        ui->deUsed->setDate(QDate::currentDate());
    } else {
        C5Message::error(tr("Unknown code"));
    }
}
