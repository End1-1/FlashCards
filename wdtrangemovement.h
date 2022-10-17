#ifndef WDTRANGEMOVEMENT_H
#define WDTRANGEMOVEMENT_H

#include "c5grid.h"
#include <QObject>

class WdtRangeMovementFilter;

class WdtRangeMovement : public C5Grid
{
    Q_OBJECT
public:
    WdtRangeMovement(const QIcon &icon, const QString &label, QWidget *parent = nullptr);

public slots:
    virtual void buildQuery() override;

private:
    WdtRangeMovementFilter *fFilter;
};

#endif // WDTRANGEMOVEMENT_H
