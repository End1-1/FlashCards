#include "c5grid.h"
#include "ui_c5grid.h"
#include "c5tablemodel.h"
#include "c5filtervalues.h"
#include "c5filterwidget.h"
#include "c5printing.h"
#include "c5printpreview.h"
#include "c5gridgilter.h"
#include "c5lineedit.h"
#include "dbdialog.h"
#include <QMenu>
#include <QLabel>
#include <QScrollBar>
#include <QClipboard>
#include <QFileDialog>
#include <QDesktopServices>
#include <QShortcut>
#include <QXlsx/header/xlsxdocument.h>

QString columnNumberToLetter(int n)
{
    QString letter = "";
    while (n > 0) {
        n--;  // Уменьшаем на 1, чтобы учитывать нумерацию с A
        letter = char(n % 26 + 'A') + letter;
        n /= 26;
    }
    return letter;
}

C5Grid::C5Grid(const QIcon &icon, const QString &label, QWidget *parent) :
    WdtWidget(icon, label, parent),
    fDb(__dbhost, __dbschema, __dbusername, __dbpassword),
    ui(new Ui::C5Grid)
{
    ui->setupUi(this);
    fCheckboxes = false;
    fModel = new C5TableModel(fDb, ui->tblView);
    ui->tblView->setModel(fModel);
    fSimpleQuery = true;
    fTableView = ui->tblView;
    fTableTotal = ui->tblTotal;
    fTableView->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(fTableView->horizontalHeader(), SIGNAL(customContextMenuRequested(QPoint)), this,
            SLOT(tableViewHeaderContextMenuRequested(QPoint)));
    connect(fTableView->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(tableViewHeaderClicked(int)));
    connect(fTableView->horizontalHeader(), SIGNAL(sectionResized(int, int, int)), this, SLOT(tableViewHeaderResized(int,
            int, int)));
    connect(fTableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this,
            SLOT(selectionChanged(QItemSelection, QItemSelection)));
    connect(fTableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(tableViewContextMenuRequested(QPoint)));
    connect(ui->tblTotal->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(tblValueChanged(int)));
    fFilterWidget = nullptr;
    ui->tblTotal->setVisible(false);
    ui->tblView->resizeRowsToContents();
    QShortcut *s = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Enter), this);
    connect(s, SIGNAL(activated()), this, SLOT(ctrlEnter()));
    s = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Return), this);
    connect(s, SIGNAL(activated()), this, SLOT(ctrlEnter()));
    fFilterLineEdit = new C5LineEdit(widget());
    fFilterLineEdit->setPlaceholderText(tr("Use \"|\" for separator multiple values"));
    widget()->layout()->addWidget(new QLabel(tr("Search in table"), widget()));
    widget()->layout()->addWidget(fFilterLineEdit);
    widget()->setVisible(false);
    connect(fFilterLineEdit, &C5LineEdit::textChanged, [this](const QString &arg1) {
        fModel->setFilter(-1, arg1);
        sumColumnsData();
    });
    addAction(fReportActions, ":/res/refresh.png", tr("Refresh"), this, SLOT(buildQuery()));
    addAction(fReportActions, ":/res/print.png", tr("Print"), this, SLOT(print()));
    addAction(fReportActions, ":/res/excel.png", tr("Export to Excel"), this, SLOT(exportToExcel()));
}

C5Grid::~C5Grid()
{
    if (fFilterWidget) {
        delete fFilterWidget;
    }
    delete ui;
}

void C5Grid::setTableForUpdate(const QString &table, const QList<int> &columns)
{
    fModel->fTableForUpdate = table;
    fModel->fColumnsForUpdate = columns;
}

void C5Grid::postProcess()
{
    if (fFilterWidget) {
        fFilterWidget->restoreFilter(fFilterWidget);
        fWhereCondition = fFilterWidget->condition();
    }
    buildQuery();
}

