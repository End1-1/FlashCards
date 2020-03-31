#ifndef WDTCONTAINER_H
#define WDTCONTAINER_H

#include <QWidget>
#include <QMap>

class C5LineEdit;

struct KV {
    C5LineEdit *lkey;
    C5LineEdit *lvalue;
    KV() {
        lkey = nullptr;
        lvalue = nullptr;
    }
};

class WdtContainer : public QWidget
{
    Q_OBJECT
public:
    explicit WdtContainer(QWidget *parent = nullptr);
    void enumLineEdit();
    void setKey(C5LineEdit *le, const QString &key);
    void setKey(C5LineEdit *le, int key);

private:
    void setKeyValueEdit(QWidget *parent);
    QMap<QString, KV> fKeyValuePairs;
};

#endif // WDTCONTAINER_H
