#ifndef LOGIN_H
#define LOGIN_H

#include "dialog.h"

namespace Ui {
class Login;
}

class Login : public Dialog
{
    Q_OBJECT

public:
    explicit Login();
    ~Login();
    static bool login();

private slots:
    void on_btnCancel_clicked();

    void on_btnOK_clicked();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
