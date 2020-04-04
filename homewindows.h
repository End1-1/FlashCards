#ifndef HOMEWINDOWS_H
#define HOMEWINDOWS_H

#include <QMainWindow>
#include <QTabWidget>
#include <QTabBar>
#include <QToolButton>

namespace Ui {
class HomeWindows;
}

class HomeWindows : public QMainWindow
{
    Q_OBJECT

public:
    explicit HomeWindows(QWidget *parent = nullptr);
    ~HomeWindows();
    void login();
    void logout();
    void buildEditMenu(QList<QAction *> actions);
    void buildReportMenu(QList<QAction *> actions);
    void buildOtherMenu(QList<QMenu *> menu);
    void setMenuState(bool v);
    virtual void closeEvent(QCloseEvent *event) override;

    template<typename T>
    T *addWidget(QToolButton *btn) {
        T *t = new T(btn->icon(), btn->text());
        int index = fTab->addTab(t, btn->icon(), btn->text());
        if (index == 0) {
            fTab->tabBar()->tabButton(0, QTabBar::RightSide)->setVisible(false);
        }
        fTab->setCurrentIndex(index);
        return t;
    }

private slots:
    void hotKey();
    void on_actionConfiguration_triggered();
    void on_tabWidget_tabCloseRequested(int index);
    void on_actionPrint_triggered();
    void on_actionExport_to_Excel_triggered();
    void on_actionLogin_triggered();
    void on_tabWidget_currentChanged(int index);
    void on_actionExit_triggered();

private:
    Ui::HomeWindows *ui;
    QTabWidget *fTab;
};

#endif // HOMEWINDOWS_H
