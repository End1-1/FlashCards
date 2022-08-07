#ifndef DLGDEBTPAYMENT_H
#define DLGDEBTPAYMENT_H

#include "dialog.h"

namespace Ui {
class DlgDebtPayment;
}

class DlgDebtPayment : public Dialog
{
    Q_OBJECT

public:
    explicit DlgDebtPayment(QWidget *parent = nullptr);
    ~DlgDebtPayment();

private slots:
    void on_btnCancel_clicked();

    void on_btnSave_clicked();

private:
    Ui::DlgDebtPayment *ui;
    QString fTransaction;
    int fDocNumber;
};

#endif // DLGDEBTPAYMENT_H
