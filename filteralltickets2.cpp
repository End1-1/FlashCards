#include "filteralltickets2.h"
#include "ui_filteralltickets2.h"

FilterAllTickets2::FilterAllTickets2(QWidget *parent) :
    C5FilterWidget(parent),
    ui(new Ui::FilterAllTickets2)
{
    ui->setupUi(this);
    ui->wc->enumLineEdit();
}

FilterAllTickets2::~FilterAllTickets2()
{
    delete ui;
}

QString FilterAllTickets2::condition()
{
    QString where = "where ct.fid>0 ";
    if (!ui->leFuel->isEmpty()) {
        where += " and ct.ffuel in (" + ui->leFuel->text() + ") ";
    }
    return where;
}

void FilterAllTickets2::setFuelFilter(const QString &v)
{
    ui->wc->setKey(ui->leFuel, v);
}
