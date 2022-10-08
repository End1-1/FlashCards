#include "wdtamount3filter.h"
#include "ui_wdtamount3filter.h"

WdtAmount3Filter::WdtAmount3Filter(QWidget *parent) :
    C5FilterWidget(parent),
    ui(new Ui::WdtAmount3Filter)
{
    ui->setupUi(this);
}

WdtAmount3Filter::~WdtAmount3Filter()
{
    delete ui;
}


QString WdtAmount3Filter::condition()
{
    return "";
}

QString WdtAmount3Filter::conditionText()
{
    return QString("%1: %2 - %3").arg(tr("Date range"), ui->deDate1->text(), ui->deDate2->text());
}

QString WdtAmount3Filter::filterString()
{
    return "";
}

QMap<QString, QString> WdtAmount3Filter::replaces()
{
    fReplaceData["%date1%"] = ui->deDate1->toMySQLDate();
    fReplaceData["%date2%"] = ui->deDate2->toMySQLDate();
    return fReplaceData;
}
