#ifndef C5GRID_H
#define C5GRID_H

#include "c5database.h"
#include "c5tableview.h"
#include "config.h"
#include "c5message.h"
#include "wdtwidget.h"
#include <QDebug>
#include <QHBoxLayout>
#include <QTableView>
#include <QSettings>

class C5TableModel;

namespace Ui {
class C5Grid;
}

class C5FilterWidget;
class WdbWidget;
class C5LineEdit;
class C5TableWidget;

class C5Grid : public WdtWidget
{
    Q_OBJECT

public:
    explicit C5Grid(const QIcon &icon, const QString &label, QWidget *parent = nullptr);
    ~C5Grid();
    C5TableView *fTableView;
    C5TableWidget *fTableTotal;
    C5TableModel *fModel;
    QMap<QString, QString> fTranslation;
    void setTableForUpdate(const QString &table, const QList<int> &columns);
    virtual void postProcess();
    virtual void buildQuery(const QString &query);
    void setFilter(int column, const QString &filter);
    virtual void setCheckboxes(bool v);
    virtual void hkFind() override;
    virtual void hkEscape() override;

public slots:
    virtual void buildQuery();
    virtual bool on_tblView_doubleClicked(const QModelIndex &index);

protected:
    C5Database fDb;
    bool fSimpleQuery;
    QString fSqlQuery;
    QString fMainTable;
    QStringList fLeftJoinTables;
    QStringList fColumnsFields;
    QMap<QString, bool> fColumnsVisible;
    QStringList fColumnsGroup;
    QStringList fColumnsSum;
    QString fWhereCondition;
    QString fHavindCondition;
    QString fGroupCondition;
    QString fOrderCondition;
    C5FilterWidget *fFilterWidget;
    C5LineEdit *fFilterLineEdit;
    QWidget *widget();
    QHBoxLayout *hl();
    int rowId();
    int rowId(int column);
    int rowId(int &row, int column);
    virtual void cellClicked(const QModelIndex &index);
    virtual void callEditor(const QString &id);
    virtual void removeWithId(int id, int row);
    void sumColumnsData();
    virtual void restoreColumnsVisibility();
    virtual void restoreColumnsWidths();
    virtual QString reportAdditionalTitle();
    virtual QMenu *buildTableViewContextMenu(const QPoint &point);
    virtual bool tblDoubleClicked(int row, int column, const QList<QVariant> &values);
    virtual void buildReport(const QString &name);
    virtual void insertNewRow(WdbWidget *w);
    virtual void updateRow(WdbWidget *w);

    template<typename T>
    void newElement() {
        T *t = new T();
        insertNewRow(t);
    }

    template<typename T>
    void editElement() {
        T *t = new T();
        updateRow(t);
    }

private:
    Ui::C5Grid *ui;
    QModelIndex fFilterIndex;
    QString fFilterString;
    bool fCheckboxes;
    int fFilterColumn;
    void insertJoinTable(QStringList &joins, QMap<QString, QString> &joinsMap, const QString &table, const QString &mainTable);
    int sumOfColumnsWidghtBefore(int column);

protected slots:
    virtual void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    virtual void copySelection();
    virtual void copyAll();
    virtual void saveDataChanges();
    bool currentRow(int &row);
    virtual void editRow(int columnWidthId = 0);
    virtual void removeRow(int columnWithId = 0);
    virtual void print();
    virtual void exportToExcel();
    virtual void clearFilter();
    virtual void refreshData();
    virtual void setSearchParameters();
    virtual void resetSearchParameters();
    virtual void tableViewContextMenuRequested(const QPoint &point);
    virtual void tableViewHeaderContextMenuRequested(const QPoint &point);
    virtual void tableViewHeaderClicked(int index);
    virtual void tableViewHeaderResized(int column, int oldSize, int newSize);

private slots:    
    void ctrlEnter();
    void autofitColumns();
    void filterByColumn();
    void filterByStringAndIndex();
    void removeFilterForColumn();
    void tblValueChanged(int pos);
    void on_tblView_clicked(const QModelIndex &index);

signals:
    void tblSingleClick(const QModelIndex &);
    void tblDoubleClick(int row, int column, const QList<QVariant> &values);


};

#endif // C5GRID_H
