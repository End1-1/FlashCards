#ifndef DBDIALOG_H
#define DBDIALOG_H

#include "dialog.h"
#include "wdbwidget.h"

namespace Ui {
class DbDialog;
}

class DbDialog : public Dialog
{
    Q_OBJECT

public:
    explicit DbDialog();
    ~DbDialog();

    template<typename T>
    static int createDialog(int id = 0) {
        DbDialog *d = new DbDialog();
        WdbWidget *w = reinterpret_cast<WdbWidget *>(new T());
        w->setId(id);
        d->setWidget(w);
        d->exec();
        int result = d->fWidget->id();
        delete d;
        return result;
    }

private slots:
    void on_btnCancel_clicked();
    void on_btnAdd_clicked();

private:
    Ui::DbDialog *ui;
    WdbWidget *fWidget;
    void setWidget(WdbWidget *w);
};

#endif // DBDIALOG_H
