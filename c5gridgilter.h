#ifndef C5GRIDGILTER_H
#define C5GRIDGILTER_H

#include "dialog.h"

namespace Ui {
class C5GridGilter;
}

class C5FilterWidget;

class C5GridGilter : public Dialog
{
    Q_OBJECT

public:
    explicit C5GridGilter();

    ~C5GridGilter();

    static bool filter(C5FilterWidget *filterWidget, QString &condition, QMap<QString, bool> &showColumns, QMap<QString, QString> &colTranslation);

private slots:
    void on_btnCancel_clicked();

    void on_btnOK_clicked();

private:
    Ui::C5GridGilter *ui;
};

#endif // C5GRIDGILTER_H
