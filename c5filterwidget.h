#ifndef C5FILTERWIDGET_H
#define C5FILTERWIDGET_H

#include "config.h"
#include "keyvaluelineedit.h"
#include <QWidget>

class C5FilterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit C5FilterWidget(QWidget *parent = nullptr);
    ~C5FilterWidget();
    virtual QString condition() = 0;
    virtual QString conditionText() = 0;
    virtual QString filterString() = 0;
    virtual void saveFilter(QWidget *parent);
    virtual void restoreFilter(QWidget *parent);
    virtual void clearFilter(QWidget *parent);
    virtual QMap<QString, QString> replaces() ;

protected:
    QMap<QString, QString> fReplaceData;
    void concat(C5LineEdit *l1, C5LineEdit *l2, const QString &title, QString &v1);

private:
    C5LineEdit *isKeyValueEdit(QObject *o);
    KeyValueLineEdit *keyValue(C5LineEdit *l);
};

#endif // C5FILTERWIDGET_H
