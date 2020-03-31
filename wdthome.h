#ifndef WDTHOME_H
#define WDTHOME_H

#include "wdtwidget.h"

namespace Ui {
class WdtHome;
}

class WdtHome : public WdtWidget
{
    Q_OBJECT

public:
    explicit WdtHome(const QIcon &icon, const QString &label, QWidget *parent = nullptr);
    ~WdtHome();

private slots:
    void on_btnCardTypes_clicked();

    void on_btnPartners_clicked();

    void on_btnRegister_clicked();

    void on_btnAllTickets_clicked();

    void on_btnPassTickets_clicked();

    void on_btnRegisterUsed_clicked();

private:
    Ui::WdtHome *ui;
};

#endif // WDTHOME_H