void C5Grid::buildQuery()
{
    QString mainTable = fMainTable.mid(fMainTable.indexOf(QRegularExpression("\\b[a-z,A-Z]*$")), fMainTable.length() - 1);
    QStringList leftJoinTables;
    QMap<QString, QString> leftJoinTablesMap;
    QStringList groupFields;
    if (fSimpleQuery) {
    } else {
        fSqlQuery = "select ";
        bool first = true;
        foreach (QString s, fColumnsFields) {
            if (fColumnsVisible[s]) {
                if (first) {
                    first = false;
                } else {
                    fSqlQuery += ",";
                }
                fSqlQuery += s;
                int pos = s.indexOf(QRegularExpression("\\b[a-z,A-Z]*\\."));
                QString tableOfField = s.mid(pos, s.indexOf(".", pos) - pos);
                if (tableOfField != mainTable) {
                    insertJoinTable(leftJoinTables, leftJoinTablesMap, tableOfField, mainTable);
                }
                if (fColumnsGroup.contains(s)) {
                    pos = s.indexOf(" as");
                    s = s.mid(0, pos);
                    if (!groupFields.contains(s)) {
                        groupFields << s;
                    }
                }
            }
        }
        if (fFilterWidget) {
            fWhereCondition = fFilterWidget->condition();
        }
        int p = 0;
        QRegularExpression re("\\b[a-z]*\\.");
        re.setPatternOptions(QRegularExpression::PatternOption::DotMatchesEverythingOption);
        while (true) {
            QRegularExpressionMatch match = re.match(fWhereCondition, p);
            if (!match.hasMatch()) {
                break;
            }
            QString table = match.captured(0);
            p += match.capturedLength();
            table = table.remove(table.length() - 1, 1);
            if (table != mainTable) {
                insertJoinTable(leftJoinTables, leftJoinTablesMap, table, mainTable);
            }
        }
        fSqlQuery += " from " + fMainTable + " ";
        foreach (QString s, leftJoinTables) {
            fSqlQuery += s + " ";
        }
        if (!fWhereCondition.isEmpty()) {
            if (fWhereCondition.contains("where", Qt::CaseInsensitive)) {
                fSqlQuery += " and " + fWhereCondition;
            } else {
                fSqlQuery += " where " + fWhereCondition;
            }
        }
        if (groupFields.count() > 0) {
            first = true;
            fSqlQuery += " group by ";
            foreach (QString s, groupFields) {
                if (first) {
                    first = false;
                } else {
                    fSqlQuery += ",";
                }
                fSqlQuery += s;
            }
        }
        fSqlQuery += fOrderCondition;
        if (!fHavindCondition.isEmpty()) {
            fSqlQuery += fHavindCondition;
        }
    }
    fModel->translate(fTranslation);
    refreshData();
}

void C5Grid::buildQuery(const QString &query)
{
    fSqlQuery = query;
    buildQuery();
}

void C5Grid::setFilter(int column, const QString &filter)
{
    fModel->setFilter(column, filter);
}

void C5Grid::setCheckboxes(bool v)
{
    fCheckboxes = v;
}

void C5Grid::hkFind()
{
    widget()->setVisible(true);
    fFilterLineEdit->setFocus();
    fFilterLineEdit->setSelection(0, fFilterLineEdit->text().length());
}

void C5Grid::hkEscape()
{
    fFilterLineEdit->clear();
    widget()->setVisible(false);
}

QLabel *C5Grid::lbFilter()
{
    return ui->lbFilter;
}

QWidget *C5Grid::widget()
{
    return ui->wd;
}

QHBoxLayout *C5Grid::hl()
{
    return ui->hl;
}

int C5Grid::rowId()
{
    QModelIndexList ml = fTableView->selectionModel()->selectedIndexes();
    if (ml.count() == 0) {
        return 0;
    }
    return fModel->data(ml.at(0).row(), 0, Qt::EditRole).toInt();
}

int C5Grid::rowId(int column)
{
    QModelIndexList ml = fTableView->selectionModel()->selectedIndexes();
    if (ml.count() == 0) {
        C5Message::info(tr("Nothing was selected"));
        return 0;
    }
    return fModel->data(ml.at(0).row(), column, Qt::EditRole).toInt();
}

int C5Grid::rowId(int &row, int column)
{
    QModelIndexList ml = fTableView->selectionModel()->selectedIndexes();
    if (ml.count() == 0) {
        C5Message::info(tr("Nothing was selected"));
        return 0;
    }
    row = ml.at(0).row();
    return fModel->data(row, column, Qt::EditRole).toInt();
}

void C5Grid::cellClicked(const QModelIndex &index)
{
    emit tblSingleClick(index);
}

void C5Grid::callEditor(const QString &id)
{
    Q_UNUSED(id);
}

void C5Grid::removeWithId(int id, int row)
{
    Q_UNUSED(id);
    Q_UNUSED(row);
}

void C5Grid::sumColumnsData()
{
    if (fColumnsSum.count() == 0) {
        return;
    }
    QMap<QString, double> values;
    fModel->sumForColumns(fColumnsSum, values);
    for (QMap<QString, double>::const_iterator it = values.begin(); it != values.end(); it++) {
        int idx = fModel->indexForColumnName(it.key());
        double value = it.value();
        ui->tblTotal->setData(0, idx, value);
    }
    QStringList vheader;
    vheader << QString::number(fModel->rowCount());
    ui->tblTotal->setVerticalHeaderLabels(vheader);
    ui->tblTotal->setVisible(true);
}

