#include "c5tablewidget.h"
#include "c5lineedit.h"
#include "c5combobox.h"
#include "c5message.h"
#include "c5checkbox.h"
#include "config.h"
#include <QHeaderView>
#include <QApplication>
#include <QDateTime>
#include <QFileDialog>
#include <QDesktopServices>
#include <QXlsx/header/xlsxdocument.h>

C5TableWidget::C5TableWidget(QWidget *parent) :
    QTableWidget(parent)
{
    setEditTriggers(NoEditTriggers);
}

C5TableWidgetItem *C5TableWidget::item(int row, int column) const
{
    return static_cast<C5TableWidgetItem *>(QTableWidget::item(row, column));
}

void C5TableWidget::setColumnWidths(int count, ...)
{
    setColumnCount(count);
    va_list vl;
    va_start(vl, count);
    for (int i = 0; i < count; i++) {
        setColumnWidth(i, va_arg(vl, int));
    }
    va_end(vl);
}

void C5TableWidget::fitColumnsToWidth(int dec)
{
    int colWidths = 0;
    int hiddenColumns = 0;
    for (int i = 0; i < columnCount(); i++) {
        if (columnWidth(i) == 0) {
            hiddenColumns++;
        }
        colWidths += columnWidth(i);
    }
    int freeSpace = width() - colWidths - dec;
    int delta = 0;
    if (columnCount() - hiddenColumns > 0) {
        delta = freeSpace / (columnCount() - hiddenColumns);
    }
    for (int i = 0; i < columnCount(); i++) {
        if (columnWidth(i) == 0) {
            continue;
        }
        setColumnWidth(i, columnWidth(i) + delta);
    }
}

void C5TableWidget::fitRowToHeight(int dec)
{
    int rh = verticalHeader()->defaultSectionSize();
    int visibleRows = (height() - dec) / rh;
    int delta = (height() - dec) - (visibleRows *rh);
    verticalHeader()->setDefaultSectionSize(rh + (delta / visibleRows));
}

C5LineEdit *C5TableWidget::createLineEdit(int row, int column)
{
    C5LineEdit *l = new C5LineEdit(this);
    l->setProperty("row", row);
    l->setProperty("column", column);
    l->setFrame(false);
    connect(l, SIGNAL(textChanged(QString)), this, SLOT(lineEditTextChanged(QString)));
    setCellWidget(row, column, l);
    return l;
}

C5LineEdit *C5TableWidget::lineEdit(int row, int column)
{
    return static_cast<C5LineEdit *>(cellWidget(row, column));
}

C5ComboBox *C5TableWidget::createComboBox(int row, int column)
{
    C5ComboBox *c = new C5ComboBox(this);
    c->setProperty("row", row);
    c->setProperty("column", column);
    c->setFrame(false);
    setCellWidget(row, column, c);
    connect(c, SIGNAL(currentTextChanged(QString)), this, SLOT(comboTextChanged(QString)));
    return c;
}

C5ComboBox *C5TableWidget::comboBox(int row, int column)
{
    return static_cast<C5ComboBox *>(cellWidget(row, column));
}

C5CheckBox *C5TableWidget::createCheckbox(int row, int column)
{
    C5CheckBox *c = new C5CheckBox(this);
    c->setProperty("row", row);
    c->setProperty("column", column);
    setCellWidget(row, column, c);
    connect(c, SIGNAL(clicked(bool)), this, SLOT(checkBoxChecked(bool)));
    return c;
}

C5CheckBox *C5TableWidget::checkBox(int row, int column)
{
    return static_cast<C5CheckBox *>(cellWidget(row, column));
}

bool C5TableWidget::findWidget(QWidget *w, int &row, int &column)
{
    for (int r = 0; r < rowCount(); r++) {
        for (int c = 0; c < columnCount(); c++) {
            if (cellWidget(r, c) == w) {
                row = r;
                column = c;
                return true;
            }
        }
    }
    return false;
}

QVariant C5TableWidget::getData(int row, int column)
{
    C5TableWidgetItem *i = item(row, column);
    return i->data(Qt::EditRole);
}

void C5TableWidget::setData(int row, int column, const QVariant &value)
{
    C5TableWidgetItem *i = item(row, column);
    if (!i) {
        i = new C5TableWidgetItem();
        QTableWidget::setItem(row, column, i);
        if (fColumnsDecimals.contains(column)) {
            i->fDecimals = fColumnsDecimals[column];
        }
    }
    i->setData(Qt::EditRole, value);
}

