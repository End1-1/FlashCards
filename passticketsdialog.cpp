#include "passticketsdialog.h"
#include "ui_passticketsdialog.h"
#include "c5database.h"
#include "config.h"
#include "c5message.h"
#include <QFile>
#include <QDir>
#include <QUrl>
#include <QDesktopServices>

PassTicketsDialog::PassTicketsDialog() :
    Dialog(),
    ui(new Ui::PassTicketsDialog)
{
    ui->setupUi(this);
    ui->wc->enumLineEdit();
    ui->progressBar->setVisible(false);
}

PassTicketsDialog::~PassTicketsDialog()
{
    delete ui;
}

void PassTicketsDialog::on_btnCancel_clicked()
{
    reject();
}

void PassTicketsDialog::on_leStartCode_returnPressed()
{
    QString err;
    getListOfTickets(err);
}

void PassTicketsDialog::on_leEndCode_returnPressed()
{
    QString err;
    getListOfTickets(err);
}

void PassTicketsDialog::getListOfTickets(QString &err, bool process)
{
    if (ui->lePartner->isEmpty()) {
        err += tr("Partner is null") + "<br>";
    }
    ui->leCount->setInteger(0);
    if (ui->leStartCode->isEmpty()) {
        err += tr("Start code is empty") + "<br>";
    }
    if (ui->leEndCode->isEmpty()) {
        err += tr("End code is empty") + "<br>";
    }
    if (ui->leStartCode->text().length() != 8 + 2 + 4) {
        err += tr("Start code has invalid lenght") + "<br>";
    }
    if (ui->leStartCode->text().length() != 8 + 2 + 4) {
        err += tr("End code has invalid lenght") + "<br>";
    }
    QDate issueDate1 = QDate::fromString(ui->leStartCode->text().mid(0, 8), "yyyyMMdd");
    int ticketType1 = ui->leStartCode->text().mid(8, 2).toInt();
    int startNumber = ui->leStartCode->text().mid(10, 4).toInt();
    QDate issueDate2 = QDate::fromString(ui->leEndCode->text().mid(0, 8), "yyyyMMdd");
    int ticketType2 = ui->leEndCode->text().mid(8, 2).toInt();
    int endNumber = ui->leEndCode->text().mid(10, 4).toInt();
    if (issueDate1 != issueDate2) {
        err += tr("The range of the tickets has different date of issues") + "<br>";
    }
    if (ticketType1 != ticketType2) {
        err += tr("The type of beginning ticket and the type of end ticket is not same") + "<br>";
    }
    if (startNumber > endNumber) {
        err += tr(" The starting number is greater than the ending") + "<br>";
    }
    if (issueDate1.isValid() && issueDate2.isValid()) {
        ui->leCount->setInteger(endNumber - startNumber + 1);
        ui->progressBar->setMaximum(ui->leCount->getInteger());
    }
    ui->wc->setKey(ui->leTicket, ticketType1);
    if (process && err.isEmpty()) {
        ui->progressBar->setVisible(true);
        ui->progressBar->setMinimum(startNumber);
        ui->progressBar->setMaximum(endNumber - 1);
        C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
        int notPassed = 0;
        QStringList notPassedList;
        for (int i = startNumber; i < endNumber; i++) {
            QString code = QString("%1%2%3").arg(issueDate1.toString("yyyyMMdd")).arg(ui->leTicket->getInteger(), 2, 10, QChar('0')).arg(i, 4, 10, QChar('0'));
            db[":fcode"] = code;
            db[":fpartner"] = ui->lePartner->getInteger();
            db[":fstate"] = 2;
            db.exec("update cards set fstate=:fstate, fpartner=:fpartner, fpartnerdate=current_date where fcode=:fcode and fstate=1 ");
            if (db.fAffectedRows != 1) {
                notPassed++;
                notPassedList.append(QString("%1 %2 %3 %4").arg(code).arg(issueDate1.toString(FORMAT_DATE_TO_STR)).arg(ui->leTicketName->text(), 4, QChar(' ')).arg(i, 4, 10, QChar('0')));
            }
            ui->progressBar->setValue(i);
            qApp->processEvents();
        }
        if (notPassed > 0) {
            notPassedList.insert(0, "\r\n");
            notPassedList.insert(0, QString("%1: %2").arg(tr("Not passed")).arg(notPassed));
            QDir d;
            QString fileName = d.tempPath() + "/notpassed.txt";
            QFile f(fileName);
            if (f.open(QIODevice::WriteOnly)) {
                f.write(notPassedList.join("\r\n").toUtf8());
                f.close();
                QDesktopServices::openUrl(QUrl(fileName));
            }
        }
    }
}

void PassTicketsDialog::on_btnSave_clicked()
{
    QString err;
    getListOfTickets(err, true);
    if (!err.isEmpty()) {
        C5Message::error(err);
        return;
    }
    C5Message::info(tr("Tickets issued"));
}
