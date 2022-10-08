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
    QString w = " where c.fid is not null ";
    if (!ui->leFuel->isEmpty()) {
        w += " and ct.ffuel in (" + ui->leFuel->text() + ") ";
    }
    return w;
}

QString FilterAmount1::conditionText()
{
    QString where;
    if (!ui->leFuel->isEmpty()) {
        where += QString("%1: %2").arg(tr("Fuel"), ui->leFuelName->text());
    }
    where += QString("%1: %2").arg(tr("Date"), ui->deDate->text());
    return where;
}

QString FilterAmount1::filterString()
{
    QString f;
    concat(ui->deDate, nullptr, tr("Date"), f);
    concat(ui->leFuel, ui->leFuelName, tr("Fuel"), f);
    return f;
}

void FilterAmount1::setFuel(const QString &v)
{
    ui->wc->setKey(ui->leFuel, v);
}

QMap<QString, QString> FilterAmount1::replaces()
{
    fReplaceData["%date%"] = ui->deDate->toMySQLDate();
    return fReplaceData;
}
