#include "filterpartner1.h"
#include "ui_filterpartner1.h"

FilterPartner1::FilterPartner1(QWidget *parent) :
    C5FilterWidget(parent),
    ui(new Ui::FilterPartner1)
{
    ui->setupUi(this);
    ui->wc->enumLineEdit();
}

FilterPartner1::~FilterPartner1()
{
    delete ui;
}

QString FilterPartner1::condition()
{
    QString w = " where s.fid>0 ";
    if (!ui->leFuel->isEmpty()) {
        w += " and ct.ffuel in (" + ui->leFuel->text() + ") ";
    }
    if (!ui->leTicket->isEmpty()) {
        w += " and s.ftype in (" + ui->leTicket->text() + ") ";
    }
    if (!ui->lePartner->isEmpty()) {
        w += " and s.fpartner in (" + ui->lePartner->text() + ") ";
    }
    return w;
}

QString FilterPartner1::conditionText()
{
    QString w;
    if (!ui->leFuel->isEmpty()) {
        w += QString("%1: %2 ").arg(tr("Fuel")).arg(ui->leFuelName->text());
    }
    if (!ui->leTicket->isEmpty()) {
        w += QString("%1: %2 ").arg(tr("Ticket")).arg(ui->leTicketName->text());
    }
    if (!ui->lePartner->isEmpty()) {
        w += QString("%1: %2 ").arg(tr("Partner")).arg(ui->lePartnerName->text());
    }
    return w;
}

QString FilterPartner1::filterString()
{
    QString f;
    concat(ui->leFuel, ui->leFuelName, tr("Fuel"), f);
    concat(ui->lePartner, ui->lePartnerName, tr("Partner"), f);
    concat(ui->leTicket, ui->leTicketName, tr("Ticket"), f);
    return f;
}

void FilterPartner1::setTicketFilter(const QString &v)
{
    ui->wc->setKey(ui->leTicket, v);
}
