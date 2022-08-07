#ifndef DLGFUELPRICES_H
#define DLGFUELPRICES_H

#include <QDialog>

namespace Ui {
class DlgFuelPrices;
}

class DlgFuelPrices : public QDialog
{
    Q_OBJECT

public:
    explicit DlgFuelPrices(const QDate &d, QWidget *parent = nullptr);
    ~DlgFuelPrices();

private slots:
    void on_btnDateForward_clicked();
    void on_btnDateBack_clicked();
    void on_btnSave_clicked();
    void on_btnCopyFromPreviouseDay_clicked();

private:
    Ui::DlgFuelPrices *ui;
    void loadPrices(const QDate &d);
};

#endif // DLGFUELPRICES_H
