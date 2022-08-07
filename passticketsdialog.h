#ifndef PASSTICKETSDIALOG_H
#define PASSTICKETSDIALOG_H

#include "dialog.h"
#include "c5database.h"

namespace Ui {
class PassTicketsDialog;
}

class PassTicketsDialog : public Dialog
{
    Q_OBJECT

public:
    explicit PassTicketsDialog();
    explicit PassTicketsDialog(int id);
    ~PassTicketsDialog();

private slots:
    void on_btnCancel_clicked();
    void on_leStartCode_returnPressed();
    void on_leEndCode_returnPressed();
    void on_btnSave_clicked();
    void on_leCount_returnPressed();
    void on_leQty_returnPressed();
    void on_btnRollback_clicked();
    void on_lePrice_textEdited(const QString &arg1);
    void on_btnAddRow_clicked();
    void on_btnRemoveRow_clicked();
    void on_btnGroups_clicked();
    void on_leTicketName_textChanged(const QString &arg1);
    void on_leDiscount_textChanged(const QString &arg1);
    void on_btnPrint_clicked();
    void on_lePrice_textChanged(const QString &arg1);

private:
    Ui::PassTicketsDialog *ui;
    QString fTransaction;
    int fTotalCount;
    void getListOfTickets(C5Database &db, QString &err, bool process = false);
    void countTotal();
    void setPrice();
    void openDoc(int id);
};

#endif // PASSTICKETSDIALOG_H
