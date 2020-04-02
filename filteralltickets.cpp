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
    return where;
}
