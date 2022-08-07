#ifndef REGISTERUSEDDIALOG_H
#define REGISTERUSEDDIALOG_H

#include "dialog.h"

namespace Ui {
class RegisterUsedDialog;
}

class RegisterUsedDialog : public Dialog
{
    Q_OBJECT

public:
    explicit RegisterUsedDialog();
    ~RegisterUsedDialog();

private slots:
    void on_btnClose_clicked();
    void on_btnWrite_clicked();
    void on_leCode_returnPressed();
    void on_chBack_clicked(bool checked);
    void on_btnGo_clicked();
    void on_comboBox_currentIndexChanged(int index);
    void on_deUsed_editingFinished();
    void on_btnPrint_clicked();
    void on_btnZeroArmPetStore_clicked();

private:
    Ui::RegisterUsedDialog *ui;
    void getTicketByCode();
    int fTicketId;
    QString fTransaction;
    double fuelPrice(int fuel);
};

#endif // REGISTERUSEDDIALOG_H
