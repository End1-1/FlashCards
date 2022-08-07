#include "filterdebts.h"
#include "ui_filterdebts.h"

FilterDebts::FilterDebts(QWidget *parent) :
    C5FilterWidget(parent),
    ui(new Ui::FilterDebts)
{
    ui->setupUi(this);
    ui->wc->enumLineEdit();
}

FilterDebts::~FilterDebts()
{
    delete ui;
}

QString FilterDebts::condition()
{
    return "";
}

QString FilterDebts::conditionText()
{
    QString w = QString("%1 %2-%3").arg(tr("Date"), ui->leDate1->text(), ui->leDate2->text());
    return w;
}

QString FilterDebts::filterString()
{
    QString f;
    concat(ui->leDate1, nullptr, tr("Date from"), f);
    concat(ui->leDate2, nullptr, tr("Date to"), f);
    return f;
}

QString FilterDebts::c1() const
{
    QString w = QString(" where t.fdate < %1").arg(ui->leDate1->toMySQLDate());
    return w;
}

QString FilterDebts::c2() const
{
    QString w =  QString(" where d.fdate between %1 and %2").arg(ui->leDate1->toMySQLDate(), ui->leDate2->toMySQLDate());
    return w;
}
