#include "filterdocs.h"
#include "ui_filterdocs.h"

FilterDocs::FilterDocs(QWidget *parent) :
    C5FilterWidget(parent),
    ui(new Ui::FilterDocs)
{
    ui->setupUi(this);
    ui->wc->enumLineEdit();
}

FilterDocs::~FilterDocs()
{
    delete ui;
}

QString FilterDocs::condition()
{
    QString w = " where  ";
    w += QString(" d.fdatecreate between %1 and %2").arg(ui->leDate1->toMySQLDate(), ui->leDate2->toMySQLDate());
    if (!ui->lePartner->isEmpty()) {
        w += " and c.fpartner in (" + ui->lePartner->text() + ") ";
    }
    if (!ui->leState->isEmpty()) {
        w += " and d.fpaymentstate in (" + ui->leState->text() + ") ";
    }
    if (!ui->lePayment->isEmpty()) {
        w += " and d.fpaymenttype in (" + ui->lePayment->text() + ") ";
    }
    if (!ui->leDocType->isEmpty()) {
        w += " and d.fdoctype in (" + ui->leDocType->text() + ") ";
    }
    return w;
}

QString FilterDocs::conditionText()
{
    QString w = QString("%1 %2-%3").arg(tr("Date"), ui->leDate1->text(), ui->leDate2->text());
    if (!ui->lePartner->isEmpty()) {
        w += QString("%1: %2 ").arg(tr("Partner"), ui->lePartnerName->text());
    }
    if (!ui->leState->isEmpty()) {
        w += QString("%1: %2 ").arg(tr("State"), ui->leStateName->text());
    }
    if (!ui->lePayment->isEmpty()) {
        w += QString("%1: %2 ").arg(tr("Payment"), ui->lePaymentType->text());
    }
    if (!ui->leDocType->isEmpty()) {
        w += QString("%1: %2").arg(tr("Document type"), ui->leDocumentType->text());
    }
    return w;
}

QString FilterDocs::filterString()
{
    QString f;
    concat(ui->leDate1, nullptr, tr("Date from"), f);
    concat(ui->leDate2, nullptr, tr("Date to"), f);
    concat(ui->lePartner, ui->lePartnerName, tr("Partner"), f);
    concat(ui->leState, ui->leStateName, tr("Payment state"), f);
    concat(ui->lePartnerName, ui->lePaymentType, tr("Payment type"), f);
    concat(ui->leDocType, ui->leDocumentType, tr("Document type"), f);
    return f;
}
