#include "filterticketsbyone.h"
#include "ui_filterticketsbyone.h"

FilterTicketsByOne::FilterTicketsByOne(QWidget *parent) :
    C5FilterWidget(parent),
    ui(new Ui::FilterTicketsByOne)
{
    ui->setupUi(this);
    ui->wc->enumLineEdit();
    fMode = 0;
}

FilterTicketsByOne::~FilterTicketsByOne()
{
    delete ui;
}

QString FilterTicketsByOne::condition()
{
    Q_ASSERT(fMode != 0);
    QString w = " where  ";
    C5DateEdit *d1, *d2;
    switch (fMode) {
    case 1:
        d1 = ui->deIssue1;
        d2 = ui->deIssue2;
        w += " c.fissuedate ";
        break;
    case 2:
        d1 = ui->deSold1;
        d2 = ui->deSold2;
        w += " c.fpartnerdate ";
        break;
    case 3:
        d1 = ui->deBack1;
        d2 = ui->deBack2;
        w += " c.frefunddate ";
        break;
    }
    w += QString(" between %1 and %2").arg(d1->toMySQLDate()).arg(d2->toMySQLDate());
    if (!ui->leFuel->isEmpty()) {
        w += " and ct.ffuel in (" + ui->leFuel->text() + ") ";
    }
    if (!ui->leTicket->isEmpty()) {
        w += " and c.ftype in (" + ui->leTicket->text() + ") ";
    }
    if (!ui->lePartner->isEmpty()) {
        w += " and c.fpartner in (" + ui->lePartner->text() + ") ";
    }
    if (!ui->leState->isEmpty()) {
        w += " and c.fstate in (" + ui->leState->text() + ") ";
    }
    if (!ui->lePayment->isEmpty()) {
        w += " and c.fpaymenttype in (" + ui->lePayment->text() + ") ";
    }
    return w;
}

void FilterTicketsByOne::setDateMode(int mode)
{
    fMode = mode;
    ui->lbIssue->setVisible(mode == 1);
    ui->deIssue1->setVisible(mode == 1);
    ui->deIssue2->setVisible(mode == 1);
    ui->lbSold->setVisible(mode == 2);
    ui->deSold1->setVisible(mode == 2);
    ui->deSold2->setVisible(mode == 2);
    ui->lbBack->setVisible(mode == 3);
    ui->deBack1->setVisible(mode == 3);
    ui->deBack2->setVisible(mode == 3);
    adjustSize();
}
