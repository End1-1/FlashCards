#ifndef FUELFLASHMOVE_H
#define FUELFLASHMOVE_H

#include "dialog.h"

namespace Ui {
class FuelFlashMove;
}

class FuelFlashMove : public Dialog
{
    Q_OBJECT

public:
    explicit FuelFlashMove();
    ~FuelFlashMove();
    void setMove(int m);
    FuelFlashMove &setDoc(int id);

private slots:
    void on_btnCancel_clicked();
    void on_btnSave_clicked();
    void on_leQty_textEdited(const QString &arg1);
    void on_lePrice_textEdited(const QString &arg1);
    void on_leTotal_textEdited(const QString &arg1);
    void on_btnPrint_clicked();
    void on_leInitialPrice_textChanged(const QString &arg1);
    void on_leDiscount_textChanged(const QString &arg1);
    void on_btnAddRow_clicked();

    void on_btnRemoveRow_clicked();

private:
    Ui::FuelFlashMove *ui;
    int fMove;
    QString fTransaction;
    int fDocNumber;
    void countTotal();
};

#endif // FUELFLASHMOVE_H