void C5Grid::restoreColumnsVisibility()
{
    QSettings s(_ORGANIZATION_, QString("%1\\%2\\reports\\%3\\visiblecolumns")
                .arg(_APPLICATION_)
                .arg(_MODULE_)
                .arg(fLabel));
    for (QMap<QString, bool>::iterator it = fColumnsVisible.begin(); it != fColumnsVisible.end(); it++) {
        if (s.contains(it.key())) {
            it.value() = s.value(it.key()).toBool();
        }
    }
}

void C5Grid::restoreColumnsWidths()
{
    QSettings s(_ORGANIZATION_, QString("%1\\%2\\reports\\%3\\columnswidth")
                .arg(_APPLICATION_)
                .arg(_MODULE_)
                .arg(fLabel));
    ui->tblTotal->setColumnCount(fModel->columnCount());
    for (int i = 0; i < ui->tblTotal->columnCount(); i++) {
        ui->tblTotal->setItem(0, i, new C5TableWidgetItem());
        QString colName = fModel->nameForColumnIndex(i);
        if (colName.isEmpty()) {
            qDebug() << "WARNING colName is Empty!";
            continue;
        }
        QString fullColName = colName;
        for (QMap<QString, bool>::const_iterator it = fColumnsVisible.begin(); it != fColumnsVisible.end(); it++) {
            QString c = it.key().toLower();
            if (c.contains(" as ")) {
                int pos = c.indexOf(" as ");
                if (fullColName == c.mid(pos + 4, c.length() - pos)) {
                    fullColName = c;
                    break;
                }
            } else if (c.contains(".")) {
                int pos = c.indexOf(".");
                if (fullColName == c.mid(pos + 1, c.length() - pos)) {
                    fullColName = c;
                    break;
                }
            } else if (c == fullColName) {
                fullColName = c;
                break;
            }
        }
        if (!fColumnsVisible[fullColName] || ui->tblView->columnWidth(i) == 0) {
            ui->tblView->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Interactive);
            //continue; why this row was commented, i dont know
        }
        if (s.contains(colName)) {
            ui->tblView->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Interactive);
            int w = s.value(colName).toInt();
            ui->tblView->setColumnWidth(i, w);
        } else {
            ui->tblView->horizontalHeader()->setSectionResizeMode(i, QHeaderView::ResizeToContents);
            //qApp->processEvents();
            ui->tblView->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Interactive);
        }
    }
}

QString C5Grid::reportAdditionalTitle()
{
    return "";
}

QMenu *C5Grid::buildTableViewContextMenu(const QPoint &point)
{
    QModelIndex index = fTableView->indexAt(point);
    QMenu *m = new QMenu(this);
    if (index.row() > -1 && index.column() > -1) {
        fFilterString = fModel->data(index, Qt::DisplayRole).toString();
        fFilterIndex = index;
        m->addAction(QIcon(":/filter_set.png"), QString("%1 '%2'").arg(tr("Filter")).arg(fFilterString), this,
                     SLOT(filterByStringAndIndex()));
    }
    m->addAction(QIcon(":/copy.png"), tr("Copy selection"), this, SLOT(copySelection()));
    m->addAction(QIcon(":/copy.png"), tr("Copy all"), this, SLOT(copyAll()));
    return m;
}

bool C5Grid::tblDoubleClicked(int row, int column, const QList<QVariant> &values)
{
    emit tblDoubleClick(row, column, values);
    if (values.count() > 0) {
        callEditor(values.at(0).toString());
    }
    return false;
}

void C5Grid::buildReport(const QString &name)
{
    fDb[":fid"] = name;
    fDb.exec("select fquery from sql_query where fid=:fid");
    if (fDb.nextRow()) {
        fSimpleQuery = true;
        fSqlQuery = fDb.getString("fquery");
    }
    buildQuery();
}

void C5Grid::insertNewRow(WdbWidget *w)
{
    int id = DbDialog::createDialog(w);
    if (id > 0) {
        QString q = fSqlQuery + " where " + w->tableName() + ".fid=" + QString::number(id);
        C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
        db.exec(q);
        if (db.nextRow()) {
            fModel->addRowValues(db.row());
            int colorColumn = fColorColumn.isEmpty() ? -1 : fModel->indexForColumnName(fColorColumn);
            if (colorColumn > -1) {
                setColor(fModel->rowCount() - 1, colorColumn);
            }
            postRefreshData(fModel->rowCount() - 1);
        }
    }
    w->deleteLater();
}

