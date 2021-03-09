#include "wdthome.h"
#include "ui_wdthome.h"
#include "wdtcardtypes.h"
#include "wdtpartners.h"
#include "wdtalltickets.h"
#include "wdtalltickets2.h"
#include "wdtreppartners1.h"
#include "wdtreppartners2.h"
#include "wdtreppartners3.h"
#include "wdtamount1.h"
#include "wdtamount2.h"
#include "wdticketsbyone.h"
#include "wdtfuel.h"
#include "wdtfuelflash.h"
#include "fuelflashmove.h"
#include "config.h"
#include "registerticketdialog.h"
#include "passticketsdialog.h"
#include "registeruseddialog.h"

WdtHome::WdtHome(const QIcon &icon, const QString &label, QWidget *parent) :
    WdtWidget(icon, label, parent),
    ui(new Ui::WdtHome)
{
    ui->setupUi(this);
}

WdtHome::~WdtHome()
{
    delete ui;
}

void WdtHome::on_btnCardTypes_clicked()
{
    __parent->addWidget<WdtCardTypes>(static_cast<QToolButton *>(sender()));
}

void WdtHome::on_btnPartners_clicked()
{
    __parent->addWidget<WdtPartners>(static_cast<QToolButton *>(sender()));
}

void WdtHome::on_btnRegister_clicked()
{
    RegisterTicketDialog *d = new RegisterTicketDialog();
    d->exec();
    delete d;
}

void WdtHome::on_btnAllTickets_clicked()
{
    __parent->addWidget<WdtAllTickets>(static_cast<QToolButton *>(sender()));
}

void WdtHome::on_btnPassTickets_clicked()
{
    PassTicketsDialog *d = new PassTicketsDialog();
    d->exec();
    delete d;
}

void WdtHome::on_btnRegisterUsed_clicked()
{
    RegisterUsedDialog *d = new RegisterUsedDialog();
    d->exec();
    delete d;
}

void WdtHome::on_btnFuel_clicked()
{
    __parent->addWidget<WdtFuel>(static_cast<QToolButton *>(sender()));
}

void WdtHome::on_btnAllTickets2_clicked()
{
    __parent->addWidget<WdtAllTickets2>(static_cast<QToolButton *>(sender()));
}

void WdtHome::on_btnAllTickets2_2_clicked()
{
    __parent->addWidget<WdtRepPartners1>(static_cast<QToolButton *>(sender()));
}

void WdtHome::on_btnPartnersTicket2_clicked()
{
    __parent->addWidget<WdtRepPartners2>(static_cast<QToolButton *>(sender()));
}

void WdtHome::on_btnPartnersTicket3_clicked()
{
    __parent->addWidget<WdtRepPartners3>(static_cast<QToolButton *>(sender()));
}

void WdtHome::on_btnMoney1_clicked()
{
    __parent->addWidget<WdtAmount1>(static_cast<QToolButton *>(sender()));
}

void WdtHome::on_btnMoney2_clicked()
{
    __parent->addWidget<WdtAmount2>(static_cast<QToolButton *>(sender()));
}

void WdtHome::on_btnTicketsMoveIn_clicked()
{
    auto *t = __parent->addWidget<WdTicketsByOne>(static_cast<QToolButton *>(sender()));
    t->setFilterMode(1);
}

void WdtHome::on_btnTicketsMoveSale_clicked()
{
    auto *t = __parent->addWidget<WdTicketsByOne>(static_cast<QToolButton *>(sender()));
    t->setFilterMode(2);
}

void WdtHome::on_btnTicketsMoveBack_clicked()
{
    auto *t = __parent->addWidget<WdTicketsByOne>(static_cast<QToolButton *>(sender()));
    t->setFilterMode(3);
}

void WdtHome::on_btnFuelFlash_clicked()
{
    __parent->addWidget<WdtFuelFlash>(static_cast<QToolButton *>(sender()));
}

void WdtHome::on_btnFlashInput_clicked()
{
    FuelFlashMove ffm;
    ffm.exec();
}
