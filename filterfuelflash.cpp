#include "filterfuelflash.h"
#include "ui_filterfuelflash.h"

FilterFuelFlash::FilterFuelFlash(QWidget *parent) :
    C5FilterWidget(parent),
    ui(new Ui::FilterFuelFlash)
{
    ui->setupUi(this);
    ui->wc->enumLineEdit();
}

FilterFuelFlash::~FilterFuelFlash()
{
    delete ui;
}

QString FilterFuelFlash::condition()
{
    QString w = " where  ";
    w += QString(" c.fdate between %1 and %2").arg(ui->leDate1->toMySQLDate(), ui->leDate2->toMySQLDate());
    if (!ui->leFuel->isEmpty()) {
        w += " and ct.ffuel in (" + ui->leFuel->text() + ") ";
    }
    if (!ui->lePartner->isEmpty()) {
        w += " and c.fpartner in (" + ui->lePartner->text() + ") ";
    }
    if (!ui->leState->isEmpty()) {
        w += " and c.fstate in (" + ui->leState->text() + ") ";
    }
    if (!ui->lePayment->isEmpty()) {
        w += " and c.fpaymenttype in (" + ui->lePayment->text() + ") ";
    }
    return w;
}

QString FilterFuelFlash::conditionText()
{
    QString w = QString("%1 %2-%3").arg(tr("Date"), ui->leDate1->text(), ui->leDate2->text());
    if (!ui->leFuel->isEmpty()) {
        w += QString("%1: %2 ").arg(tr("Fuel"), ui->leFuelName->text());
    }
    if (!ui->lePartner->isEmpty()) {
        w += QString("%1: %2 ").arg(tr("Partner"), ui->lePartnerName->text());
    }
    if (!ui->leState->isEmpty()) {
        w += QString("%1: %2 ").arg(tr("State"), ui->leStateName->text());
    }
    if (!ui->lePayment->isEmpty()) {
        w += QString("%1: %2 ").arg(tr("Payment"), ui->lePaymentType->text());
    }
    return w;
}

QString FilterFuelFlash::filterString()
{
    QString f;
    concat(ui->leDate1, nullptr, tr("Date from"), f);
    concat(ui->leDate2, nullptr, tr("Date to"), f);
    concat(ui->lePartner, ui->lePartnerName, tr("Partner"), f);
    concat(ui->leFuel, ui->leFuelName, tr("Fuel"), f);
    concat(ui->leState, ui->leStateName, tr("State"), f);
    concat(ui->lePayment, ui->lePaymentType, tr("Payment type"), f);
    return f;
}