void C5Grid::updateRow(WdbWidget *w)
{
    int r = 0, c = 0;
    int id = rowId(r, c);
    id = DbDialog::createDialog(w, id);
    QString q = fSqlQuery + " where " + w->tableName() + ".fid=" + QString::number(id);
    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
    db.exec(q);
    if (db.nextRow()) {
        fModel->replaceRowValues(r, db.row());
        int colorColumn = fColorColumn.isEmpty() ? -1 : fModel->indexForColumnName(fColorColumn);
        if (colorColumn > -1) {
            setColor(r, colorColumn);
        }
        postRefreshData(r);
    }
}

void C5Grid::postRefreshData(int row)
{
    Q_UNUSED(row);
}

void C5Grid::setColor(int row, int colorColumn)
{
    if (row < 0) {
        for (int i = 0; i < fModel->rowCount(); i++) {
            fModel->setRowColor(i, QColor::fromRgb(fModel->data(i, colorColumn, Qt::EditRole).toInt()));
        }
    } else {
        fModel->setRowColor(row, QColor::fromRgb(fModel->data(row, colorColumn, Qt::EditRole).toInt()));
    }
    fTableView->setColumnWidth(colorColumn, 0);
    fTableTotal->setColumnWidth(colorColumn, 0);
}

void C5Grid::insertJoinTable(QStringList &joins, QMap<QString, QString> &joinsMap, const QString &table,
                             const QString &mainTable)
{
    QString j;
    for (int i = 0; i < fLeftJoinTables.count(); i++) {
        QString tmpJoinTable = fLeftJoinTables.at(i);
        int pos = tmpJoinTable.indexOf(QRegularExpression("\\[.*\\]"));
        tmpJoinTable = tmpJoinTable.mid(pos, tmpJoinTable.length() - pos);
        tmpJoinTable.replace("[", "").replace("]", "");
        if (tmpJoinTable == table) {
            if (!joinsMap.contains(tmpJoinTable)) {
                j = fLeftJoinTables.at(i).mid(0, fLeftJoinTables.at(i).length() - (fLeftJoinTables.at(i).length() - fLeftJoinTables.at(
                                                  i).indexOf(" [")));
                QRegularExpression rx("=.*$");
                QRegularExpressionMatch match = rx.match(j, 0);
                QString otherTable = match.captured(0).trimmed();
                otherTable.remove(0, 1);
                otherTable = otherTable.mid(0, otherTable.indexOf(".", 0));
                if (otherTable != mainTable && !otherTable.isEmpty()) {
                    if (!joinsMap.contains(otherTable)) {
                        insertJoinTable(joins, joinsMap, otherTable, mainTable);
                    }
                }
                joinsMap[tmpJoinTable] = j;
                joins << joinsMap[tmpJoinTable];
            }
            break;
        }
    }
}

int C5Grid::sumOfColumnsWidghtBefore(int column)
{
    int sum = 0;
    for (int i = 0; i < column; i++) {
        sum += fTableView->columnWidth(i);
    }
    return sum;
}

void C5Grid::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected);
    Q_UNUSED(deselected);
}

void C5Grid::copySelection()
{
    QModelIndexList sel = fTableView->selectionModel()->selectedIndexes();
    if (sel.count() == 0) {
        return;
    }
    QString data;
    int currCol = -1;
    bool first = true;
    foreach (QModelIndex m, sel) {
        if (currCol < 0) {
            currCol = m.row();
        }
        if (currCol != m.row()) {
            currCol = m.row();
            first = true;
            data += "\r\n";
        }
        if (first) {
            first = false;
        } else {
            data += "\t";
        }
        data += m.data(Qt::DisplayRole).toString();
    }
    QClipboard *c = qApp->clipboard();
    c->setText(data);
}

void C5Grid::copyAll()
{
    QString data;
    for (int i = 0; i < fModel->columnCount(); i++) {
        data += fModel->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString();
        if (i < fModel->columnCount() - 1) {
            data += "\t";
        } else {
            data += "\r\n";
        }
    }
    for (int r = 0, rc = fModel->rowCount(); r < rc; r++) {
        for (int c = 0, cc = fModel->columnCount(); c < cc; c++) {
            data += fModel->data(r, c, Qt::DisplayRole).toString();
            if (c == cc - 1) {
                data += "\r\n";
            } else {
                data += "\t";
            }
        }
    }
    if (ui->tblTotal->isVisible()) {
        for (int i = 0; i < ui->tblTotal->columnCount(); i++) {
            data += ui->tblTotal->getString(0, i);
            if (i < fModel->columnCount() - 1) {
                data += "\t";
            } else {
                data += "\r\n";
            }
        }
    }
    QClipboard *c = qApp->clipboard();
    c->setText(data);
}

