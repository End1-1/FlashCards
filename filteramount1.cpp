#include "filteramount1.h"
#include "ui_filteramount1.h"

FilterAmount1::FilterAmount1(QWidget *parent) :
    C5FilterWidget(parent),
    ui(new Ui::FilterAmount1)
{
    ui->setupUi(this);
    ui->wc->enumLineEdit();
}

FilterAmount1::~FilterAmount1()
{
    delete ui;
}

QString FilterAmount1::condition()
{
    QString w = " where c.fid>0 ";
    if (!ui->leFuel->isEmpty()) {
        w += " and ct.ffuel in (" + ui->leFuel->text() + ") ";
    }
    if (!ui->deDate->isEmpty()) {
        w += " and year(c.fissuedate) in (" + ui->deDate->text() + ") ";
    }
    return w;
}

void FilterAmount1::setFuel(const QString &v)
{
    ui->wc->setKey(ui->leFuel, v);
}
