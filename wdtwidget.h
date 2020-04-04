#ifndef WDTWIDGET_H
#define WDTWIDGET_H

#include <QWidget>
#include <QIcon>
#include <QAction>
#include <QMenu>

class WdtWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WdtWidget(const QIcon &icon, const QString &label, QWidget *parent = nullptr);
    ~WdtWidget();
    virtual QIcon icon() const;
    virtual QString label() const;
    virtual void hkFind();
    virtual void hkEscape();
    virtual QList<QAction *> editMenu() const;
    virtual QList<QAction *> reportMenu() const;
    virtual QList<QMenu *> otherMenu() const;

public slots:
    virtual void print();
    virtual void exportToExcel();

protected:
    QIcon fIcon;
    QString fLabel;
    QList<QAction *> fEditActions;
    QList<QAction *> fReportActions;
    QList<QMenu *> fOtherMenu;
    QAction *addAction(QList<QAction *> &list, const QString &icon, const QString &title, QObject *receiver, const char *slot);

protected slots:

};

#endif // WDTWIDGET_H
