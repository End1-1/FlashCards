#include "dlgpassword.h"
#include "ui_dlgpassword.h"
#include "C5Database.h"
#include "config.h"
#include <QMessageBox>

DlgPassword::DlgPassword(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgPassword)
{
    ui->setupUi(this);
}

DlgPassword::~DlgPassword()
{
    delete ui;
}

void DlgPassword::on_btnChange_clicked()
{
    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
    db[":fid"] = __userid;
    db[":fpass"] = ui->leOldPassword->text();
    db.exec("select fid, frole from users where fid=:fid and fpassword=md5(:fpass)");
    if (db.nextRow()) {
        db[":fid"] = __userid;
        db[":fpass"] = ui->leNewPassword->text();
        db.exec("update users set fpassword=md5(:fpass) where fid=:fid");
        QMessageBox::information(this, tr("Password"), tr("New password set"));
        accept();
    } else {
        QMessageBox::critical(this, tr("Password"), tr("Incorrect old password"));
    }
}

void DlgPassword::on_btnCancel_clicked()
{
    reject();
}
