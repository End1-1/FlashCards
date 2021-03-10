#ifndef WDTFUELFLASHMOVE_H
#define WDTFUELFLASHMOVE_H

#include "c5grid.h"

class WdtFuelFlashMove : public C5Grid
{
    Q_OBJECT
public:
    WdtFuelFlashMove(const QIcon &icon, const QString &label, QWidget *parent = nullptr);

private slots:
    void newDocInput();
    void newDocOutput();
    void editDoc();
    virtual bool tblDoubleClicked(int row, int column, const QList<QVariant> &values) override;
};

#endif // WDTFUELFLASHMOVE_H
