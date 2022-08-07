#include "filterpartner2.h"
#include "ui_filterpartner2.h"

FilterPartner2::FilterPartner2(QWidget *parent) :
    C5FilterWidget(parent),
    ui(new Ui::FilterPartner2)
{
    ui->setupUi(this);
    ui->wc->enumLineEdit();
}

FilterPartner2::~FilterPartner2()
{
    delete ui;
}

QString FilterPartner2::condition()
{
    QString w = " where s.fid>0 ";
    if (!ui->leFuel->isEmpty()) {
        w += " and ct.ffuel in (" + ui->leFuel->text() + ") ";
    }
    if (!ui->lePartner->isEmpty()) {
        w += " and s.fpartner in (" + ui->lePartner->text() + ") ";
    }
    return w;
}

QString FilterPartner2::conditionText()
{
    QString w;
    if (!ui->leFuel->isEmpty()) {
        w += QString("%1: %2 ").arg(tr("Fuel")).arg(ui->leFuelName->text());
    }
    if (!ui->lePartner->isEmpty()) {
        w += QString("%1: %2 ").arg(tr("Partner")).arg(ui->lePartnerName->text());
    }
    return w;
}

QString FilterPartner2::filterString()
{
    QString f;
    concat(ui->leFuel, ui->leFuelName, tr("Fuel"), f);
    concat(ui->lePartner, ui->lePartnerName, tr("Partner"), f);
    return f;
}

void FilterPartner2::setFuelFilter(const QString &v)
{
    ui->wc->setKey(ui->leFuel, v);
}