void C5Grid::filterByColumn()
{
    QSet<QString> filterValues;
    fModel->uniqueValuesForColumn(fFilterColumn, filterValues);
    QStringList sortedValues = filterValues.values();
    std::sort(sortedValues.begin(), sortedValues.end());
    if (C5FilterValues::filterValues(sortedValues)) {
        fModel->setFilter(fFilterColumn, sortedValues.join("|"));
    }
    sumColumnsData();
}

void C5Grid::filterByStringAndIndex()
{
    if (fFilterString.isEmpty()) {
        return;
    }
    fModel->setFilter(fFilterIndex.column(), fFilterString);
    fFilterString.clear();
    sumColumnsData();
}

void C5Grid::removeFilterForColumn()
{
    fModel->removeFilter(fFilterColumn);
}

void C5Grid::tblValueChanged(int pos)
{
    fTableView->horizontalScrollBar()->setValue(pos);
}

bool C5Grid::currentRow(int &row)
{
    row = -1;
    QModelIndexList ml = ui->tblView->selectionModel()->selectedIndexes();
    if (ml.count() == 0) {
        C5Message::info(tr("Nothing was selected"));
        return false;
    }
    row = ml.at(0).row();
    return true;
}

void C5Grid::editRow(int columnWidthId)
{
    int row = 0;
    if (!currentRow(row)) {
        return;
    }
    callEditor(fModel->data(row, columnWidthId, Qt::EditRole).toString());
}

void C5Grid::removeRow(int columnWithId)
{
    int row = 0;
    if (!currentRow(row)) {
        return;
    }
    removeWithId(fModel->data(row, columnWithId, Qt::EditRole).toInt(), row);
}

