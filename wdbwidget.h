#ifndef WDBWIDGET_H
#define WDBWIDGET_H

#include <QWidget>
#include <QMap>

class C5LineEdit;

class WdbWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WdbWidget(QWidget *parent = nullptr);
    virtual bool save(QWidget *parent);
    virtual void setId(int id);
    virtual int id();

protected:
    virtual QString tableName() const = 0;
    virtual QString editQuery() const;
    QMap<QString, QVariant> fValues;

private:
    QString fError;
    C5LineEdit *fLineID;
    void buildFieldsValues(QWidget *parent);
    void buildFieldsForEdit(QMap<QString, QWidget *> &fields, QWidget *parent);
};

#endif // WDBWIDGET_H
