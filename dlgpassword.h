#ifndef DLGPASSWORD_H
#define DLGPASSWORD_H

#include <QDialog>

namespace Ui {
class DlgPassword;
}

class DlgPassword : public QDialog
{
    Q_OBJECT

public:
    explicit DlgPassword(QWidget *parent = nullptr);
    ~DlgPassword();

private slots:
    void on_btnChange_clicked();

    void on_btnCancel_clicked();

private:
    Ui::DlgPassword *ui;
};

#endif // DLGPASSWORD_H
