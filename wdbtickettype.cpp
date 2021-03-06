#include "wdbtickettype.h"
#include "ui_wdbtickettype.h"

WdbTicketType::WdbTicketType(QWidget *parent) :
    WdbWidget(parent),
    ui(new Ui::WdbTicketType)
{
    ui->setupUi(this);
    ui->wc->enumLineEdit();
}

WdbTicketType::~WdbTicketType()
{
    delete ui;
}

QString WdbTicketType::tableName() const
{
    return "cards_types";
}
