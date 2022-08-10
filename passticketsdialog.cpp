#include "passticketsdialog.h"
#include "ui_passticketsdialog.h"
#include "config.h"
#include "c5message.h"
#include "c5selector.h"
#include "cardranges.h"
#include "doctype.h"
#include "c5printing.h"
#include "c5printpreview.h"
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
    ui->leCount->setValidator(new QIntValidator());
    ui->leQty->setValidator(new QIntValidator());
    ui->leDiscount->setValidator(new QDoubleValidator(0, 999999, 2));
    ui->tbl->setColumnWidths(10, 0, 50, 200, 200, 100, 100, 100, 200, 150, 0);
    adjustSize();
}

PassTicketsDialog::PassTicketsDialog(int id) :
    PassTicketsDialog()
{
    openDoc(id);
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
    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
    db[":fcode"] = ui->leStartCode->text();
    db.exec("select * from cards where fcode=:fcode");
    if (db.nextRow()) {
        if (db.getInt("fstate") != 1) {
            C5Message::error(QString("%1<br>%2").arg(ui->leStartCode->text(), tr("This ticket already saled.")));
            ui->leStartCode->clear();
            return;
        }
        ui->wc->setKey(ui->leTicket, db.getInt("ftype"));
        setPrice();
    } else {
        ui->leStartCode->clear();
        C5Message::error(tr("Invalid code"));
    }
}

void PassTicketsDialog::on_leEndCode_returnPressed()
{
    if (ui->leStartCode->isEmpty()) {
        ui->leEndCode->clear();
        C5Message::error(tr("Enter the start code first."));
        return;
    }
    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
    db[":fid"] = ui->leTicket->getInteger();
    db.exec("select fmeas from cards_types where fid=:fid");
    double litr = 0;
    if (db.nextRow()) {
        litr = db.getDouble("fmeas");
    }
    db[":fcode"] = ui->leStartCode->text();
    db.exec("select fid, fnum, fstate, fissuetrans from cards where fcode=:fcode");
    int fid1 = 0;
    if (db.nextRow()) {
        fid1 = db.getInt("fid");
        if (db.getInt("fstate") != 1) {
            ui->leStartCode->clear();
            C5Message::error(tr("This ticket already used"));
            return;
        }
    }

    db[":fcode"] = ui->leEndCode->text();
    db.exec("select fid, fnum, fstate, fissuetrans from cards where fcode=:fcode");
    int fid2 = 0;
    if (db.nextRow()) {
        fid2 = db.getInt("fid");
        if (db.getInt("fstate") != 1) {
            ui->leStartCode->clear();
            C5Message::error(tr("This ticket already used"));
            return;
        }
    }

    if (fid1 == 0) {
        C5Message::error(tr("Start code error"));
        return;
    }
    if (fid2 == 0) {
        C5Message::error(tr("End code error"));
        return;
    }
    if (fid2 < fid1) {
        C5Message::error(tr("End code cannot be greater than start code"));
        return;
    }

    ui->leCount->setInteger(fid2 - fid1 + 1);
    ui->leQty->setDouble(ui->leCount->getInteger() * litr);

    db[":fid1"] = fid1;
    db[":fid2"] = fid2;
    db.exec("select * from cards where fid between :fid1 and :fid2 order by fid");
    QString ltrans;
    while (db.nextRow()) {
        if (ltrans.isEmpty()) {
            ltrans = db.getString("fissuetrans");
        }
        if (ltrans != db.getString("fissuetrans")) {
            ui->leCount->setInteger(0);
            ui->leQty->setDouble(0);
            C5Message::error(tr("Break detected."));
            return;
        }
        if (db.getInt("fstate") != 1) {
            ui->leCount->setInteger(0);
            ui->leQty->setDouble(0);
            C5Message::error(tr("Break detected."));
            return;
        }
    }
    ui->leAmount->setDouble(ui->leQty->getDouble() * ui->lePrice->getDouble());
}

