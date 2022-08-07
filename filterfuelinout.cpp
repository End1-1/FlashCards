#include "filterfuelinout.h"
#include "ui_filterfuelinout.h"

FilterFuelInOut::FilterFuelInOut(QWidget *parent) :
    C5FilterWidget(parent),
    ui(new Ui::FilterFuelInOut)
{
    ui->setupUi(this);
}

FilterFuelInOut::~FilterFuelInOut()
{
    delete ui;
}

QString FilterFuelInOut::condition()
{
    QString w;
    w += QString(" and d.fdatecreate between %1 and %2").arg(ui->leDate1->toMySQLDate(), ui->leDate2->toMySQLDate());
    return w;
}

QString FilterFuelInOut::conditionText()
{
    QString w = QString("%1 %2-%3").arg(tr("Date"), ui->leDate1->text(), ui->leDate2->text());
    return w;
}

QString FilterFuelInOut::filterString()
{
    QString f;
    concat(ui->leDate1, nullptr, tr("Date from"), f);
    concat(ui->leDate2, nullptr, tr("Date to"), f);
    return f;
}
