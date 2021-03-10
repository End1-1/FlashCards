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
    QString w = " where ";
    w += QString(" c.fdate <= %1 ").arg(ui->leDate->toMySQLDate());
    return w;
}
