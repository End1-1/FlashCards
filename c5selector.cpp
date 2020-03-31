#include "c5selector.h"
#include "ui_c5selector.h"
#include "c5grid.h"
#include "c5tablemodel.h"
#include <QKeyEvent>

static QMap<QString, QString> fSelectorQueries;

C5Selector::C5Selector() :
    Dialog(),
    ui(new Ui::C5Selector)
{
    ui->setupUi(this);
    fGrid = new C5Grid(QIcon(), "", nullptr);
    fGrid->fTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->hl->addWidget(fGrid);
    connect(fGrid, SIGNAL(tblDoubleClick(int,int,QList<QVariant>)), this, SLOT(tblDoubleClicked(int,int,QList<QVariant>)));
    connect(fGrid, SIGNAL(tblSingleClick(QModelIndex)), this, SLOT(tblSingleClick(QModelIndex)));
    fReset = true;
    if (fSelectorQueries.isEmpty()) {
        C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
        db.exec("select * from key_value");
        while (db.nextRow()) {
            fSelectorQueries[db.getString("fkey")] = db.getString("fvalue");
        }
    }
}

C5Selector::~C5Selector()
{
    delete ui;
}

void C5Selector::getKeyValue(const QString &keyvalue, C5LineEdit *lkey, C5LineEdit *lvalue)
{
    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
    QString query = fSelectorQueries[keyvalue] + " where fid=:fid";
    db[":fid"] = lkey->getInteger();
    db.exec(query);
    if (db.nextRow()) {
        if (lvalue) {
            lvalue->setText(db.getString(1));
        }
    } else {
        lkey->clear();
        if (lvalue) {
            lvalue->clear();
        }
    }
}

bool C5Selector::getValue(const QString &keyvalue, QList<QVariant> &values)
{
    fMultipleSelection = false;
    fQuery = fSelectorQueries[keyvalue];
    refresh();
    ui->leFilter->setFocus();
    bool result = exec() == QDialog::Accepted;
    values = fValues;
    return result;
}

bool C5Selector::getMultipleValues(const QString &keyvalue, QList<QList<QVariant> > &values)
{
    fMultipleSelection = true;
    fQuery = fSelectorQueries[keyvalue];
    refresh();
    ui->leFilter->setFocus();
    bool result = exec() == QDialog::Accepted;
    if (result) {
        for (int i = 0; i < fGrid->fModel->rowCount(); i++) {
            if (fGrid->fModel->data(i, 0, Qt::CheckStateRole) == Qt::Checked) {
                values.append(fGrid->fModel->getRowValues(i));
            }
        }
    }
    return result;
}

bool C5Selector::getValues(const QString &sql, QList<QVariant> &values, const QMap<QString, QString> &translator)
{
    fQuery = sql;
    fGrid->fTranslation = translator;
    refresh();
    bool result = exec() == QDialog::Accepted;
    values = fValues;
    return result;
}

void C5Selector::keyPressEvent(QKeyEvent *key)
{
    int row = fGrid->fTableView->currentIndex().row();
    QModelIndex index = fGrid->fTableView->model()->index(row, 0);
    switch (key->key()) {
    case Qt::Key_Up:
        if (row > 0) {
            row--;
        }
        break;
    case Qt::Key_Down:
        if (row < fGrid->fModel->rowCount() - 1) {
            row++;
        }
        break;
    case Qt::Key_Enter:
    case Qt::Key_Return:
        if (key->modifiers() & Qt::ControlModifier) {
            fGrid->on_tblView_doubleClicked(index);
        }
        break;
    default:
        break;
    }
    index = fGrid->fTableView->model()->index(row, 0);
    fGrid->fTableView->setCurrentIndex(index);
    Dialog::keyPressEvent(key);
}

void C5Selector::tblSingleClick(const QModelIndex &index)
{
    if (index.column() == 0) {
        return;
    }
    int v = fGrid->fModel->data(index.row(), 0, Qt::EditRole).toInt() == 0 ? 1 : 0;
    fGrid->fModel->setData(index.row(), 0, v, Qt::CheckStateRole);
}

bool C5Selector::tblDoubleClicked(int row, int column, const QList<QVariant> &values)
{
    Q_UNUSED(column);
    Q_UNUSED(row);
    fValues = values;
    if (fValues.count() > 0) {
        accept();
    }
    return true;
}

void C5Selector::on_btnRefreshCache_clicked()
{
    refresh();
}

void C5Selector::refresh()
{
    fGrid->setCheckboxes(true);
    fGrid->fModel->setSingleCheckBoxSelection(!fMultipleSelection);
    fGrid->buildQuery(fQuery);
    fGrid->fTableView->setColumnWidth(0, 30);
}

void C5Selector::on_leFilter_textChanged(const QString &arg1)
{
    fGrid->setFilter(-1, arg1);
}

void C5Selector::on_btnCheck_clicked()
{
    for (int i = 0; i < fGrid->fModel->rowCount(); i++) {
        if (fGrid->fModel->data(i, 0, Qt::CheckStateRole) == Qt::Checked) {
            fValues = fGrid->fModel->getRowValues(i);
            accept();
            return;
        }
    }
}
