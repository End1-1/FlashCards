#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

class Dialog : public QDialog
{
public:
    Dialog();
    virtual void keyPressEvent(QKeyEvent *e) override;

protected:
    virtual void enter();
};

#endif // DIALOG_H
