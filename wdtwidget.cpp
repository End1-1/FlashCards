#include "wdtwidget.h"

WdtWidget::WdtWidget(const QIcon &icon, const QString &label, QWidget *parent) :
    QWidget(parent)
{
    fIcon = icon;
    fLabel = label;
}

WdtWidget::~WdtWidget()
{
    for (QAction *a: fEditActions) {
        a->deleteLater();
    }
    for (QAction *a: fReportActions) {
        a->deleteLater();
    }
}

QIcon WdtWidget::icon() const
{
    return fIcon;
}

QString WdtWidget::label() const
{
    return fLabel;
}

void WdtWidget::hkFind()
{

}

void WdtWidget::hkEscape()
{

}

QList<QAction *> WdtWidget::editMenu() const
{
    return fEditActions;
}

QList<QAction *> WdtWidget::reportMenu() const
{
    return fReportActions;
}

void WdtWidget::print()
{

}

void WdtWidget::exportToExcel()
{

}

QAction *WdtWidget::addAction(QList<QAction *> &list, const QString &icon, const QString &title, QObject *receiver, const char *slot)
{
    QAction *a = new QAction(QIcon(icon), title);
    connect(a, SIGNAL(triggered()), receiver, slot);
    list.append(a);
    return a;
}
