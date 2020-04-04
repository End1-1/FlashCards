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
    if (!ui->leState->isEmpty()) {
        where += " and c.fstate in (" + ui->leState->text() + ") ";
    }
    if (!ui->leFuel->isEmpty()) {
        where += " and ct.ffuel in (" + ui->leFuel->text() + ") ";
    }
    if (!ui->leTicket->isEmpty()) {
        where += " and ct.fid in (" + ui->leTicket->text() + ") ";
    }
    return where;
}

void FilterAllTickets::setFuelFilter(const QString &v)
{
    ui->wc->setKey(ui->leFuel, v);
    ui->wc->setKey(ui->leTicket, "");
    ui->wc->setKey(ui->leState, "");
}

void FilterAllTickets::setTicketFilter(const QString &v)
{
    ui->wc->setKey(ui->leFuel, "");
    ui->wc->setKey(ui->leTicket, v);
    ui->wc->setKey(ui->leState, "");
}

void FilterAllTickets::setStateFilter(const QString &v)
{
    ui->wc->setKey(ui->leFuel, "");
    ui->wc->setKey(ui->leTicket, "");
    ui->wc->setKey(ui->leState, v);
}
