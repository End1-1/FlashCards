#ifndef PASSTICKETSDIALOG_H
#define PASSTICKETSDIALOG_H

#include "dialog.h"

namespace Ui {
class PassTicketsDialog;
}

class PassTicketsDialog : public Dialog
{
    Q_OBJECT

public:
    explicit PassTicketsDialog();
    ~PassTicketsDialog();

private slots:
    void on_btnCancel_clicked();
    void on_leStartCode_returnPressed();
    void on_leEndCode_returnPressed();
    void on_btnSave_clicked();
    void on_leCount_returnPressed();
    void on_leQty_returnPressed();

    void on_btnRollback_clicked();

private:
    Ui::PassTicketsDialog *ui;
    QString fTransaction;
    void getListOfTickets(QString &err, bool process = false);
};

#endif // PASSTICKETSDIALOG_H
