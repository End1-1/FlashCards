#ifndef WDBPARTNER_H
#define WDBPARTNER_H

#include "wdbwidget.h"
#include <QSet>

namespace Ui {
class WdbPartner;
}

class WdbPartner : public WdbWidget
{
    Q_OBJECT

public:
    explicit WdbPartner(QWidget *parent = nullptr);
    ~WdbPartner();
    virtual QString tableName() const;

private:
    Ui::WdbPartner *ui;
    QSet<QString> fStrings;
};

#endif // WDBPARTNER_H
