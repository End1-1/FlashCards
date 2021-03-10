#include "login.h"
#include "ui_login.h"
#include "config.h"
#include "c5database.h"
#include "c5message.h"

Login::Login() :
    Dialog(),
    ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

bool Login::login()
{
    Login *l = new Login();
    bool result = l->exec() == QDialog::Accepted;
    delete l;
    return result;
}

void Login::enter()
{
    on_btnOK_clicked();
}

void Login::on_btnCancel_clicked()
{
    reject();
}

void Login::on_btnOK_clicked()
{
    getdb();
    db[":fusername"] = ui->leUsername->text();
    db[":fpassword"] = ui->lePassword->text();
    db.exec("select fid from users where fusername=:fusername and fpassword=:fpassword");
    if (db.nextRow()) {
        __userid = db.getInt(0);
    }
    if (__userid > 0) {
        accept();
    } else {
        C5Message::error(tr("Access denied"));
    }
}
