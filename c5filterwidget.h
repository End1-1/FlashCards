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
    virtual void saveFilter(QWidget *parent);
    virtual void restoreFilter(QWidget *parent);
    virtual void clearFilter(QWidget *parent);

private:
    C5LineEdit *isKeyValueEdit(QObject *o);
    KeyValueLineEdit *keyValue(C5LineEdit *l);
};

#endif // C5FILTERWIDGET_H
