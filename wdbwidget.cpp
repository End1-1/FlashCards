#include "wdbwidget.h"
#include "c5lineedit.h"
#include "c5dateedit.h"
#include "c5database.h"
#include "c5message.h"
#include "config.h"
#include "keyvaluelineedit.h"
#include <QIntValidator>

WdbWidget::WdbWidget(QWidget *parent) : QWidget(parent)
{
    fLineID = nullptr;
}

bool WdbWidget::save(QWidget *parent)
{
    fError.clear();
    fValues.clear();
    buildFieldsValues(parent);
    if (!fError.isEmpty()) {
        C5Message::error(fError);
        return false;
    }
    Q_ASSERT(fLineID != nullptr);
    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
    db.setBindValues(fValues);
    if (fLineID->getInteger() == 0) {
        fLineID->setInteger(db.insert(tableName(), true));
        if (fLineID->getInteger() == 0) {
            C5Message::error(db.fLastError);
            return false;
        }
    } else {
        if (!db.update(tableName(), where_id(fLineID->text()))) {
            C5Message::error(db.fLastError);
            return false;
        }
    }
    C5Message::info(tr("Saved"));
    return true;
}

void WdbWidget::setId(int id)
{
    QMap<QString, QWidget*> f;
    buildFieldsForEdit(f, this);
    if (id == 0) {
        return;
    }
    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
    db[":fid"] = id;
    db.exec(editQuery());
    if (db.nextRow()) {
        for (int i = 0; i < db.columnCount(); i++) {
            QString fieldName = ":" + db.columnName(i);
            if (f.contains(fieldName)) {
                C5LineEdit *l = dynamic_cast<C5LineEdit*>(f[fieldName]);
                if (l) {
                    if (l->property("lkey").isValid()) {
                        KeyValueLineEdit *kve = l->property("keyvalue").value<KeyValueLineEdit*>();
                        if (kve) {
                            kve->setKey(db.getString(i));
                        }
                    } else {
                        l->setText(db.getString(i));
                    }
                    continue;
                }
                C5DateEdit *d = dynamic_cast<C5DateEdit*>(f[fieldName]);
                if (d) {
                    d->setDate(db.getDate(i));
                    continue;
                }
            }
        }
    }
}

int WdbWidget::id()
{
    if (fLineID) {
        return fLineID->getInteger();
    } else {
        return 0;
    }
}

QString WdbWidget::editQuery() const
{
    return QString("select * from %1 where %1.fid=:fid").arg(tableName());
}

void WdbWidget::buildFieldsValues(QWidget *parent)
{
    QObjectList ol = parent->children();
    C5LineEdit *le = nullptr;
    C5DateEdit *de = nullptr;
    QWidget *w = nullptr;
    for (QObject *o: ol) {
        le = dynamic_cast<C5LineEdit *>(o);
        if (le) {
            if (le->property("id").toBool()) {
                fLineID = le;
                fLineID->setReadOnly(true);
                fLineID->setFocusPolicy(Qt::NoFocus);
            }
            if (le->property("field").isValid()) {
                switch (le->property("type").toInt()) {
                case 0:
                    fValues[le->property("field").toString()] = le->getInteger();
                    break;
                case 1:
                    le->setText(le->text().toUpper());
                    fValues[le->property("field").toString()] = le->text();
                    break;
                }
            }
            if (le->property("noempty").isValid()) {
                if (le->isEmpty()) {
                    fError += QString("%1 %2").arg(le->property("noempty").toString()).arg(tr("cannot be null")) + "\r\n";
                }
            }
            continue;
        }
        de = dynamic_cast<C5DateEdit *>(o);
        if (de) {
            if (de->property("field").isValid()) {
                fValues[le->property("field").toString()] = de->date();
            }
            continue;
        }
        w = dynamic_cast<QWidget *>(o);
        if (w) {
            buildFieldsValues(w);
        }
    }
}

void WdbWidget::buildFieldsForEdit(QMap<QString, QWidget*> &fields, QWidget *parent)
{
    QObjectList ol = parent->children();
    C5LineEdit *le = nullptr;
    C5DateEdit *de = nullptr;
    QWidget *w = nullptr;
    for (QObject *o: ol) {
        le = dynamic_cast<C5LineEdit *>(o);
        if (le) {
            if (le->property("id").toBool()) {
                fLineID = le;
                fLineID->setReadOnly(true);
                fLineID->setFocusPolicy(Qt::NoFocus);
            }
            if (le->property("field").isValid()) {
                QString fieldName = le->property("field").toString();
                fields[fieldName] = le;
            }
            if (le->property("type").isValid()) {
                switch (le->property("type").toInt()) {
                case 0:
                    le->setValidator(new QIntValidator());
                    break;
                }
            }
            continue;
        }
        de = dynamic_cast<C5DateEdit *>(o);
        if (de) {
            if (de->property("field").isValid()) {
                fields[de->property("field").toString()] = de;
            }
            continue;
        }
        w = dynamic_cast<QWidget *>(o);
        if (w) {
            buildFieldsForEdit(fields, w);
        }
    }
}
