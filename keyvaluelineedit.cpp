#include "keyvaluelineedit.h"
#include "c5lineedit.h"
#include "c5selector.h"

static C5Selector *__sel = nullptr;

KeyValueLineEdit::KeyValueLineEdit(C5LineEdit *lkey, C5LineEdit *lvalue, const QString &kv) :
    QObject(lkey)
{
    if (__sel == nullptr) {
        __sel = new C5Selector();
    }
    fleKey = lkey;
    fleValue = lvalue;
    fKeyValueName = kv;
    connect(lkey, SIGNAL(focusOut()), this, SLOT(focusOut()));
    connect(lkey, SIGNAL(returnPressed()), this, SLOT(returnPressed()));
    connect(lkey, SIGNAL(doubleClicked()), this, SLOT(doubleClicked()));
    if (lvalue) {
        lvalue->setReadOnly(true);
        lvalue->setFocusPolicy(Qt::NoFocus);
    }
}

KeyValueLineEdit::~KeyValueLineEdit()
{

}

void KeyValueLineEdit::setKey(const QString &key)
{
    fleKey->setText(key);
    __sel->getKeyValue(fKeyValueName, fleKey, fleValue);
}

void KeyValueLineEdit::createKeyValueEditor(C5LineEdit *lkey, C5LineEdit *lvalue, const QString &kv)
{
    KeyValueLineEdit *kve = new KeyValueLineEdit(lkey, lvalue, kv);
    QVariant vkve = qVariantFromValue(kve);
    lkey->setProperty("keyvalue", vkve);
}

void KeyValueLineEdit::focusOut()
{
    __sel->getKeyValue(fKeyValueName, fleKey, fleValue);
}

void KeyValueLineEdit::returnPressed()
{
    __sel->getKeyValue(fKeyValueName, fleKey, fleValue);
}

void KeyValueLineEdit::doubleClicked()
{
    if (__sel->getValue(fKeyValueName, fValues)) {
        if (fleKey) {
            fleKey->setText(fValues.at(1).toString());
        }
        if (fleValue) {
            fleValue->setText(fValues.at(2).toString());
        }
    }
}