void PassTicketsDialog::getListOfTickets(C5Database &db, QString &err, bool process)
{
    if (ui->lePartner->isEmpty()) {
        err += tr("Partner is null") + "<br>";
    }

    ui->progressBar->setMaximum(fTotalCount);
    for (int i = 0; i < ui->tbl->rowCount(); i++) {
        int startnumber = ui->tbl->getString(i, 2).right(5).toInt();
        int endnumber = ui->tbl->getString(i, 3).right(5).toInt();
        QString prefix = ui->tbl->getString(i, 2).left(5);
        if (process && err.isEmpty()) {
            ui->progressBar->setVisible(true);
            ui->progressBar->setMinimum(startnumber);
            ui->progressBar->setMaximum(endnumber - 1);
            int notPassed = 0;
            QStringList notPassedList;
            for (int c = startnumber; c < endnumber + 1; c++) {
                QString code = QString("%1%2").arg(prefix).arg(c, 5, 10, QChar('0'));
                db[":fcode"] = code;
                db[":fpartner"] = ui->lePartner->getInteger();
                db[":fstate"] = 2;
                db[":fprice"] = ui->tbl->getDouble(i, 6);
                db[":fpricediscount"] = ui->tbl->getDouble(i, 9);
                db[":fpartnertrans"] = fTransaction;
                db[":fpaymenttype"] = ui->cbPaymentType->currentIndex() + 1;
                db.exec("update cards set fstate=:fstate, fpartner=:fpartner, fpartnerdate=current_date, fprice=:fprice, "
                        "fpartnertrans=:fpartnertrans, fpricediscount=:fpricediscount where fcode=:fcode and fstate=1 ");
                if (db.fAffectedRows != 1) {
                    notPassed++;
                    notPassedList.append(code);
                }
                ui->progressBar->setValue(c);
                qApp->processEvents();
            }

            db[":fdoc"] = ui->leDocNumber->getInteger();
            db[":frow"] = i;
            db[":ftickettype"] = ui->tbl->getInteger(i, 0);
            db[":fstartnumber"] = ui->tbl->getString(i, 2);
            db[":fendnumber"] = ui->tbl->getString(i, 3);
            db[":fcount"] = ui->tbl->getInteger(i, 4);
            db[":fqty"] = ui->tbl->getDouble(i, 5);
            db[":fprice"] = ui->tbl->getDouble(i, 6);
            db[":fdiscount"] = ui->tbl->getDouble(i, 9);
            db.insert("docs_pass", false);

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
}

void PassTicketsDialog::countTotal()
{
    double total = 0;
    fTotalCount = 0;
    for (int i = 0; i < ui->tbl->rowCount(); i++) {
        total += str_float(ui->tbl->getString(i, 7));
        fTotalCount += ui->tbl->getInteger(i, 0);
    }
    ui->leTotal->setDouble(total);
}

void PassTicketsDialog::setPrice()
{
    ui->leCurrentPrice->clear();
    ui->leDiscount->clear();
    if (ui->leTicket->getInteger() == 0) {
        return;
    }
    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
    db[":fid"] = ui->leTicket->getInteger();
    db[":fdate"] = ui->leDocDate->date();
    db.exec("select fp.fprice "
            "from fuel_prices fp "
            "inner join fuel f on f.fid=fp.ffuel "
            "inner join cards_types ct on ct.ffuel=f.fid "
            "where fp.fdate=:fdate and ct.fid=:fid");
    if (db.nextRow()) {
        ui->leCurrentPrice->setDouble(db.getDouble("fprice"));
    }
    if (ui->leCurrentPrice->getDouble() < 0.01) {
        C5Message::error(tr("Current price is not set."));
    }
    ui->lePrice->setDouble(ui->leCurrentPrice->getDouble() - ui->leDiscount->getDouble());
}

void PassTicketsDialog::openDoc(int id)
{
   ui->leTicket->setEnabled(false);
   ui->leStartCode->setEnabled(false);
   ui->leEndCode->setEnabled(false);
   ui->btnGroups->setEnabled(false);
   ui->btnAddRow->setEnabled(false);
   ui->btnRemoveRow->setEnabled(false);
   ui->leDocDate->setReadOnly(true);
   ui->lePartner->setEnabled(false);
   ui->leCount->setEnabled(false);
   ui->leCurrentPrice->setEnabled(false);
   ui->leDiscount->setEnabled(false);
   ui->lePrice->setEnabled(false);
   ui->leQty->setEnabled(false);
   ui->btnSave->setVisible(false);
   ui->btnPrint->setEnabled(true);

   C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
   db[":fid"] = id;
   db.exec("select * from docs where fid=:fid");
   if (!db.nextRow()) {
       C5Message::error(tr("Invalid document number"));
       return;
   }
   fTransaction = db.getString("ftransaction");
   ui->leDocDate->setDate(db.getDate("fdatecreate"));
   ui->leDocNumber->setInteger(db.getInt("fid"));
   ui->cbPaymentType->setCurrentIndex(db.getInt("fpaymenttype") - 1);
   ui->cbPaymentType->setEnabled(db.getInt("fpaymentstate") == 2);

   db[":fpartnertrans"] = fTransaction;
   db.exec("select * from cards where fpartnertrans=:fpartnertrans limit 1");
   while (db.nextRow()) {
       ui->wc->setKey(ui->lePartner, db.getInt("fpartner"));
   }

   db[":fdoc"] = id;
   db.exec("select dp.ftickettype, ct.fname, dp.fstartnumber, dp.fendnumber, "
           "dp.fcount, dp.fqty, dp.fprice, dp.fqty*dp.fprice as ftotal "
           "from docs_pass dp "
            "left join cards_types ct on ct.fid=dp.ftickettype "
            "where fdoc=:fdoc order by frow");
   double total = 0;
   while (db.nextRow()) {
       int r = ui->tbl->addEmptyRow();
       ui->tbl->setInteger(r, 0, db.getInt("ftickettype"));
       ui->tbl->setString(r, 1, db.getString("fname"));
       ui->tbl->setString(r, 2, db.getString("fstartnumber"));
       ui->tbl->setString(r, 3, db.getString("fendnumber"));
       ui->tbl->setInteger(r, 4, db.getInt("fcount"));
       ui->tbl->setDouble(r, 5, db.getDouble("fqty"));
       ui->tbl->setDouble(r, 6, db.getDouble("fprice"));
       ui->tbl->setDouble(r, 7, db.getDouble("ftotal"));
       total += db.getDouble("ftotal");
   }
   ui->leTotal->setDouble(total);
}

void PassTicketsDialog::on_btnSave_clicked()
{
    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
    db.startTransaction();
    if (fTransaction.isEmpty()) {
        fTransaction = db.uuid();
        db[":fcompany"] = 1;
        db[":fdoctype"] = DOCTYPE_PASSTICKET;
        db[":fdatecreate"] = QDate::currentDate();
        db[":ftimecreate"] = QTime::currentTime();
        db[":ftransaction"] = fTransaction;
        db[":fpaymenttype"] = ui->cbPaymentType->currentIndex() + 1;
        db[":fpaymentstate"] = 1;
        db[":finvoice"] = ui->leInvoiceNumber->text();
        ui->leDocNumber->setInteger(db.insert("docs", true));

        db[":fdate"] = QDate::currentDate();
        db[":fdoc"] = ui->leDocNumber->getInteger();
        db[":fpartner"] = ui->lePartner->getInteger();
        db[":fdebet"] = ui->lePaid->getDouble();
        db[":fcredit"] = ui->leTotal->getDouble();
        db.insert("debts", false);
    }
    db[":fid"] = fTransaction;
    db[":fuser"] = __userid;
    if (!db.insert("transactions")) {
        db.rollback();
        C5Message::error(db.fLastError);
        return;
    }
    QString err;
    getListOfTickets(db, err, true);
    if (!err.isEmpty()) {
        fTransaction.clear();
        C5Message::error(err);
        return;
    }
    db.commit();
    ui->btnSave->setEnabled(false);
    ui->btnPrint->setEnabled(true);
    //ui->btnRollback->setVisible(true);
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

    db[":fid"] = ui->leTicket->getInteger();
    db.exec("select fmeas from cards_types where fid=:fid");
    if (db.nextRow()) {
        ui->leQty->setInteger(ui->leCount->getInteger() * db.getInt(0));
    }

    QString and;
    if (!ui->leStartCode->text().isEmpty()) {
        db[":fcode"] = ui->leStartCode->text();
        db.exec("select fid from cards where fcode=:fcode");
        if (db.nextRow()) {
            and = QString(" and fid>=%1").arg(db.getInt(0));
        }
    }
    db[":ftype"] = ui->leTicket->getInteger();
    db.exec(QString("select * from cards where ftype=:ftype and fstate=1 %and% order by fissuedate, fid").replace("%and%", and));
    QDate id;
    while (db.nextRow() && count < ui->leCount->getInteger()) {
        if (count == 0) {
            count++;
            ui->leStartCode->setText(db.getString("fcode"));
            QDate issueDate1 = db.getDate("fissuedate");
            if (issueDate1.isValid()) {
                id = issueDate1;
            }
            nextnum = db.getString("fcode").right(5).toInt();
            continue;
        }
        ui->leEndCode->setText(db.getString("fcode"));
        int codenum = db.getString("fcode").right(5).toInt();
        if (codenum - 1 != nextnum || id != db.getDate("fissuedate")) {
            ui->leCountGive->setInteger(count);
            ui->leCountGive->setVisible(true);
            C5Message::error(tr("Break detected."));
            break;
        }
        nextnum++;
        count++;
    }
    ui->leAmount->setDouble(ui->leQty->getDouble() * ui->lePrice->getDouble());
//    db[":fid"] = ui->leTicket->getInteger();
//    db[":fqty"] = count;
//    db.exec("select fmeas*:fqty from cards_types where fid=:fid");
//    if (db.nextRow()) {
//        ui->leQty->setDouble(db.getDouble(0));
//    }
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
    ui->leAmount->setDouble(ui->leQty->getDouble() * ui->lePrice->getDouble());
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
    ui->leAmount->setDouble(str_float(arg1) * ui->leQty->getDouble());
    ui->leDiscount->setDouble(ui->leCurrentPrice->getDouble() - str_float(arg1));
}

void PassTicketsDialog::on_btnAddRow_clicked()
{
    QString err;
    if (ui->leCurrentPrice->getDouble() < 0.01) {
        err += tr("Current price is not set.");
    }
    if (ui->leStartCode->isEmpty()) {
        err += tr("Start code is empty") + "<br>";
    }
    if (ui->leEndCode->isEmpty()) {
        err += tr("End code is empty") + "<br>";
    }
    if (ui->leStartCode->text().length() != 1 + 2 + 2 + 5) {
        err += tr("Start code has invalid lenght") + "<br>";
    }
    if (ui->leEndCode->text().length() != 1 + 2 + 2 + 5) {
        err += tr("End code has invalid lenght") + "<br>";
    }
    if (ui->lePrice->getDouble() < 0.001) {
        err += tr("Price is empty") + "<br>";
    }
    if (!err.isEmpty()) {
        C5Message::error(err);
        return;
    }

    int r = ui->tbl->addEmptyRow();
    ui->tbl->setString(r, 0, ui->leTicket->text());
    ui->tbl->setString(r, 1, ui->leTicketName->text());
    ui->tbl->setString(r, 2, ui->leStartCode->text());
    ui->tbl->setString(r, 3, ui->leEndCode->text());
    ui->tbl->setString(r, 4, ui->leCount->text());
    ui->tbl->setString(r, 5, ui->leQty->text());
    ui->tbl->setDouble(r, 6, ui->lePrice->getDouble());
    ui->tbl->setString(r, 7, ui->leAmount->text());
    ui->tbl->setString(r, 8, "");
    ui->tbl->setDouble(r, 9, ui->leDiscount->getDouble());
    countTotal();

    ui->leCount->setInteger(0);
    ui->leTicket->setText("");
    ui->leStartCode->clear();
    ui->leEndCode->clear();
    ui->leQty->clear();
    ui->lePrice->clear();
    ui->leAmount->clear();
    ui->leTicketName->clear();
}

void PassTicketsDialog::on_btnRemoveRow_clicked()
{
    QModelIndexList ml = ui->tbl->selectionModel()->selectedIndexes();
    if (ml.count() == 0) {
        return;
    }
    QSet<int> tempRows;
    for (QModelIndex mi: ml) {
        tempRows.insert(mi.row());
    }
    QList<int> rows;
    for (int i: tempRows) {
        rows.append(i);
    }
    std::sort(rows.begin(), rows.end());
    std::reverse(rows.begin(), rows.end());
    for (int i: rows) {
        ui->tbl->removeRow(i);
    }
    countTotal();
}

void PassTicketsDialog::on_btnGroups_clicked()
{
    if (ui->leTicket->getInteger() == 0) {
        C5Message::error(tr("Select the ticket type"));
        return;
    }
    QString f, l;
    if (CardRanges::getRange(ui->leTicket->getInteger(), f, l)) {
        ui->leStartCode->setText(f);
        on_leStartCode_returnPressed();
    }
}

void PassTicketsDialog::on_leTicketName_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    setPrice();
}

void PassTicketsDialog::on_leDiscount_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    ui->lePrice->setDouble(ui->leCurrentPrice->getDouble() - ui->leDiscount->getDouble());
}

