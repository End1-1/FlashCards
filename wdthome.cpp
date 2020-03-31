#include "wdthome.h"
#include "ui_wdthome.h"
#include "wdtcardtypes.h"
#include "wdtpartners.h"
#include "wdtalltickets.h"
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