void C5Grid::print()
{
    QFont font(qApp->font());
    font.setPointSize(20);
    C5Printing p;
    QSize paperSize(2000, 2800);
    p.setFont(font);
    int page = p.currentPageIndex();
    int startFrom = 0;
    bool stopped = false;
    int columnsWidth = 0;
    qreal scaleFactor = 0.40;
    qreal rowScaleFactor = 0.79;
    for (int i = 0; i < fModel->columnCount(); i++) {
        columnsWidth += fTableView->columnWidth(i);
    }
    columnsWidth /= scaleFactor;
    if (columnsWidth > 2000) {
        p.setSceneParams(paperSize.height(), paperSize.width(), QPageLayout::Landscape);
    } else {
        p.setSceneParams(paperSize.width(), paperSize.height(), QPageLayout::Portrait);
    }
    do {
        p.setFontBold(true);
        p.ltext(QString("%1 %2")
                .arg(fLabel)
                .arg(reportAdditionalTitle()), 0);
        p.br();
        if (fFilterWidget) {
            p.ltext(fFilterWidget->conditionText(), 0);
            p.br();
        }
        p.setFontBold(false);
        p.line(0, p.fTop, columnsWidth, p.fTop);
        for (int c = 0; c < fModel->columnCount(); c++) {
            if (fTableView->columnWidth(c) == 0) {
                continue;
            }
            if (c > 0) {
                p.ltext(fModel->headerData(c, Qt::Horizontal, Qt::DisplayRole).toString(),
                        (sumOfColumnsWidghtBefore(c) / scaleFactor) + 1);
                p.line(sumOfColumnsWidghtBefore(c) / scaleFactor, p.fTop, sumOfColumnsWidghtBefore(c) / scaleFactor,
                       p.fTop + (fTableView->verticalHeader()->defaultSectionSize() / rowScaleFactor));
            } else {
                p.ltext(fModel->headerData(c, Qt::Horizontal, Qt::DisplayRole).toString(), 1);
                p.line(0, p.fTop, 0, p.fTop + (fTableView->verticalHeader()->defaultSectionSize() / rowScaleFactor));
            }
        }
        p.line(columnsWidth, p.fTop, columnsWidth,
               p.fTop + (fTableView->verticalHeader()->defaultSectionSize() / rowScaleFactor));
        p.br();
        p.line(0, p.fTop, columnsWidth, p.fTop);
        for (int r = startFrom; r < fModel->rowCount(); r++) {
            p.line(0, p.fTop, columnsWidth, p.fTop);
            for (int c = 0; c < fModel->columnCount(); c++) {
                if (fTableView->columnWidth(c) == 0) {
                    continue;
                }
                int s = fTableView->columnSpan(r, c) - 1;
                if (c > 0) {
                    p.ltext(fModel->data(r, c, Qt::DisplayRole).toString(), (sumOfColumnsWidghtBefore(c) / scaleFactor) + 1);
                    p.line(sumOfColumnsWidghtBefore(c + s) / scaleFactor, p.fTop, sumOfColumnsWidghtBefore(c + s) / scaleFactor,
                           p.fTop + (fTableView->rowHeight(r) / rowScaleFactor));
                } else {
                    p.ltext(fModel->data(r, c, Qt::DisplayRole).toString(), 1);
                    p.line(0, p.fTop, 0, p.fTop + (fTableView->rowHeight(r) / rowScaleFactor));
                }
                c += s;
            }
            //last vertical line
            p.line(columnsWidth, p.fTop, columnsWidth, p.fTop + (fTableView->rowHeight(r) / rowScaleFactor));
            if (ui->tblTotal->isVisible() && r == fModel->rowCount() - 1) {
                p.setFontBold(true);
                if (p.checkBr(ui->tblTotal->rowHeight(0))) {
                    p.line(0, p.fTop + (ui->tblTotal->rowHeight(0) / rowScaleFactor), columnsWidth,
                           p.fTop + (ui->tblTotal->rowHeight(0) / rowScaleFactor));
                    \
                    p.br();
                    stopped = startFrom >= fModel->rowCount() - 1;
                    p.fTop = p.fNormalHeight - p.fLineHeight;
                    p.ltext(QString("%1 %2, %3 %4, %5/%6")
                            .arg("Page")
                            .arg(page + 1)
                            .arg(tr("Printed"))
                            .arg(QDateTime::currentDateTime().toString(FORMAT_DATETIME_TO_STR2))
                            .arg(hostinfo)
                            .arg(hostuser), 0);
                    p.rtext("ArmPetrol");
                    page++;
                } else {
                    p.br();
                }
                p.line(0, p.fTop, columnsWidth, p.fTop);
                p.line(0, p.fTop + (ui->tblTotal->rowHeight(0) / rowScaleFactor), columnsWidth,
                       p.fTop + (ui->tblTotal->rowHeight(0) / rowScaleFactor));
                \
                for (int c = 0; c < fModel->columnCount(); c++) {
                    if (fTableView->columnWidth(c) == 0) {
                        continue;
                    }
                    if (c > 0) {
                        p.ltext(ui->tblTotal->getString(0, c), (sumOfColumnsWidghtBefore(c) / scaleFactor) + 1);
                        p.line(sumOfColumnsWidghtBefore(c) / scaleFactor, p.fTop, sumOfColumnsWidghtBefore(c) / scaleFactor,
                               p.fTop + (fTableView->verticalHeader()->defaultSectionSize() / rowScaleFactor));
                    } else {
                        p.ltext(ui->tblTotal->getString(0, c), 1);
                        p.line(0, p.fTop, 0, p.fTop + (fTableView->verticalHeader()->defaultSectionSize() / rowScaleFactor));
                    }
                }
                p.line(columnsWidth, p.fTop, columnsWidth, p.fTop + (ui->tblTotal->rowHeight(0) / rowScaleFactor));
            }
            if (p.checkBr(p.fLineHeight * 4) || r >= fModel->rowCount() - 1) {
                p.line(0, p.fTop + (fTableView->rowHeight(r) / rowScaleFactor), columnsWidth,
                       p.fTop + (fTableView->rowHeight(r) / rowScaleFactor));
                \
                p.br();
                startFrom = r + 1;
                stopped = startFrom >= fModel->rowCount() - 1;
                p.fTop = p.fNormalHeight - p.fLineHeight;
                p.ltext(QString("%1 %2, %3 %4, %5/%6")
                        .arg("Page")
                        .arg(page + 1)
                        .arg(tr("Printed"))
                        .arg(QDateTime::currentDateTime().toString(FORMAT_DATETIME_TO_STR2))
                        .arg(hostinfo)
                        .arg(hostuser), 0);
                p.rtext("ArmPetrol");
                if (r < fModel->rowCount() - 1) {
                    p.br(p.fLineHeight * 4);
                }
                if (r < fModel->rowCount() - 1) {
                    page++;
                }
                break;
            } else {
                p.br();
            }
        }
        if (fModel->rowCount() == 0) {
            stopped = true;
        }
    } while (!stopped);
    C5PrintPreview pp( &p, this);
    pp.exec();
}

