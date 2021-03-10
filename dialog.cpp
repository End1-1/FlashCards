#include "dialog.h"
#include "config.h"
#include <QKeyEvent>

Dialog::Dialog() :
    QDialog(__parent)
{

}

void Dialog::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) {
        enter();
        return;
    }
    QDialog::keyPressEvent(e);
}

void Dialog::enter()
{

}
