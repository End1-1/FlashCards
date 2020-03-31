#include "c5message.h"
#include "ui_c5message.h"
#include "config.h"

C5Message::C5Message() :
    Dialog(),
    ui(new Ui::C5Message)
{
    ui->setupUi(this);
}

C5Message::~C5Message()
{
    delete ui;
}

int C5Message::error(const QString &errorStr)
{
    return showMessage(errorStr, "red");
}

int C5Message::info(const QString &infoStr)
{
    return showMessage(infoStr, "green");
}

int C5Message::question(const QString &questionStr)
{
    return showMessage(questionStr, "blue");
}

int C5Message::showMessage(const QString &text, const QString &color)
{
    C5Message *c5 = new C5Message();
    c5->ui->lbMessage->setStyleSheet("color:" + color);
    c5->ui->lbMessage->setText(text);
    c5->adjustSize();
    int result = c5->exec();
    delete c5;
    return result;
}

void C5Message::on_btnYes_clicked()
{
    accept();
}

void C5Message::on_btnCancel_clicked()
{
    reject();
}
