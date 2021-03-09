#include "c5connection.h"
#include "ui_c5connection.h"
#include "config.h"
#include "c5message.h"
#include "c5database.h"
#include <QSettings>
#include <QInputDialog>

#define db_ver 1
#define params_count 6

C5Connection::C5Connection() :
    Dialog(),
    ui(new Ui::C5Connection)
{
    ui->setupUi(this);
    QStringList params;
    readParams(params);
    ui->leHost->setText(params.at(0));
    ui->leDatabase->setText(params.at(1));
    ui->leUsername->setText(params.at(2));
    ui->lePassword->setText(params.at(3));
    ui->sbFont->setValue(params.at(4).toInt());
}

C5Connection::~C5Connection()
{
    delete ui;
}

void C5Connection::configure()
{
    C5Connection *c = new C5Connection();
    c->exec();
    delete c;
}

void C5Connection::readParams(QStringList &p)
{
    QSettings s(_ORGANIZATION_, _APPLICATION_+ QString("\\") + _MODULE_);
    if (s.value("db_ver").toInt() == 0) {
        s.setValue("db_ver", db_ver);
    }
    QByteArray buf = s.value("db").toByteArray();
    for (int i = 0; i < buf.length(); i++) {
        buf[i] = buf[i] ^ ((i % 2) + (i % 3) + (i % 4) + (i % 5) + (i % 6) + (i % 7) + (i % 8) + (i % 9));
    }
    QList<QByteArray> params = buf.split('\r');
    while (params.count() < params_count) {
        params.append("");
        params.append("\r");
    }
    for (const QByteArray &ba: params) {
        p.append(ba);
    }
}

void C5Connection::writeParams()
{
    QByteArray buf;
    buf.append(__dbhost.toUtf8());
    buf.append('\r');
    buf.append(__dbschema.toUtf8());
    buf.append('\r');
    buf.append(__dbusername.toUtf8());
    buf.append('\r');
    buf.append(__dbpassword.toUtf8());
    buf.append('\r');
    buf.append(QString::number(__appFontSize).toUtf8());
    buf.append('\r');

    for (int i = 0; i < buf.length(); i++) {
        buf[i] = buf[i] ^ ((i % 2) + (i % 3) + (i % 4) + (i % 5) + (i % 6) + (i % 7) + (i % 8) + (i % 9));
    }
    QSettings s(_ORGANIZATION_, _APPLICATION_ + QString("\\") + _MODULE_);
    s.setValue("db", buf);
    s.setValue("db_ver", db_ver);
}

bool C5Connection::preambule()
{
    bool ok = false;
    QString password = QInputDialog::getText(__parent, tr("Configuration settings"), tr("Password"), QLineEdit::Password, "", &ok);
    if (ok) {
        if (password != ui->lePassword->text()) {
            C5Message::error(tr("Access denied"));
            return false;
        }
    } else {
        return false;
    }
    return true;
}

void C5Connection::on_btnCancel_clicked()
{
    reject();
}

void C5Connection::on_btnTest_clicked()
{
    C5Database db(ui->leHost->text(), ui->leDatabase->text(), ui->leUsername->text(), ui->lePassword->text());
    if (!db.open()) {
        C5Message::error(tr("Cannot connect to database") + "<br>" + db.fLastError);
        return;
    }
    db.close();
    C5Message::info(tr("Connection successfull"));
}

void C5Connection::on_btnSave_clicked()
{
    __dbhost = ui->leHost->text();
    __dbschema = ui->leDatabase->text();
    __dbusername = ui->leUsername->text();
    __dbpassword = ui->lePassword->text();
    __appFontSize = ui->sbFont->value();
    writeParams();
    C5Message::info(tr("Saved"));
}

void C5Connection::on_btnInit_clicked()
{
    C5Database db(ui->leHost->text(), ui->leDatabase->text(), ui->leUsername->text(), ui->lePassword->text());
    if (!db.open()) {
        C5Message::error(tr("Cannot connect to database") + "<br>" + db.fLastError);
        return;
    }
    bool init = true;
    db.exec("select * from s_db where f_id=1");
    if (db.nextRow()) {
        init = false;
    }
    C5Message::info(tr("Ready to use"));
}
