#ifndef C5FILTERVALUES_H
#define C5FILTERVALUES_H

#include "dialog.h"

namespace Ui {
class C5FilterValues;
}

class C5FilterValues : public Dialog
{
    Q_OBJECT

public:
    explicit C5FilterValues();

    ~C5FilterValues();

    static bool filterValues(QStringList &values);

private slots:
    void on_leFilter_textChanged(const QString &arg1);

    void on_buttonBox_rejected();

    void on_buttonBox_accepted();

private:
    Ui::C5FilterValues *ui;
};

#endif // C5FILTERVALUES_H
