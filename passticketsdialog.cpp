#include "passticketsdialog.h"
#include "ui_passticketsdialog.h"
#include "c5database.h"
#include "config.h"
#include "c5message.h"
#include "c5selector.h"
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
    ui->leCountGive->setVisible(false);
    ui->btnRollback->setVisible(false);
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
    if (ui->leStartCode->text().length() < 5) {
        C5Selector *s = new C5Selector();
        QString sql  = QString("select ct.fname, s.fissuedate, s.fcode "
                               "from cards s left "
                               "join cards_types ct on ct.fid=s.ftype "
                               "where s.fstate=1 and cast(right(s.fcode, 4) as unsigned) >= 1").arg(ui->leStartCode->text().toInt(), 4, 10, QChar('0'));
        if (!ui->leTicket->isEmpty()) {
            sql += " and s.ftype=" + ui->leTicket->text();
        }
        QList<QVariant> v;
        QMap<QString, QString> t;
        t["fname"] = tr("Ticket");
        t["fissuedate"] = tr("Issue");
        t["fcode"] = tr("Code");
        if (s->getValues(sql, v, t)) {
            ui->leStartCode->setText(v.at(3).toString());
            on_leStartCode_returnPressed();
        }
        return;
    }
    QString err;
    getListOfTickets(err);
}

void PassTicketsDialog::on_leEndCode_returnPressed()
{
    if (ui->leEndCode->text().length() < 5) {
        C5Selector *s = new C5Selector();
        QString sql  = QString("select ct.fname, s.fissuedate, s.fcode from cards s left join cards_types ct on ct.fid=s.ftype where s.fstate=1 and s.fcode like '%%1'").arg(ui->leEndCode->text().toInt(), 4, 10, QChar('0'));
        if (!ui->leTicket->isEmpty()) {
            sql += " and s.ftype=" + ui->leTicket->text();
        }
        QList<QVariant> v;
        QMap<QString, QString> t;
        t["fname"] = tr("Ticket");
        t["fissuedate"] = tr("Issue");
        t["fcode"] = tr("Code");
        if (s->getValues(sql, v, t)) {
            ui->leEndCode->setText(v.at(3).toString());
            on_leStartCode_returnPressed();
        }
        return;
    }
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
    if (ui->lePrice->getDouble() < 0.001) {
        err += tr("Price is empty") + "<br>";
    }
    QDate issueDate1 = QDate::fromString(ui->leStartCode->text().mid(0, 8), "yyyyMMdd");
    if (issueDate1.isValid()) {
        ui->deDate->setDate(issueDate1);
    }
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
        db.startTransaction();
        int notPassed = 0;
        QStringList notPassedList;
        for (int i = startNumber; i < endNumber + 1; i++) {
            QString code = QString("%1%2%3").arg(issueDate1.toString("yyyyMMdd")).arg(ui->leTicket->getInteger(), 2, 10, QChar('0')).arg(i, 4, 10, QChar('0'));
            db[":fcode"] = code;
            db[":fpartner"] = ui->lePartner->getInteger();
            db[":fstate"] = 2;
            db[":fprice"] = ui->lePrice->getDouble();
            db[":fpartnertrans"] = fTransaction;
            db[":fpaymenttype"] = ui->cbPaymentType->currentIndex() + 1;
            db.exec("update cards set fstate=:fstate, fpartner=:fpartner, fpartnerdate=current_date, fprice=:fprice, "
                    "fpartnertrans=:fpartnertrans where fcode=:fcode and fstate=1 ");
            if (db.fAffectedRows != 1) {
                notPassed++;
                notPassedList.append(QString("%1 %2 %3 %4").arg(code).arg(issueDate1.toString(FORMAT_DATE_TO_STR)).arg(ui->leTicketName->text(), 4, QChar(' ')).arg(i, 4, 10, QChar('0')));
            }
            ui->progressBar->setValue(i);
            qApp->processEvents();
        }
        if (notPassed > 0) {
            db.rollback();
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
        } else {
            db.commit();
        }
    }
}

void PassTicketsDialog::on_btnSave_clicked()
{
    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
    fTransaction = db.uuid();
    QString err;
    getListOfTickets(err, true);
    if (!err.isEmpty()) {
        C5Message::error(err);
        return;
    }
    ui->btnSave->setEnabled(false);
    ui->btnRollback->setVisible(true);
    C5Message::info(tr("Tickets issued"));
}

void PassTicketsDialog::on_leCount_returnPressed()
{
    if (ui->leTicket->getInteger() == 0) {
        return;
    }
    ui->leCountGive->setVisible(false);
    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
    db[":ftype"] = ui->leTicket->getInteger();
    db.exec("select min(fid) as fid, fcode from cards where ftype=:ftype and fstate=1");
    int nextnum = 0;
    int count = 0;
    db[":ftype"] = ui->leTicket->getInteger();
    db.exec("select * from cards where ftype=:ftype and fstate=1 order by fissuedate, fid");
    while (db.nextRow() && count < ui->leCount->getInteger()) {
        if (count == 0) {
            count++;
            qDebug() << db.fDbRows;
            ui->leStartCode->setText(db.getString("fcode"));
            QDate issueDate1 = QDate::fromString(ui->leStartCode->text().mid(0, 8), "yyyyMMdd");
            if (issueDate1.isValid()) {
                ui->deDate->setDate(issueDate1);
            }
            nextnum = db.getString("fcode").right(4).toInt();
            continue;
        }
        ui->leEndCode->setText(db.getString("fcode"));
        int codenum = db.getString("fcode").right(4).toInt();
        if (codenum - 1 != nextnum || ui->deDate->date() != QDate::fromString(db.getString("fcode").left(8), "yyyyMMdd")) {
            ui->leCountGive->setInteger(count);
            ui->leCountGive->setVisible(true);
            C5Message::error(tr("Break detected."));
            break;
        }
        nextnum++;
        count++;
    }
    db[":fid"] = ui->leTicket->getInteger();
    db[":fqty"] = count;
    db.exec("select fmeas*:fqty from cards_types where fid=:fid");
    if (db.nextRow()) {
        ui->leQty->setDouble(db.getDouble(0));
    }
}

void PassTicketsDialog::on_leQty_returnPressed()
{
    if (ui->leTicket->getInteger() == 0) {
        return;
    }
    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
    db[":fid"] = ui->leTicket->getInteger();
    db.exec("select fmeas from cards_types where fid=:fid");
    if (db.nextRow()) {
        int mod = ui->leQty->getInteger() % db.getInt(0);
        ui->leQty->setInteger(ui->leQty->getInteger() - mod);
        ui->leCount->setInteger(ui->leQty->getInteger() / db.getInt(0));
        on_leCount_returnPressed();
    }
}

void PassTicketsDialog::on_btnRollback_clicked()
{
    if (C5Message::question(tr("Are you sure to rollback transaction?")) != QDialog::Accepted) {
        return;
    }
    ui->progressBar->setVisible(false);
    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
    db[":fpartnertrans"] = fTransaction;
    db.exec("update cards set fprice=0, fpartner=0, fpartnerdate=null, fstate=1 where fstate=2 and fpartnertrans=:fpartnertrans");
    ui->btnRollback->setVisible(false);
    ui->btnSave->setEnabled(true);
}

void PassTicketsDialog::on_lePrice_textEdited(const QString &arg1)
{
    ui->leAmount->setDouble(arg1.toDouble() * ui->leQty->getDouble());
}