void C5Grid::exportToExcel()
{
    int colCount = fModel->columnCount();
    int rowCount = fModel->rowCount();
    if (colCount == 0 || rowCount == 0) {
        C5Message::info(tr("Empty report!"));
        return;
    }
    QXlsx::Document d;
    d.addSheet("Sheet1");
    /* HEADER */
    QColor color = QColor::fromRgb(200, 200, 250);
    QFont headerFont(qApp->font());
    headerFont.setBold(true);
    QXlsx::Format hf;
    hf.setFont(headerFont);
    hf.setPatternBackgroundColor( color);
    for (int i = 0; i < colCount; i++) {
        d.write(1, i + 1, fModel->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString(), hf);
        d.setColumnWidth(i + 1, fTableView->columnWidth(i) / 7);
    }
    /* BODY */
    QMap<int, QString> bgFill;
    QFont bodyFont(qApp->font());
    QXlsx::Format bf;
    bf.setFont( bodyFont);
    bgFill[QColor(Qt::white).rgb()] = "body";
    for (int j = 0; j < rowCount; j++) {
        for (int i = 0; i < colCount; i++) {
            d.write(j + 2, i + 1, fModel->data(j, i, Qt::EditRole), bf);
        }
    }
    /* MERGE cells */
    QMap<int, QList<int> > skiprow, skipcol;
    for (int r = 0; r < rowCount; r++) {
        for (int c = 0; c < colCount; c++) {
            if (fTableView->columnSpan(r, c) > 1 || fTableView->rowSpan(r, c) > 1) {
                int rs = -1, cs = -1;
                if (fTableView->columnSpan(r, c) > 1 && skipcol[r].contains(c) == false) {
                    cs = fTableView->columnSpan(r, c) - 1;
                    for (int i = c + 1; i < c + cs + 1; i++) {
                        skipcol[r].append(i);
                    }
                }
                if (fTableView->rowSpan(r, c) > 1 && skiprow[c].contains(r) == false) {
                    rs = fTableView->rowSpan(r, c) - 1;
                    for (int i = r + 1; i < r + rs + 1; i++) {
                        skiprow[c].append(i);
                    }
                }
                if (rs == -1 && cs == -1) {
                    continue;
                }
                rs = rs < 0 ? 0 : rs;
                cs = cs < 0 ? 0 : cs;
                d.mergeCells(QString("%1%2:%3:%4")
                             .arg(columnNumberToLetter(c + 1))
                             .arg(r + 2)
                             .arg(c + 1 + cs)
                             .arg(r + 2 + rs));
            }
        }
    }
    /* TOTALS ROWS */
    if (ui->tblTotal->isVisible()) {
        QFont totalFont(qApp->font());
        totalFont.setBold(true);
        bf.setFont(totalFont);
        bf.setPatternBackgroundColor(color);
        for (int i = 0; i < colCount; i++) {
            d.write(1 + fModel->rowCount() + 1, i + 1, ui->tblTotal->getData(0, i), bf);
        }
    }
    QString filename = QFileDialog::getSaveFileName(nullptr, "", "", "*.xlsx");
    if (filename.isEmpty()) {
        return;
    }
    d.saveAs(filename);
    QDesktopServices::openUrl(filename);
}

void C5Grid::clearFilter()
{
    if (fFilterWidget) {
        fFilterWidget->clearFilter(fFilterWidget);
        fFilterWidget->saveFilter(fFilterWidget);
    }
    fModel->clearFilter();
    sumColumnsData();
}

void C5Grid::setSearchParameters()
{
    if (fFilterWidget) {
        if (C5GridGilter::filter(fFilterWidget, fWhereCondition, fColumnsVisible, fTranslation)) {
            QSettings s(_ORGANIZATION_, QString("%1\\%2\\reports\\%3\\visiblecolumns")
                        .arg(_APPLICATION_)
                        .arg(_MODULE_)
                        .arg(fLabel));
            for (QMap<QString, bool>::const_iterator it = fColumnsVisible.begin(); it != fColumnsVisible.end(); it++) {
                s.setValue(it.key(), it.value());
            }
            buildQuery();
        }
    }
}

void C5Grid::resetSearchParameters()
{
    if (fFilterWidget) {
        fFilterWidget->clearFilter(fFilterWidget);
        buildQuery();
    }
}

void C5Grid::tableViewContextMenuRequested(const QPoint &point)
{
    QMenu *m = buildTableViewContextMenu(point);
    m->exec(fTableView->mapToGlobal(point));
    delete m;
}

