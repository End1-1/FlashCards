#include "registeruseddialog.h"
#include "ui_registeruseddialog.h"
#include "c5database.h"
#include "config.h"
#include "c5message.h"
#include "doctype.h"
#include "c5selector.h"
#include "c5printing.h"
#include "c5printpreview.h"
#include <QShortcut>

RegisterUsedDialog::RegisterUsedDialog() :
    Dialog(),
    ui(new Ui::RegisterUsedDialog)
{
    ui->setupUi(this);
    ui->wc->enumLineEdit();
    QShortcut *sh = new QShortcut(QKeySequence("Ctrl+Return"), this);
    connect(sh, SIGNAL(activated()), this, SLOT(on_btnWrite_clicked()));
    fTicketId = 0;
    ui->tbl->setColumnWidths(9, 0, 120, 0, 150, 200, 140, 140, 0, 120);
    ui->tblStore->setColumnWidths(4, 0, 200, 80, 80);
    ui->tblStore->setMinimumWidth(400);
    ui->tblStore->setMaximumWidth(400);
    ui->wback->setVisible(false);
    on_deUsed_editingFinished();
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
    int cardstate = 3;
    int doctype = 0;
    switch (ui->comboBox->currentIndex()) {
        case 0:
            doctype = DOCTYPE_USEDTICKET;
            break;
        case 1:
            doctype = DOCTYPE_BACKTICKET;
            cardstate = 4;
            break;
        case 2:
            doctype = DOCTYPE_TRASHTICKET;
            cardstate = 5;
            break;
    }
    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
    db.startTransaction();
    fTransaction = db.uuid();
    db[":fdoctype"] = doctype;
    db[":fdatecreate"] = QDate::currentDate();
    db[":ftimecreate"] = QTime::currentTime();
    db[":ftransaction"] = fTransaction;
    ui->leDocNumber->setInteger(db.insert("docs", true));
    for (int i = 0; i < ui->tbl->rowCount(); i++) {
        db[":fcode"] = ui->tbl->getString(i, 1);
        db[":fstate"] = cardstate;
        db[":fpriceback"] = ui->tbl->getDouble(i, 8);
        db[":fdate"] = ui->deUsed->date();
        db[":fusedtrans"] = fTransaction;
        db.exec("update cards set fstate=:fstate, frefunddate=:fdate, fpriceback=:fpriceback, fusedtrans=:fusedtrans where fcode=:fcode");
    }
    if (doctype == DOCTYPE_USEDTICKET) {
        for (int i = 0; i < ui->tblStore->rowCount(); i++) {
            db[":fid"] = ui->tblStore->getInteger(i, 0);
            db[":fqty"] = ui->tblStore->getDouble(i, 3);
            db.exec("update fuel set fqty=:fqty where fid=:fid");
        }
    }
    db.commit();
    ui->leCode->setEnabled(false);
    ui->btnWrite->setEnabled(false);
    ui->btnZeroArmPetStore->setEnabled(true);
}

void RegisterUsedDialog::on_leCode_returnPressed()
{
    bool pricecomplete = true;
    for (int i = 0; i < ui->tblStore->rowCount(); i++) {
        if (ui->tblStore->getDouble(i, 2) < 0.01) {
            pricecomplete = false;
            break;
        }
    }
    if (ui->tblStore->rowCount() == 0) {
        pricecomplete = false;
    }
    if (!pricecomplete) {
        C5Message::error(tr("The prices of selected date not set"));
        return;
    }
    getTicketByCode();
}

