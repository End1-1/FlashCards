#include "filterflashfuelstore.h"
#include "ui_filterflashfuelstore.h"

FilterFlashFuelStore::FilterFlashFuelStore(QWidget *parent) :
    C5FilterWidget(parent),
    ui(new Ui::FilterFlashFuelStore)
{
    ui->setupUi(this);
    ui->wc->enumLineEdit();
}

FilterFlashFuelStore::~FilterFlashFuelStore()
{
    delete ui;
}

QString FilterFlashFuelStore::condition()
{
    QString w = " where c.fstate in (1,2) ";
    w += QString(" and c.fdate <= %1 ").arg(ui->leDate->toMySQLDate());
    return w;
}

QString FilterFlashFuelStore::conditionText()
{
    return QString("%1: %2 ").arg(tr("Date"), ui->leDate->text());
}

QString FilterFlashFuelStore::filterString()
{
    QString f;
    concat(ui->leDate, nullptr, tr("Date"), f);
    return f;
}
