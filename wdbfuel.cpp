#include "wdbfuel.h"
#include "ui_wdbfuel.h"

WdbFuel::WdbFuel(QWidget *parent) :
    WdbWidget(parent),
    ui(new Ui::WdbFuel)
{
    ui->setupUi(this);
    ui->wc->enumLineEdit();
}

WdbFuel::~WdbFuel()
{
    delete ui;
}

QString WdbFuel::tableName() const
{
    return "fuel";
}
