#include "wdtcontainer.h"
#include "c5lineedit.h"
#include "keyvaluelineedit.h"

WdtContainer::WdtContainer(QWidget *parent) :
    QWidget(parent)
{

}

void WdtContainer::enumLineEdit()
{
    fKeyValuePairs.clear();
    setKeyValueEdit(this);
    for (const QString &c: fKeyValuePairs.keys()) {
        KV &kv = fKeyValuePairs[c];
        keyvalueedit(kv.lkey, kv.lvalue, c);
    }
}

void WdtContainer::setKey(C5LineEdit *le, const QString &key)
{
    KeyValueLineEdit *kve = le->property("keyvalue").value<KeyValueLineEdit*>();
    kve->setKey(key);
}

void WdtContainer::setKey(C5LineEdit *le, int key)
{
    setKey(le, QString::number(key));
}

void WdtContainer::setKeyValueEdit(QWidget *parent)
{
    QObjectList ol = parent->children();
    for (QObject *o: ol) {
        C5LineEdit *le = dynamic_cast<C5LineEdit *>(o);
        if (le) {
            if (le->property("lkey").isValid()) {
                KV &kv = fKeyValuePairs[le->property("lkey").toString()];
                kv.lkey = le;
            }
            if (le->property("lvalue").isValid()) {
                KV &kv = fKeyValuePairs[le->property("lvalue").toString()];
                kv.lvalue = le;
            }
            continue;
        }
        QWidget *w = dynamic_cast<QWidget *>(o);
        if (w) {
            setKeyValueEdit(static_cast<QWidget *>(o));
            continue;
        }
    }
}
