#include "filteramount2.h"
#include "ui_filteramount2.h"

FilterAmount2::FilterAmount2(QWidget *parent) :
    C5FilterWidget(parent),
    ui(new Ui::FilterAmount2)
{
    ui->setupUi(this);
    ui->wc->enumLineEdit();
}

FilterAmount2::~FilterAmount2()
{
    delete ui;
}

QString FilterAmount2::condition()
{
    QString w = " where c.fid>0 ";
    if (!ui->leFuel->isEmpty()) {
        w += " and ct.ffuel in (" + ui->leFuel->text() + ") ";
    }
    if (!ui->leTicket->isEmpty()) {
        w += " and c.ftype in (" + ui->leTicket->text() + ") ";
    }
    if (!ui->lePartner->isEmpty()) {
        w += " and c.fpartner in (" + ui->lePartner->text() + ") ";
    }
    if (!ui->deDate->isEmpty()) {
        w += " and year(c.fissuedate) in (" + ui->deDate->text() + ") ";
    }
    return w;
}

QString FilterAmount2::conditionText()
{
    QString w;
    if (!ui->leFuel->isEmpty()) {
        w += QString("%1: %2").arg(tr("Fuel")).arg(ui->leFuelName->text());
    }
    if (!ui->leTicket->isEmpty()) {
        w += QString("%1: %2").arg(tr("Ticket")).arg(ui->leTicketName->text());
    }
    if (!ui->lePartner->isEmpty()) {
        w += QString("%1: %2").arg(tr("Partner")).arg(ui->lePartnerName->text());
    }
    if (!ui->deDate->isEmpty()) {
        w += QString("%1: %2").arg(tr("Date")).arg(ui->deDate->text());
    }
    return w;
}

QString FilterAmount2::filterString()
{
    QString f;
    concat(ui->deDate, nullptr, tr("Year"), f);
    concat(ui->leTicket, ui->leTicketName, tr("Ticket"), f);
    concat(ui->leFuel, ui->leFuelName, tr("Fuel"), f);
    concat(ui->lePartner, ui->lePartnerName, tr("Partner"), f);
    return f;
}

void FilterAmount2::setFuel(const QString &v)
{
    ui->wc->setKey(ui->leFuel, v);
    ui->wc->setKey(ui->leTicket, "");
}

void FilterAmount2::setTicket(const QString &v)
{
    ui->wc->setKey(ui->leFuel, "");
    ui->wc->setKey(ui->leTicket, v);
}
