#include "wdbfuelflash.h"
#include "ui_wdbfuelflash.h"
#include <QColorDialog>

WdbFuelFlash::WdbFuelFlash(QWidget *parent) :
    WdbWidget(parent),
    ui(new Ui::WdbFuelFlash)
{
    ui->setupUi(this);
}

WdbFuelFlash::~WdbFuelFlash()
{
    delete ui;
}

QString WdbFuelFlash::tableName() const
{
    return "fuel_flash";
}

void WdbFuelFlash::on_btnSetColor_clicked()
{
    QColor color = QColor::fromRgb(ui->leColor->getInteger());
    color = QColorDialog::getColor(color, this, tr("Select color"));
    ui->leColor->setInteger(color.rgb());
}

void WdbFuelFlash::on_leColor_textChanged(const QString &arg1)
{
    QColor color = QColor::fromRgb(arg1.toInt());
    QPalette palette = ui->leColor->palette();
    palette.setColor(QPalette::Base, color);
    palette.setColor(QPalette::Text, color);
    ui->leColor->setPalette(palette);
}
