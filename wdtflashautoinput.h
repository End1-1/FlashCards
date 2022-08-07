#ifndef WDTFLASHAUTOINPUT_H
#define WDTFLASHAUTOINPUT_H

#include "c5grid.h"

class WdtFlashAutoinput : public C5Grid
{
    Q_OBJECT
public:
    WdtFlashAutoinput(const QIcon &icon, const QString &label, QWidget *parent = nullptr);

public slots:
    void editDoc();
    virtual bool tblDoubleClicked(int row, int column, const QList<QVariant> &values) override;
};

#endif // WDTFLASHAUTOINPUT_H
