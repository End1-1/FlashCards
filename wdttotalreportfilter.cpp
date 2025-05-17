#include "wdttotalreportfilter.h"
#include "ui_wdttotalreportfilter.h"

WdtTotalReportFilter::WdtTotalReportFilter(QWidget *parent) :
    C5FilterWidget(parent),
    ui(new Ui::WdtTotalReportFilter)
{
    ui->setupUi(this);
}

WdtTotalReportFilter::~WdtTotalReportFilter()
{
    delete ui;
}

QString WdtTotalReportFilter::condition()
{
    return "";
}

QString WdtTotalReportFilter::conditionText()
{
    return "";
}

QString WdtTotalReportFilter::filterString()
{
    return "";
}

QDate WdtTotalReportFilter::d1()
{
    return ui->leStart->date();
}

QDate WdtTotalReportFilter::d2()
{
    return ui->leEnd->date();
}
