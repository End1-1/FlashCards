#include "wdtrangemovementfilter.h"
#include "ui_wdtrangemovementfilter.h"

WdtRangeMovementFilter::WdtRangeMovementFilter(QWidget *parent) :
    C5FilterWidget(parent),
    ui(new Ui::WdtRangeMovementFilter)
{
    ui->setupUi(this);
}

WdtRangeMovementFilter::~WdtRangeMovementFilter()
{
    delete ui;
}

QString WdtRangeMovementFilter::condition()
{
    return QString("WHERE c.fpartnerdate BETWEEN %1 AND %2 ")
            .arg(ui->leStart->toMySQLDate(), ui->leEnd->toMySQLDate());
}

QString WdtRangeMovementFilter::conditionText()
{
    return QString("%1: %2 - %3").arg(tr("Date range"), ui->leStart->text(), ui->leEnd->text());
}

QString WdtRangeMovementFilter::filterString()
{
    return "";
}

QMap<QString, QString> WdtRangeMovementFilter::replaces()
{
    QMap<QString,QString> m = {{"%date1%", ui->leStart->toMySQLDate()}, {"%date2%", ui->leEnd->toMySQLDate()}};
    return m;
}

QDate WdtRangeMovementFilter::date1() const
{
    return ui->leStart->date();
}

QDate WdtRangeMovementFilter::date2() const
{
    return ui->leEnd->date();
}

int WdtRangeMovementFilter::mode()
{
    return ui->cbMode->currentIndex();
}