void C5Grid::tableViewHeaderContextMenuRequested(const QPoint &point)
{
    fFilterColumn = fTableView->columnAt(point.x());
    QString colName = fModel->headerData(fFilterColumn, Qt::Horizontal, Qt::DisplayRole).toString();
    QMenu m;
    m.addAction(QIcon(":/filter_set.png"), QString("%1 '%2'").arg(tr("Set filter")).arg(colName), this,
                SLOT(filterByColumn()));
    m.addAction(QIcon(":/filter_clear.png"), QString("%1 '%2'").arg(tr("Remove filter")).arg(colName), this,
                SLOT(removeFilterForColumn()));
    m.addAction(QIcon(":/expand.png"), tr("Autofit columns widths"), this, SLOT(autofitColumns()));
    m.exec(fTableView->mapToGlobal(point));
}

void C5Grid::tableViewHeaderClicked(int index)
{
    fModel->sort(index);
}

void C5Grid::tableViewHeaderResized(int column, int oldSize, int newSize)
{
    Q_UNUSED(oldSize);
    ui->tblTotal->setColumnWidth(column, newSize);
    QString columnName = fModel->nameForColumnIndex(column);
    QSettings s(_ORGANIZATION_, QString("%1\\%2\\reports\\%3\\columnswidth")
                .arg(_APPLICATION_)
                .arg(_MODULE_)
                .arg(fLabel));
    s.setValue(columnName, newSize);
}

void C5Grid::ctrlEnter()
{
    QModelIndex m = ui->tblView->currentIndex();
    on_tblView_doubleClicked(m);
}

void C5Grid::autofitColumns()
{
    fTableView->resizeColumnsToContents();
}

void C5Grid::saveDataChanges()
{
    fTableView->selectionModel()->clear();
    fModel->saveDataChanges();
    fTableView->viewport()->update();
}

void C5Grid::refreshData()
{
    QString sqlQuery = fSqlQuery;
    if (fSimpleQuery) {
        if (fFilterWidget) {
            fWhereCondition = fFilterWidget->condition();
            ui->lbFilter->setVisible(true);
            ui->lbFilter->setText(fFilterWidget->conditionText());
        } else {
            ui->lbFilter->clear();
            ui->lbFilter->setVisible(false);
        }
        if (!fWhereCondition.isEmpty()) {
            if (fSqlQuery.contains("%where%")) {
                sqlQuery.replace("%where%", fWhereCondition);
            } else if (fSqlQuery.contains("where")) {
                if (fWhereCondition.contains("where")) {
                    fWhereCondition.replace("where", " ");
                }
                sqlQuery += " and " + fWhereCondition;
            } else {
                sqlQuery += fWhereCondition;
            }
        } else {
            if (fSqlQuery.contains("%where%")) {
                sqlQuery.replace("%where%", "");
            }
        }
        if (!fGroupCondition.isEmpty()) {
            sqlQuery += fGroupCondition;
        }
        if (!fOrderCondition.isEmpty()) {
            sqlQuery += fOrderCondition;
        }
    }
    if (fFilterWidget) {
        for (QMap<QString, QString>::const_iterator ir = fFilterWidget->replaces().constBegin();
                ir != fFilterWidget->replaces().constEnd(); ir++) {
            sqlQuery.replace(ir.key(), ir.value());
        }
    }
    fModel->setCheckboxes(fCheckboxes);
    fModel->execQuery(sqlQuery);
    if (fSimpleQuery) {
        for (int i = 0; i < fModel->columnCount(); i++) {
            fColumnsVisible[fModel->fColumnIndexName[i]] = true;
        }
    }
    restoreColumnsWidths();
    sumColumnsData();
    if (!ui->tblTotal->isVisible()) {
        ui->tblView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    }
    int colorColumn = fColorColumn.isEmpty() ? -1 : fModel->indexForColumnName(fColorColumn);
    if (colorColumn > -1) {
        setColor(-1, colorColumn);
    }
    foreach (const QString &s, fHiddenColumns) {
        fTableTotal->setColumnWidth(fModel->indexForColumnName(s), 0);
        fTableView->setColumnWidth(fModel->indexForColumnName(s), 0);
    }
    postRefreshData();
}

void C5Grid::on_tblView_clicked(const QModelIndex &index)
{
    cellClicked(index);
}

bool C5Grid::on_tblView_doubleClicked(const QModelIndex &index)
{
    if (index.row() < 0 || index.column() < 0) {
        return false;
    }
    QList<QVariant> values = fModel->getRowValues(index.row());
    if (tblDoubleClicked(index.row(), index.column(), values)) {
        return false;
    }
    return true;
}
