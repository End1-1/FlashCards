#include "cardranges.h"
#include "ui_cardranges.h"
#include "config.h"
#include "C5Database.h"

CardRanges::CardRanges(int type) :
    Dialog(),
    ui(new Ui::CardRanges)
{
    ui->setupUi(this);
    ui->tbl->setColumnWidths(4, 200, 200, 80, 80);
    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
    db[":fid"] = type;
    db.exec("select fmeas from cards_types where fid=:fid");
    double litr = 0;
    if (db.nextRow()) {
        litr = db.getDouble("fmeas");
    }
    db[":ftype"] = type;
    db.exec("select fcode, fnum from cards where ftype=:ftype and fstate=1 order by fid");
    QString f, l;
    int count = 0;
    int row, num = 0;
    while (db.nextRow()) {
        if (f.isEmpty() || num != db.getInt("fnum") - 1) {
            if (!l.isEmpty()) {
                ui->tbl->setString(row, 1,l);
                ui->tbl->setInteger(row, 2, count);
                ui->tbl->setDouble(row, 3, count * litr);
            }
            f = db.getString("fcode");
            row = ui->tbl->rowCount();
            ui->tbl->setRowCount(ui->tbl->rowCount() + 1);
            ui->tbl->setString(row, 0, f);
            if (num > 0) {

            }
            count = 0;
        }
        count++;
        num = db.getInt("fnum");
        l = db.getString("fcode");
    }
    if (!l.isEmpty()) {
        ui->tbl->setString(row, 1,l);
        ui->tbl->setInteger(row, 2, count);
        ui->tbl->setDouble(row, 3, count * litr);
    }
}

CardRanges::~CardRanges()
{
    delete ui;
}

bool CardRanges::getRange(int type, QString &first, QString &last)
{
    CardRanges d(type);
    if (d.exec() == QDialog::Accepted) {
        first = d.fFirst;
        return true;
    }
    return false;
}

void CardRanges::on_tbl_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(column);
    fFirst = ui->tbl->getString(row, 0);
    accept();
}
