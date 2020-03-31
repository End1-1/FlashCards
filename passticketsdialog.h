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

private:
    Ui::PassTicketsDialog *ui;
    void getListOfTickets(QString &err, bool process = false);
};

#endif // PASSTICKETSDIALOG_H
