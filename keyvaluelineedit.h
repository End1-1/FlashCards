#ifndef KEYVALUELINEEDIT_H
#define KEYVALUELINEEDIT_H

#include <QObject>
#include <QString>

#define keyvalueedit(l1, l2, c) KeyValueLineEdit::createKeyValueEditor(l1, l2, c)

class C5LineEdit;

class KeyValueLineEdit : public QObject
{
    Q_OBJECT
public:
    KeyValueLineEdit(C5LineEdit *lkey, C5LineEdit *lvalue, const QString &kv);
    ~KeyValueLineEdit();
    void setKey(const QString &key);
    static void createKeyValueEditor(C5LineEdit *lkey, C5LineEdit *lvalue, const QString &kv);

private:
    C5LineEdit *fleKey;
    C5LineEdit *fleValue;
    QString fKeyValueName;
    QList<QVariant> fValues;

private slots:
    void focusOut();
    void returnPressed();
    void doubleClicked();
};

Q_DECLARE_METATYPE (KeyValueLineEdit *)

#endif // KEYVALUELINEEDIT_H