void PassTicketsDialog::on_btnPrint_clicked()
{
    if (ui->leDocNumber->getInteger() == 0) {
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
    QString strTemp = QString("%1 %2").arg(tr("Date"), ui->leDocDate->text());
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
    p.ltext(ui->cbPaymentType->currentText(), baseleft + 450);
    p.ltext(ui->cbPaymentType->currentText(), baseleft + sl + 450);
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
    db[":fpartnertrans"] = fTransaction;
    db.exec("SELECT f.fname, c.fpricediscount, c.fprice, SUM(ct.fmeas) as fmeas, SUM(ct.fmeas*c.fprice) as famount "
            "FROM cards c "
            "LEFT JOIN cards_types ct ON ct.fid=c.ftype "
            "LEFT JOIN fuel_flash f ON f.fid=ct.ffuel "
            "WHERE c.fpartnertrans=:fpartnertrans "
            "GROUP BY f.fname, c.fprice, c.fpricediscount "
            "ORDER BY f.fid ");
    int r = 1;
    double total = 0;
    while (db.nextRow()) {
        vals.clear();
        vals << QString::number(r++)
             << db.getString("fname")
             << tr("Litr")
             << float_str(db.getDouble("fmeas"), 2)
             << float_str(db.getDouble("fprice"), 2)
             << float_str(db.getDouble("fpricediscount"), 2)
             << float_str(db.getDouble("famount"), 2);
        p.tableText(points, vals, p.fLineHeight + 30);
        p.tableText(points2, vals, p.fLineHeight + 30);
        p.br(p.fLineHeight + 20);

        total += db.getDouble("famount");
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


    p.ltext(tr("Delivered tickets by type"), baseleft);
    p.ltext(tr("Delivered tickets by type"), baseleft + sl + 50);
    p.br();
    points.clear();
    points << baseleft << 100 << 300 << 150 << 200 << 100 << 250;
    points2 = points;
    points2[0] = points2[0] + sl;
    vals.clear();
    vals << tr("NN") << tr("Fuel") << tr("Type") << tr("Count") << "" << tr("Count");
    p.tableText(points, vals, p.fLineHeight + 30);
    p.tableText(points2, vals, p.fLineHeight + 30);
    p.br(p.fLineHeight + 20);
    r = 1;
    total = 0;
    db[":fdoc"] = ui->leDocNumber->getInteger();
    db.exec("SELECT f.fname AS ffuelname, ct.fname AS ftypename, ct.fmeas, d.fcount, d.fqty "
            "FROM docs_pass d "
            "LEFT JOIN cards_types ct ON ct.fid=d.ftickettype "
            "LEFT JOIN fuel f ON f.fid=ct.ffuel "
            "WHERE d.fdoc=:fdoc "
            "ORDER BY d.frow ");
    while (db.nextRow()) {
        vals.clear();
        vals << QString::number(r++)
             << db.getString("ffuelname")
             << db.getString("ftypename")
             << float_str(db.getDouble("fcount"), 2) + QString::fromUtf8(" հատ")
             << ""
             << float_str(db.getDouble("fqty"), 2) + QString::fromUtf8(" լիտր");
        p.tableText(points, vals, p.fLineHeight + 30);
        p.tableText(points2, vals, p.fLineHeight + 30);
        p.br(p.fLineHeight + 20);

        total += db.getDouble("fqty");
    }
    points.clear();
    points << baseleft + 850 << 250;
    points2 = points;
    points[0] = points[0] + sl;
    vals.clear();
    vals << float_str(total, 2) + QString::fromUtf8(" լիտր");
    p.tableText(points, vals, p.fLineHeight + 30);
    p.tableText(points2, vals, p.fLineHeight + 30);
    p.br(p.fLineHeight + 20);
    p.br(p.fLineHeight + 20);
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

void PassTicketsDialog::on_lePrice_textChanged(const QString &arg1)
{
    ui->leAmount->setDouble(ui->leQty->getDouble() * str_float(arg1));
}
