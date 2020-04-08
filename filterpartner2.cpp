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

void FilterPartner2::setFuelFilter(const QString &v)
{
    ui->wc->setKey(ui->leFuel, v);
}
