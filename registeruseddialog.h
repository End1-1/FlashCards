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

private:
    Ui::RegisterUsedDialog *ui;
    void getTicketByCode();
    int fTicketId;
};

#endif // REGISTERUSEDDIALOG_H
