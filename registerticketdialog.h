#ifndef REGISTERTICKETDIALOG_H
#define REGISTERTICKETDIALOG_H

#include "dialog.h"

namespace Ui {
class RegisterTicketDialog;
}

class RegisterTicketDialog : public Dialog
{
    Q_OBJECT

public:
    explicit RegisterTicketDialog();
    ~RegisterTicketDialog();

private slots:
    void on_btnRegister_clicked();
    void on_btnCancel_clicked();
    void on_leNumberStart_textEdited(const QString &arg1);
    void on_leNumberEnd_textEdited(const QString &arg1);

private:
    Ui::RegisterTicketDialog *ui;
};

#endif // REGISTERTICKETDIALOG_H
