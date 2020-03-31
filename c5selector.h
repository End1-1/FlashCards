#ifndef C5SELECTOR_H
#define C5SELECTOR_H

#include "dialog.h"

namespace Ui {
class C5Selector;
}

class C5Grid;
class C5TableModel;
class C5LineEdit;

class C5Selector : public Dialog
{
    Q_OBJECT

public:
    explicit C5Selector();
    ~C5Selector();
    void getKeyValue(const QString &keyvalue, C5LineEdit *lkey, C5LineEdit *lvalue);
    bool getValue(const QString &keyvalue, QList<QVariant> &values);
    bool getMultipleValues(const QString &keyvalue, QList<QList<QVariant> > &values);
    bool getValues(const QString &sql, QList<QVariant> &values, const QMap<QString, QString> &translator);
    void keyPressEvent(QKeyEvent *key) override;

private slots:
    void tblSingleClick(const QModelIndex &index);
    virtual bool tblDoubleClicked(int row, int column, const QList<QVariant> &values);
    void on_btnRefreshCache_clicked();
    void on_leFilter_textChanged(const QString &arg1);
    void on_btnCheck_clicked();

private:
    Ui::C5Selector *ui;
    int fCache;
    QString fQuery;
    void refresh();
    C5Grid *fGrid;
    QList<QVariant> fValues;
    bool fReset;
    bool fMultipleSelection;
};

#endif // C5SELECTOR_H
