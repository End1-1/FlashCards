#include "homewindows.h"
#include "ui_homewindows.h"
#include "login.h"
#include "c5connection.h"
#include "wdthome.h"
#include "c5message.h"
#include <QShortcut>
#include <QCloseEvent>

HomeWindows::HomeWindows(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HomeWindows)
{
    ui->setupUi(this);
    fTab = ui->tabWidget;
    QShortcut *sh = new QShortcut(QKeySequence("Ctrl+F"), this);
    connect(sh, SIGNAL(activated()), this, SLOT(hotKey()));
    sh = new QShortcut(QKeySequence("Esc"), this);
    connect(sh, SIGNAL(activated()), this, SLOT(hotKey()));
    logout();
}

HomeWindows::~HomeWindows()
{
    delete ui;
}

void HomeWindows::login()
{
    if (!Login::login()) {
        return;
    }
    setMenuState(true);
    QToolButton btn;
    btn.setIcon(QIcon(":/res/homepage.png"));
    btn.setText(tr("Home"));
    addWidget<WdtHome>(&btn);
    ui->menuMenu->setEnabled(true);
}

void HomeWindows::logout()
{
    setMenuState(false);
}

void HomeWindows::buildEditMenu(QList<QAction*> actions)
{
    ui->menuEdit->clear();
    ui->menuEdit->setEnabled(actions.count() > 0);
    ui->menuEdit->insertActions(nullptr, actions);
}

void HomeWindows::buildReportMenu(QList<QAction *> actions)
{
    ui->menuReport->clear();
    ui->menuReport->setEnabled(actions.count() > 0);
    ui->menuReport->insertActions(nullptr, actions);
}

void HomeWindows::setMenuState(bool v)
{
    QList<QAction*> menu = ui->menuMenu->actions();
    for (int i = 0; i < menu.count(); i++) {
        if (i == 0) {
            menu.at(i)->setEnabled(!v);
            continue;
        }
        if (i == 1) {
            continue;
        }
        if (i == menu.count() - 1) {
            continue;
        }
        menu.at(i)->setEnabled(v);
    }
    ui->menuEdit->setEnabled(v);
    ui->menuReport->setEnabled(v);
}

void HomeWindows::closeEvent(QCloseEvent *event)
{
    if (C5Message::question(tr("Are you sure to close application?")) == QDialog::Accepted) {
        QMainWindow::closeEvent(event);
        return;
    }
    event->ignore();
}

void HomeWindows::hotKey()
{
    QShortcut *s = static_cast<QShortcut*>(sender());
    WdtWidget *w = static_cast<WdtWidget*>(fTab->currentWidget());
    if (!w) {
        return;
    }
    QString k = s->key().toString().toLower();
    if (k == "ctrl+f") {
        w->hkFind();
    } else if (k == "esc") {
        w->hkEscape();
    }
}

void HomeWindows::on_actionConfiguration_triggered()
{
    C5Connection::configure();
}

void HomeWindows::on_tabWidget_tabCloseRequested(int index)
{
    QWidget *w = fTab->widget(index);
    fTab->removeTab(index);
    w->deleteLater();
}

void HomeWindows::on_actionPrint_triggered()
{
    WdtWidget *w = static_cast<WdtWidget*>(fTab->currentWidget());
    if (!w) {
        return;
    }
    w->print();
}

void HomeWindows::on_actionExport_to_Excel_triggered()
{
    WdtWidget *w = static_cast<WdtWidget*>(fTab->currentWidget());
    if (!w) {
        return;
    }
    w->exportToExcel();
}

void HomeWindows::on_actionLogin_triggered()
{
    login();
}

void HomeWindows::on_tabWidget_currentChanged(int index)
{
    WdtWidget *w = static_cast<WdtWidget*>(fTab->widget(index));
    if (w) {
        buildEditMenu(w->editMenu());
        buildReportMenu(w->reportMenu());
    }
}

void HomeWindows::on_actionExit_triggered()
{
    close();
}
