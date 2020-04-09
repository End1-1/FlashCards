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
