#include "filteralltickets.h"
#include "ui_filteralltickets.h"

FilterAllTickets::FilterAllTickets(QWidget *parent) :
    C5FilterWidget(parent),
    ui(new Ui::FilterAllTickets)
{
    ui->setupUi(this);
    ui->wc->enumLineEdit();
}

FilterAllTickets::~FilterAllTickets()
{
    delete ui;
}

QString FilterAllTickets::condition()
{
    QString where = "where c.fid>0 ";
    if (!ui->leFuel->isEmpty()) {
        where += " and ct.ffuel in (" + ui->leFuel->text() + ") ";
    }
    if (!ui->leTicket->isEmpty()) {
        where += " and ct.fid in (" + ui->leTicket->text() + ") ";
    }
    if (!ui->leYear->isEmpty()) {
        where += " and year(c.fissuedate) in (" + ui->leYear->text() + ") ";
    }
    return where;
}

QString FilterAllTickets::conditionText()
{
    QString where;
    if (!ui->leFuel->isEmpty()) {
        where += QString("%1: %2").arg(tr("Fuel")).arg(ui->leFuelName->text());
    }
    if (!ui->leTicket->isEmpty()) {
        where += QString("%1: %2").arg(tr("Ticket")).arg(ui->leTicketName->text());
    }
    if (!ui->leYear->isEmpty()) {
        where += QString("%1: %2").arg(tr("Year")).arg(ui->leYear->text());
    }
    return where;
}

QString FilterAllTickets::filterString()
{
    QString f;
    concat(ui->leFuel, ui->leFuelName, tr("Fuel"), f);
    concat(ui->leTicket, ui->leTicketName, tr("Ticket"), f);
    concat(ui->leYear, nullptr, tr("Year"), f);
    return f;
}

void FilterAllTickets::setFuelFilter(const QString &v)
{
    ui->wc->setKey(ui->leFuel, v);
    ui->wc->setKey(ui->leTicket, "");
    ui->leYear->clear();
}

void FilterAllTickets::setTicketFilter(const QString &v)
{
    ui->wc->setKey(ui->leFuel, "");
    ui->wc->setKey(ui->leTicket, v);
    ui->leYear->clear();
}
