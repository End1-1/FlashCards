#include "c5filterwidget.h"
#include "c5dateedit.h"
#include "c5checkbox.h"
#include "keyvaluelineedit.h"
#include <QSettings>

C5FilterWidget::C5FilterWidget(QWidget *parent) :
    QWidget(parent)
{

}

void C5FilterWidget::saveFilter(QWidget *parent)
{
    QSettings s(_ORGANIZATION_, QString("%1\\%2\\reportfilter\\%3")
                .arg(_APPLICATION_)
                .arg(_MODULE_)
                .arg(metaObject()->className()));
    QObjectList ol = parent->children();
    QVariant value;
    C5LineEdit *le;
    C5DateEdit *de;
    C5CheckBox *ce;
    foreach (QObject *o, ol) {
        if (o->children().count() > 0) {
            saveFilter(static_cast<QWidget*>(o));
        }
        if ((le = isKeyValueEdit(o))) {
            value = le->property("FilterName");
            if (value.isValid()) {
                s.setValue(value.toString(), le->text());
            }
            continue;
        }
        if ((de = isDateEdit(o))) {
            value = de->property("FilterName");
            if (value.isValid()) {
                s.setValue(value.toString(), de->date());
            }
            continue;
        }
        if ((ce = isCheckBox(o))) {
            value = ce->property("FilterName");
            if (value.isValid()) {
                s.setValue(value.toString(), ce->isChecked());
            }
            continue;
        }
    }
}

void C5FilterWidget::restoreFilter(QWidget *parent)
{
    QSettings s(_ORGANIZATION_, QString("%1\\%2\\reportfilter\\%3")
                .arg(_APPLICATION_)
                .arg(_MODULE_)
                .arg(metaObject()->className()));
    QObjectList ol = parent->children();
    QString filterName;
    QVariant value;
    C5LineEdit *le;
    C5DateEdit *de;
    C5CheckBox *ce;
    foreach (QObject *o, ol) {
        if (o->children().count() > 0) {
            saveFilter(static_cast<QWidget*>(o));
        }
        if ((le = isKeyValueEdit(o))) {
            filterName = le->property("FilterName").toString();
            if (filterName.isEmpty()) {
                continue;
            }
            value = s.value(filterName, "");
            keyValue(le)->setKey(value.toString());
            continue;
        }
        if ((de = isDateEdit(o))) {
            filterName = de->property("FilterName").toString();
            if (filterName.isEmpty()) {
                continue;
            }
            value = s.value(filterName, QDate::currentDate());
            de->setDate(value.toDate());
            continue;
        }
        if ((ce = isCheckBox(o))) {
            filterName = ce->property("FilterName").toString();
            if (filterName.isEmpty()) {
                continue;
            }
            value = s.value(filterName, 0);
            ce->setChecked(value.toBool());
            continue;
        }
    }
}

void C5FilterWidget::clearFilter(QWidget *parent)
{
    QObjectList ol = parent->children();
    C5LineEdit *le;
    C5DateEdit *de;
    C5CheckBox *ce;
    foreach (QObject *o, ol) {
        if (o->children().count() > 0) {
            clearFilter(static_cast<QWidget*>(o));
        }
        if ((le = isKeyValueEdit(o))) {
            keyValue(le)->setKey("");
            continue;
        }
        if ((de = isDateEdit(o))) {
            de->setDate(QDate::currentDate());
            continue;
        }
        if ((ce = isCheckBox(o))) {
            ce->setChecked(false);
            continue;
        }
    }
}

C5LineEdit *C5FilterWidget::isKeyValueEdit(QObject *o)
{
    C5LineEdit *le = dynamic_cast<C5LineEdit *>(o);
    if (le) {
        if (le->property("lkey").isValid()) {
            return le;
        }
    }
    return nullptr;
}

KeyValueLineEdit *C5FilterWidget::keyValue(C5LineEdit *l)
{
    if (l->property("keyvalue").isValid()) {
        return l->property("keyvalue").value<KeyValueLineEdit*>();
    }
    return nullptr;
}