void RegisterUsedDialog::getTicketByCode()
{
    for (int i = 0; i < ui->tbl->rowCount(); i++) {
        if (ui->leCode->text() == ui->tbl->getString(i, 1)) {
            C5Message::error(tr("Duplicate code"));
            return;
        }
    }
    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
    db[":fcode"] = ui->leCode->text();
    db.exec("select c.*, ct.fname as ftypename, "
            "p.fname as fpartnername, ct.ffuel, ct.fmeas "
            "from cards c "
            "left join cards_types ct on ct.fid=c.ftype "
            "left join partners p on p.fid=c.fpartner "
            "where c.fcode=:fcode");
    if (db.nextRow()) {
        QString err;
        if (db.getInt("fstate") != 2 && ui->comboBox->currentIndex() == 0) {
            switch (db.getInt("fstate")) {
                case 1:
                    err = tr("Ticket was issued, but not sold");
                    break;
                case 3:
                    err = tr("Ticket already marked as used") + "<br>" + db.getString("fpartnername");
                    break;
                default:
                    err = tr("Unknow ticket state.");
                    break;
            }
            C5Message::error(err);
            return;
        }
        if (!err.isEmpty()) {
            return;
        }
        int c = 0;
        int r = ui->tbl->addEmptyRow();
        ui->tbl->setInteger(r, c++, db.getInt("fid"));
        ui->tbl->setString(r, c++, ui->leCode->text());
        ui->tbl->setString(r, c++, db.getString("ftype"));
        ui->tbl->setString(r, c++, db.getString("ftypename"));
        ui->tbl->setString(r, c++, db.getString("fpartnername"));
        ui->tbl->setString(r, c++, db.getDate("fissuedate").toString(FORMAT_DATE_TO_STR));
        ui->tbl->setString(r, c++, db.getDate("fpartnerdate").toString(FORMAT_DATE_TO_STR));
        ui->tbl->setString(r, c++, ui->deUsed->text());
        ui->tbl->setString(r, c++, float_str(fuelPrice(db.getInt("ffuel")), 2));
        ui->btnWrite->setEnabled(true);
        ui->deUsed->setEnabled(false);
        for (int i = 0; i < ui->tblStore->rowCount(); i++) {
            if (ui->tblStore->getInteger(i, 0) == db.getInt("ffuel")) {
                ui->tblStore->setDouble(i, 3, ui->tblStore->getDouble(i, 3) - db.getDouble("fmeas"));
                break;
            }
        }
    } else {
        C5Message::error(tr("Unknown code"));
    }
    ui->leCode->clear();
    ui->leCode->setFocus();
}

double RegisterUsedDialog::fuelPrice(int fuel)
{
    for (int i = 0; i < ui->tblStore->rowCount(); i++) {
        if (fuel == ui->tblStore->getInteger(i, 0)) {
            return ui->tblStore->getDouble(i, 2);
        }
    }
    return 0;
}

void RegisterUsedDialog::on_chBack_clicked(bool checked)
{
    ui->wback->setVisible(checked);
}

void RegisterUsedDialog::on_btnGo_clicked()
{
    QString part = ui->leStart->text().mid(0, 10);
    int startcode = ui->leStart->text().mid(11, 4).toInt();
    int endcode = ui->leEnd->text().mid(11, 4).toInt();
    for (int i = startcode; i < endcode + 1; i++) {
        ui->leCode->setText(QString("%1%2").arg(part).arg(i, 4, 10, QChar('0')));
        on_leCode_returnPressed();
    }
}

void RegisterUsedDialog::on_comboBox_currentIndexChanged(int index)
{
    switch (index) {
        case 0:
            ui->wback->setVisible(false);
            break;
        case 1:
        case 2:
            ui->wback->setVisible(true);
            break;
    }
}

void RegisterUsedDialog::on_deUsed_editingFinished()
{
    ui->tblStore->clearContents();
    ui->tblStore->setRowCount(0);
    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
    db[":fdate"] = ui->deUsed->date();
    db.exec("SELECT f.fid, f.fname, fp.fprice, f.fqty  "
            "FROM fuel_prices fp "
            "LEFT JOIN fuel f ON f.fid=fp.ffuel "
            "where fp.fdate=:fdate");
    while (db.nextRow()) {
        int r = ui->tblStore->addEmptyRow();
        ui->tblStore->setInteger(r, 0, db.getInt("fid"));
        ui->tblStore->setString(r, 1, db.getString("fname"));
        ui->tblStore->setString(r, 2, float_str(db.getDouble("fprice"), 2));
        ui->tblStore->setDouble(r, 3, db.getDouble("fqty"));
    }
    bool pricecomplete = true;
    for (int i = 0; i < ui->tblStore->rowCount(); i++) {
        if (ui->tblStore->getDouble(i, 2) < 0.01) {
            pricecomplete = false;
            break;
        }
    }
    if (ui->tblStore->rowCount() == 0) {
        pricecomplete = false;
    }
    ui->leCode->setEnabled(pricecomplete);
    if (!pricecomplete) {
        C5Message::error(tr("The prices of selected date not set"));
    }
}

