#include "wdbpartner.h"
#include "ui_wdbpartner.h"

WdbPartner::WdbPartner(QWidget *parent) :
    WdbWidget(parent),
    ui(new Ui::WdbPartner)
{
    ui->setupUi(this);
}

WdbPartner::~WdbPartner()
{
    delete ui;
}

QString WdbPartner::tableName() const
{
    return "partners";
}
