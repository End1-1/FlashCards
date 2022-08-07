#include "filterpartner3.h"
#include "ui_filterpartner3.h"

FilterPartner3::FilterPartner3(QWidget *parent) :
    C5FilterWidget(parent),
    ui(new Ui::FilterPartner3)
{
    ui->setupUi(this);
    ui->wc->enumLineEdit();
}

FilterPartner3::~FilterPartner3()
{
    delete ui;
}

QString FilterPartner3::condition()
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
    if (!ui->deIssue->isEmpty()) {
        w += " and year(s.fissuedate) in (" + ui->deIssue->text() + ") ";
    }
    return w;
}

QString FilterPartner3::conditionText()
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
    if (!ui->deIssue->isEmpty()) {
        w += QString("%1: %2 ").arg(tr("Issue")).arg(ui->deIssue->text());
    }
    return w;
}

QString FilterPartner3::filterString()
{
    QString f;
    concat(ui->leFuel, ui->leFuelName, tr("Fuel"), f);
    concat(ui->lePartner, ui->lePartnerName, tr("Partner"), f);
    concat(ui->leTicket, ui->leTicketName, tr("Ticket"), f);
    concat(ui->deIssue, nullptr, tr("Year"), f);
    return f;
}

void FilterPartner3::setFuelFilter(const QString &v)
{
    ui->wc->setKey(ui->leFuel, v);
    ui->wc->setKey(ui->leTicket, "");
}

void FilterPartner3::setTicketFilter(const QString &v)
{
    ui->wc->setKey(ui->leFuel, "");
    ui->wc->setKey(ui->leTicket, v);
}
