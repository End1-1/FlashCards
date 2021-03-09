#include "fuelflashmove.h"
#include "ui_fuelflashmove.h"

FuelFlashMove::FuelFlashMove() :
    Dialog(),
    ui(new Ui::FuelFlashMove)
{
    ui->setupUi(this);
}

FuelFlashMove::~FuelFlashMove()
{
    delete ui;
}

void FuelFlashMove::on_btnCancel_clicked()
{
    reject();
}
