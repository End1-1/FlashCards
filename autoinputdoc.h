#ifndef AUTOINPUTDOC_H
#define AUTOINPUTDOC_H

#include "dialog.h"

namespace Ui {
class AutoinputDoc;
}

class AutoinputDoc : public Dialog
{
    Q_OBJECT

public:
    explicit AutoinputDoc();
    ~AutoinputDoc();
    void setDoc(int id);
    AutoinputDoc &newDoc();

private slots:
    void on_btnRemove_clicked();
    void on_btnWrite_clicked();
    void on_btnWriteDraft_clicked();

private:
    Ui::AutoinputDoc *ui;
    void write(int docstate, int movestate);
};

#endif // AUTOINPUTDOC_H
