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
#include "wdtfuelflashmove.h"
#include "wdtflashfuelstore.h"
#include "fuelflashmove.h"
#include "wdtfueloutin.h"
#include "config.h"
#include "autoinputdoc.h"
#include "registerticketdialog.h"
#include "passticketsdialog.h"
#include "registeruseddialog.h"
#include "wdtdebts.h"
#include "wdtflashautoinput.h"
#include "wdtfuelprices.h"
#include "wdtsaledocs.h"
#include "wdtamount3.h"
#include "wdtdebtshistory.h"

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

void WdtHome::on_btnOutputFlash_clicked()
{
    FuelFlashMove ffm;
    ffm.setMove(2);
    ffm.exec();
}

void WdtHome::on_btnFlashFuelMove_clicked()
{
    auto *t = __parent->addWidget<WdtFuelFlashMove>(static_cast<QToolButton*>(sender()));
}

void WdtHome::on_btnFlashFuelStore_clicked()
{
    auto *t = __parent->addWidget<WdtFlashFuelStore>(static_cast<QToolButton*>(sender()));
}

void WdtHome::on_btnFlashAutoinput_clicked()
{
    auto *t = __parent->addWidget<WdtFlashAutoinput>(static_cast<QToolButton*>(sender()));
}

void WdtHome::on_btnPrices_clicked()
{
    auto *t = __parent->addWidget<WdtFuelPrices>(static_cast<QToolButton*>(sender()));
}

void WdtHome::on_btnSalesByPayment_clicked()
{
    auto *t = __parent->addWidget<WdtSaleDocs>(static_cast<QToolButton*>(sender()));
}

void WdtHome::on_btnFuelInOut_clicked()
{
    auto *t = __parent->addWidget<WdtFuelOutIn>(static_cast<QToolButton*>(sender()));
}

void WdtHome::on_btnFuelInOut_2_clicked()
{
    auto *t = __parent->addWidget<WdtDebts>(static_cast<QToolButton*>(sender()));
}

void WdtHome::on_btnDebtsHistory_clicked()
{
    auto *t = __parent->addWidget<WdtDebtsHistory>(static_cast<QToolButton*>(sender()));
}

void WdtHome::on_btnInputFuel_clicked()
{
    AutoinputDoc().newDoc().exec();
}

void WdtHome::on_btnMoney3_clicked()
{
    __parent->addWidget<WdtAmount3>(static_cast<QToolButton *>(sender()));
}
