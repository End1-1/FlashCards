#include "filterdebtshistory.h"
#include "ui_filterdebtshistory.h"

FilterDebtsHistory::FilterDebtsHistory(QWidget *parent) :
    C5FilterWidget(parent),
    ui(new Ui::FilterDebtsHistory)
{
    ui->setupUi(this);
    ui->wc->enumLineEdit();
}

FilterDebtsHistory::~FilterDebtsHistory()
{
    delete ui;
}

QString FilterDebtsHistory::condition()
{
    QString w = " where  ";
    w += QString(" dc.fdatecreate between %1 and %2").arg(ui->leDate1->toMySQLDate(), ui->leDate2->toMySQLDate());
    if (!ui->lePartner->isEmpty()) {
        w += " and d.fpartner in (" + ui->lePartner->text() + ") ";
    }
    if (!ui->lePayment->isEmpty()) {
        w += " and dc.fpaymenttype in (" + ui->lePayment->text() + ") ";
    }
    return w;
}

QString FilterDebtsHistory::conditionText()
{
    QString w = QString("%1 %2-%3").arg(tr("Date"), ui->leDate1->text(), ui->leDate2->text());
    if (!ui->lePartner->isEmpty()) {
        w += QString("%1: %2 ").arg(tr("Partner"), ui->lePartnerName->text());
    }
    if (!ui->lePayment->isEmpty()) {
        w += QString("%1: %2 ").arg(tr("Payment"), ui->lePaymentType->text());
    }
    return w;
}

QString FilterDebtsHistory::filterString()
{
    QString f;
    concat(ui->leDate1, nullptr, tr("Date from"), f);
    concat(ui->leDate2, nullptr, tr("Date to"), f);
    concat(ui->lePartner, ui->lePartnerName, tr("Partner"), f);
    concat(ui->lePartnerName, ui->lePaymentType, tr("Payment type"), f);
    return f;
}

void FilterDebtsHistory::setFilter(const QDate &d1, const QDate &d2, int partner)
{
    ui->leDate1->setDate(d1);
    ui->leDate2->setDate(d2);
    ui->lePartner->setInteger(partner);
}
