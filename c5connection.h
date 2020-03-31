#ifndef C5CONNECTION_H
#define C5CONNECTION_H

#include "dialog.h"

namespace Ui {
class C5Connection;
}

class C5Connection : public Dialog
{
    Q_OBJECT

public:
    explicit C5Connection();

    ~C5Connection();

    static void configure();

    static void readParams(QStringList &p);

    static void writeParams();

    virtual bool preambule();

private slots:
    void on_btnCancel_clicked();

    void on_btnTest_clicked();

    void on_btnSave_clicked();

    void on_btnInit_clicked();

private:
    Ui::C5Connection *ui;
};

#endif // C5CONNECTION_H