void RegisterUsedDialog::on_btnPrint_clicked()
{
    if (ui->leDocNumber->getInteger() == 0) {
        C5Message::error(tr("Document not saved."));
        return;
    }
    C5Printing p;
    p.setSceneParams(2800, 2000, QPageLayout::Landscape);
    QFont ffont(font());
    ffont.setPointSize(28);
    ffont.setBold(true);
    p.setFont(ffont);
    p.ltext(tr("<<ARMPETROL>> LTD"), 10);
    p.br();
    p.ctext(QString("%1 %2%3").arg(tr("COUPON CONSUPTION ACT"), tr("NN"), ui->leDocNumber->text()));
    p.br();
    p.ltext(QString("%1: %2").arg(tr("Consuption date"), ui->deUsed->text()), 10);
    p.rtext(QString("%1: %2").arg(tr("Document date"), ui->deDoc->text()));
    p.br();
    p.br();
    p.setFontSize(24);
    QList<qreal> points;
    points << 10 << 100 << 300 << 150 << 600 << 250 << 250;
    QStringList vals;
    vals << tr("NN") << tr("Code") << tr("Type") << tr("Partner") << tr("Issue date") << tr("Consuption date");
    p.tableText(points, vals, p.fLineHeight + 20);
    p.br(p.fLineHeight + 10);
    int r = 1;
    for (int i = 0; i < ui->tbl->rowCount(); i++) {
        vals.clear();
        vals << QString::number(r++)
             << ui->tbl->getString(i, 1)
             << ui->tbl->getString(i, 3)
             << ui->tbl->getString(i, 4)
             << ui->tbl->getString(i, 6)
             << ui->deUsed->text();
        p.tableText(points, vals, p.fLineHeight + 20);
        p.br(p.fLineHeight + 10);
    }
    p.br(p.fLineHeight + 10);
    p.br(p.fLineHeight + 10);
    p.ltext(tr("Operator:"), 10);
    p.line(10, p.fTop + p.fLineHeight, 500, p.fTop + p.fLineHeight);
    p.br();
    p.ltext(tr("(signature)"), 200);
    p.br();
    p.br();
    p.ltext(QString("%1 %2").arg(tr("Printed:"), QDateTime::currentDateTime().toString(FORMAT_DATETIME_TO_STR)), 10);
    C5PrintPreview pp( &p, this);
    pp.exec();
}

void RegisterUsedDialog::on_btnZeroArmPetStore_clicked()
{
    bool empty = true;
    for (int i = 0; i < ui->tblStore->rowCount(); i++) {
        if (ui->tblStore->getDouble(i, 3) > 0.01) {
            empty = false;
            break;
        }
    }
    if (empty) {
        C5Message::error(tr("Nothing to reset"));
        return;
    }
    if (C5Message::question(tr("Confirm to zero store")) != QDialog::Accepted) {
        return;
    }
    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
    db.startTransaction();
    QString trans = db.uuid();
    db[":fdoctype"] = DOCTYPE_STORERESET;
    db[":fdatecreate"] = QDate::currentDate();
    db[":ftimecreate"] = QTime::currentTime();
    db[":ftransaction"] = trans;
    int doc = db.insert("docs", true);
    if (doc == 0) {
        db.rollback();
        C5Message::error(db.fLastError);
        return;
    }
    db[":fid"] = trans;
    db[":fuser"] = __userid;
    if (!db.insert("transactions")) {
        db.rollback();
        C5Message::error(db.fLastError);
        return;
    }
    for (int i = 0; i < ui->tblStore->rowCount(); i++) {
        db[":fdoc"] = doc;
        db[":ffuel"] = ui->tblStore->getInteger(i, 0);
        db[":fqty"] = ui->tblStore->getDouble(i, 3);
        db.insert("docs_reset_store", false);
        db[":fid"] = ui->tblStore->getInteger(i, 0);
        db[":fqty"] = ui->tblStore->getDouble(i, 3);
        db.exec("update fuel set fqty=fqty-:fqty where fid=:fid");
    }
    db.commit();
    on_deUsed_editingFinished();
    C5Message::info(tr("Store was reseted"));
}
