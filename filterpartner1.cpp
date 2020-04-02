#include "filterpartner1.h"
#include "ui_filterpartner1.h"

FilterPartner1::FilterPartner1(QWidget *parent) :
    C5FilterWidget(parent),
    ui(new Ui::FilterPartner1)
{
    ui->setupUi(this);
    ui->wc->enumLineEdit();
}

FilterPartner1::~FilterPartner1()
{
    delete ui;
}

QString FilterPartner1::condition()
{
    QString w = " where s.fid>0 ";
    if (!ui->leFuel->isEmpty()) {
        w += " and ct.ffuel in (" + ui->leFuel->text() + ") ";
    }
    if (!ui->leTicket->isEmpty()) {
        w += " and s.ftype in (" + ui->leTicket->text() + ") ";
    }
    if (!ui->lePartner->isEmpty()) {
        w += " and s.fpartner in (" + ui->lePartner->text() + ") ";
    }
    return w;
}

void FilterPartner1::setTicketFilter(const QString &v)
{
    ui->wc->setKey(ui->leTicket, v);
}
