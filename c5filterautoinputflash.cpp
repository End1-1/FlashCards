#include "c5filterautoinputflash.h"
#include "ui_c5filterautoinputflash.h"

C5FilterAutoinputFlash::C5FilterAutoinputFlash(QWidget *parent) :
    C5FilterWidget(parent),
    ui(new Ui::C5FilterAutoinputFlash)
{
    ui->setupUi(this);
    ui->wc->enumLineEdit();
}

C5FilterAutoinputFlash::~C5FilterAutoinputFlash()
{
    delete ui;
}

QString C5FilterAutoinputFlash::condition()
{
    QString w = " where  ";
    w += QString(" d.fdate between %1 and %2").arg(ui->leDate1->toMySQLDate()).arg(ui->leDate2->toMySQLDate());
    if (!ui->leStation->isEmpty()) {
        w += " and d.fstation in (" + ui->leStation->text() + ") ";
    }
    if (!ui->leState->isEmpty()) {
        w += " and d.fstate in (" + ui->leState->text() + ") ";
    }
    return w;
}

QString C5FilterAutoinputFlash::conditionText()
{
    QString w = QString("%1: %2-%3").arg(tr("Date")).arg(ui->leDate1->text()).arg(ui->leDate2->text());
    if (!ui->leStation->isEmpty()) {
        w += QString("%1: %2 ").arg(tr("Station")).arg(ui->leStation->text());
    }
    if (!ui->leState->isEmpty()) {
        w += QString("%1: %2 ").arg(tr("State")).arg(ui->leState->text());
    }
    return w;
}

QString C5FilterAutoinputFlash::filterString()
{
    QString f;
    concat(ui->leDate1, nullptr, tr("Date from"), f);
    concat(ui->leDate2, nullptr, tr("Date to"), f);
    concat(ui->leStation, ui->leStationName, tr("Station"), f);
    concat(ui->leState, ui->leStateName, tr("State"), f);
    return f;
}
