#include "wdbpartner.h"
#include "C5Database.h"
#include "config.h"
#include "ui_wdbpartner.h"
#include <QStringListModel>
#include <QCompleter>

WdbPartner::WdbPartner(QWidget *parent) :
    WdbWidget(parent),
    ui(new Ui::WdbPartner)
{
    ui->setupUi(this);
    ui->wc->enumLineEdit();
    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
    db.exec("select fname from partners");
    while (db.nextRow()) {
        fStrings.insert(db.getString(0));
    }
    QStringListModel *m = new QStringListModel(fStrings.values());
    QCompleter *c = new QCompleter(m);
    c->setCaseSensitivity(Qt::CaseInsensitive);
    ui->leName->setCompleter(c);
}

WdbPartner::~WdbPartner()
{
    delete ui;
}

QString WdbPartner::tableName() const
{
    return "partners";
}
