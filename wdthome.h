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

    void on_btnFuel_clicked();

    void on_btnAllTickets2_clicked();

    void on_btnAllTickets2_2_clicked();

    void on_btnPartnersTicket2_clicked();

    void on_btnPartnersTicket3_clicked();

    void on_btnMoney1_clicked();

    void on_btnMoney2_clicked();

private:
    Ui::WdtHome *ui;
};

#endif // WDTHOME_H
