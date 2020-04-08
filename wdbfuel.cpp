#include "wdbfuel.h"
#include "ui_wdbfuel.h"
#include <QColorDialog>

WdbFuel::WdbFuel(QWidget *parent) :
    WdbWidget(parent),
    ui(new Ui::WdbFuel)
{
    ui->setupUi(this);
    ui->wc->enumLineEdit();
}

WdbFuel::~WdbFuel()
{
    delete ui;
}

QString WdbFuel::tableName() const
{
    return "fuel";
}

void WdbFuel::on_btnSetColor_clicked()
{
    QColor color = QColor::fromRgb(ui->leColor->getInteger());
    color = QColorDialog::getColor(color, this, tr("Select color"));
    ui->leColor->setInteger(color.rgb());
}

void WdbFuel::on_leColor_textChanged(const QString &arg1)
{
    QColor color = QColor::fromRgb(arg1.toInt());
    QPalette palette = ui->leColor->palette();
    palette.setColor(QPalette::Base, color);
    palette.setColor(QPalette::Text, color);
    ui->leColor->setPalette(palette);
}
