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

private slots:
    void on_btnCancel_clicked();

private:
    Ui::FuelFlashMove *ui;
};

#endif // FUELFLASHMOVE_H
