#ifndef WDTSALEDOCS_H
#define WDTSALEDOCS_H

#include "c5grid.h"

class WdtSaleDocs : public C5Grid
{
    Q_OBJECT
public:
    WdtSaleDocs(const QIcon &icon, const QString &label, QWidget *parent = nullptr);

private slots:
    void editDoc();
    virtual void callEditor(const QString &id);
};

#endif // WDTSALEDOCS_H
