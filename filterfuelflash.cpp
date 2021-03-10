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
    w += QString(" c.fdate between %1 and %2").arg(ui->leDate1->toMySQLDate()).arg(ui->leDate2->toMySQLDate());
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