int C5TableWidget::getInteger(int row, int column)
{
    return getData(row, column).toInt();
}

void C5TableWidget::setInteger(int row, int column, int value)
{
    setData(row, column, value);
}

QString C5TableWidget::getString(int row, int column)
{
    C5TableWidgetItem *i = item(row, column);
    if (!i) {
        return "NO ITEM!";
    }
    return i->text();
}

void C5TableWidget::setString(int row, int column, const QString &str)
{
    setData(row, column, str);
}

double C5TableWidget::getDouble(int row, int column)
{
    return getData(row, column).toDouble();
}

void C5TableWidget::setDouble(int row, int column, double value)
{
    setData(row, column, value);
}

int C5TableWidget::addEmptyRow()
{
    int row = rowCount();
    setRowCount(row + 1);
    for (int i = 0; i < columnCount(); i++) {
        setItem(row, i, new C5TableWidgetItem());
    }
    return row;
}

void C5TableWidget::exportToExcel()
{
    if (columnCount() == 0 || rowCount() == 0) {
        C5Message::info(tr("Empty report"));
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
    for (int i = 0; i < columnCount(); i++) {
        d.write(1, i + 1, horizontalHeaderItem(i)->data(Qt::DisplayRole).toString(), hf);
        d.setColumnWidth(i + 1, columnWidth(i) / 7);
    }
    //e.setHorizontalAlignment(e.address(0, 0), e.address(0, colCount - 1), Excel::hCenter);
    /* BODY */
    QFont bodyFont(qApp->font());
    QXlsx::Format bf;
    bf.setFont( bodyFont);
    for (int j = 0; j < rowCount(); j++) {
        for (int i = 0; i < columnCount(); i++) {
            d.write(j + 2, i + 1,  item(j, i)->data(Qt::EditRole), bf);
        }
    }
    QString filename = QFileDialog::getSaveFileName(nullptr, "", "", "*.xlsx");
    if (filename.isEmpty()) {
        return;
    }
    d.saveAs(filename);
    QDesktopServices::openUrl(filename);
}

void C5TableWidget::search(const QString &txt)
{
    for (int i = 0; i < rowCount(); i++) {
        bool hidden = true;
        for (int j = 0; j < columnCount(); j++) {
            if (getString(i, j).contains(txt, Qt::CaseInsensitive)) {
                hidden = false;
            }
        }
        setRowHidden(i, hidden);
    }
}

double C5TableWidget::sumOfColumn(int column)
{
    double total = 0;
    for (int i = 0; i < rowCount(); i++) {
        total += getDouble(i, column);
    }
    return total;
}

void C5TableWidget::setColumnDecimals(int column, int decimals)
{
    fColumnsDecimals[column] = decimals;
}

void C5TableWidget::lineEditTextChanged(const QString arg1)
{
    C5LineEdit *l = static_cast<C5LineEdit *>(sender());
    setString(l->property("row").toInt(), l->property("column").toInt(), arg1);
}

void C5TableWidget::comboTextChanged(const QString &text)
{
    C5ComboBox *c = static_cast<C5ComboBox *>(sender());
    setString(c->property("row").toInt(), c->property("column").toInt(), text);
}

void C5TableWidget::checkBoxChecked(bool v)
{
    C5CheckBox *c = static_cast<C5CheckBox *>(sender());
    setString(c->property("row").toInt(), c->property("column").toInt(), (v ? "1" : "0"));
}

C5TableWidgetItem::C5TableWidgetItem(int type) :
    QTableWidgetItem (type)
{
    fDecimals = 2;
}

C5TableWidgetItem::C5TableWidgetItem(const QString &text, int type) :
    QTableWidgetItem (text, type)
{
}

QVariant C5TableWidgetItem::data(int role) const
{
    QVariant v = QTableWidgetItem::data(role);
    if (role == Qt::DisplayRole) {
        switch (v.type()) {
            case QVariant::Int:
                return v.toString();
            case QVariant::Date:
                return v.toDate().toString(FORMAT_DATE_TO_STR);
            case QVariant::DateTime:
                return v.toDateTime().toString(FORMAT_DATETIME_TO_STR);
            case QVariant::Time:
                return v.toTime().toString(FORMAT_TIME_TO_STR);
            case QVariant::Double:
                return float_str(v.toDouble(), fDecimals);
            default:
                return v.toString();
        }
    }
    return v;
}
