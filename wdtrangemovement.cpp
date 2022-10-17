#include "wdtrangemovement.h"
#include "wdtrangemovementfilter.h"
#include "c5tablemodel.h"
#include "c5tablewidget.h"
#include <QLabel>
#include <QHeaderView>

WdtRangeMovement::WdtRangeMovement(const QIcon &icon, const QString &label, QWidget *parent) :
    C5Grid(icon, label, parent)
{
    fFilterWidget = new WdtRangeMovementFilter();
    fFilterWidget->restoreFilter(fFilterWidget);
    fFilter = static_cast<WdtRangeMovementFilter*>(fFilterWidget);
    addAction(fReportActions, ":/res/filter.png", tr("Filter"), this, SLOT(setSearchParameters()));
    buildQuery();
}

void WdtRangeMovement::buildQuery()
{
    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
    db.exec("SELECT p.fid AS partnerid, p.fname AS partnername, f.fid fuelid, "
            "f.fname AS fuelname "
            "FROM partners p, fuel f "
            "order by p.fname ");
    fModel->clearModel();
    int row = 0;
    QStringList header;
    header.append(tr("Partner code"));
    header.append(tr("Partner"));
    header.append(tr("Fuel code"));
    header.append(tr("Fuel"));
    header.append("+5");
    header.append("+10");
    header.append("+20");
    header.append("+40");
    header.append("-5");
    header.append("-10");
    header.append("-20");
    header.append("-40");
    header.append(tr("Remain"));
    header.append(tr("Real remain"));
    header.append(tr("Debt"));
    for (int i = 0; i < header.count(); i++) {
        fModel->insertColumn(i, header.at(i));
    }
    QMap<int, QMap<int, QMap<int, int> > > partnerFuel;
    QMap<int, int> partnerFirstRow;
    while (db.nextRow()) {
        fModel->insertRow(row);
        for (int i = 0; i < db.columnCount(); i++) {
            fModel->setData(row, i, db.getValue(i));
        }
        if (!partnerFirstRow.contains(db.getValue("partnerid").toInt())) {
            partnerFirstRow[db.getValue("partnerid").toInt()] = row;
        }
        partnerFuel[db.getValue("partnerid").toInt()][db.getValue("fuelid").toInt()][5] = row;
        partnerFuel[db.getValue("partnerid").toInt()][db.getValue("fuelid").toInt()][10] = row;
        partnerFuel[db.getValue("partnerid").toInt()][db.getValue("fuelid").toInt()][20] = row;
        partnerFuel[db.getValue("partnerid").toInt()][db.getValue("fuelid").toInt()][40] = row;
        row++;
    }
    db[":fdate1"] = fFilter->date1();
    db[":fdate2"] = fFilter->date2();
    db.exec("SELECT c.fpartner, c.ftype, ct.fmeas, ct.ffuel  "
            "FROM cards c "
            "LEFT JOIN cards_types ct ON ct.fid=c.ftype "
            "WHERE c.fpartnerdate BETWEEN :fdate1 AND :fdate2 ");
    QMap<int, int> fuelColumn;
    fuelColumn[5] = 4;
    fuelColumn[10] = 5;
    fuelColumn[20] = 6;
    fuelColumn[40] = 7;
    while (db.nextRow()) {
        int r = partnerFuel[db.getValue("fpartner").toInt()][db.getValue("ffuel").toInt()][db.getValue("fmeas").toInt()];
        int column = fuelColumn[db.getValue("fmeas").toInt()];
        fModel->setData(r, column, fModel->data(r, column, Qt::EditRole).toDouble() + db.getValue("fmeas").toInt());
    }
    db[":fdate1"] = fFilter->date1();
    db[":fdate2"] = fFilter->date2();
    db.exec("SELECT c.fpartner, c.ftype, ct.fmeas, ct.ffuel  "
            "FROM cards c "
            "LEFT JOIN cards_types ct ON ct.fid=c.ftype "
            "WHERE c.frefunddate BETWEEN :fdate1 AND :fdate2 "
            "and c.fpartnerdate BETWEEN :fdate1 AND :fdate2 ");
    while (db.nextRow()) {
        int r = partnerFuel[db.getValue("fpartner").toInt()][db.getValue("ffuel").toInt()][db.getValue("fmeas").toInt()];
        int column = fuelColumn[db.getValue("fmeas").toInt()] + 4;
        fModel->setData(r, column, fModel->data(r, column, Qt::EditRole).toDouble() + db.getValue("fmeas").toInt());
    }
    for (int i = 0; i < fModel->rowCount(); i++) {
        fModel->setData(i, 12,
                        fModel->data(i, 4, Qt::EditRole).toInt()
                        + fModel->data(i, 5, Qt::EditRole).toInt()
                        + fModel->data(i, 6, Qt::EditRole).toInt()
                        + fModel->data(i, 7, Qt::EditRole).toInt()
                        - fModel->data(i, 8, Qt::EditRole).toInt()
                        - fModel->data(i, 9, Qt::EditRole).toInt()
                        - fModel->data(i, 10, Qt::EditRole).toInt()
                        - fModel->data(i, 11, Qt::EditRole).toInt());
    }
    db[":fdate1"] = fFilter->date1();
    db[":fdate2"] = fFilter->date2();
    switch (fFilter->mode()) {
    case 0:
        db.exec("select fpartner, sum(fdebet-fcredit) as balance "
                "from debts "
                "where fdate between :fdate1 and :fdate2 "
                "group by 1 "
                "HAVING   sum(fdebet-fcredit) <> 0 ");
        break;
    case 1:
        db.exec("select fpartner, sum(fdebet-fcredit) as balance "
                "from debts "
                "where fdate <= :fdate2 "
                "group by 1 "
                "HAVING   sum(fdebet-fcredit) <> 0 ");
        break;
    }


    while (db.nextRow()) {
        int r = partnerFirstRow[db.getInt("fpartner")];
        fModel->setData(r, 14, db.getDouble("balance"));
    }

    //FINAL REAL FUEL REMAIN
    QString sql = "SELECT distinct(p.fid) AS fpartnerid, f.fid as ffuelid, "
                    "COALESCE(c0.c0meas, 0)-COALESCE(c1.c1meas,0) AS `Սկզբնական քանակ`, coalesce(c0.c0sum, 0)-COALESCE(c1.c1sum, 0) AS `Սկզբնական գումար` ,"
                    "c2.c2meas AS `Քանակ մուտք`, c2.c2sum AS `Գումար մուտք`, "
                    "c3.c3meas AS `Քանակ ելք`, c3.c3sum AS `Գումար ելք`, "
                    "COALESCE(c0.c0meas, 0) - COALESCE(c1.c1meas, 0) + coalesce(c2.c2meas, 0) - COALESCE(c3.c3meas, 0) AS frealremainqty, "
                    "coalesce(c0.c0sum, 0) - COALESCE(c1.c1sum, 0) + COALESCE(c2.c2sum, 0) - COALESCE(c3.c3sum, 0) AS `Մնացորդ գումար` "

                    "from cards c "
                    "INNER join partners  p on p.fid=c.fpartner "
                    "inner join cards_types ct on ct.fid=c.ftype "
                    "inner join fuel f on f.fid=ct.ffuel "

                    "LEFT JOIN (SELECT c0.fpartner, ct0.ffuel, SUM(ct0.fmeas) AS c0meas, sum(ct0.fmeas*c0.fprice) AS c0sum "
                    "FROM cards c0 "
                    "INNER JOIN cards_types ct0 ON ct0.fid=c0.ftype "
                    "WHERE c0.fpartnerdate < %date1%  "
                    "GROUP BY 1, 2) c0 ON c0.fpartner=p.fid and c0.ffuel=f.fid "

                    "LEFT JOIN (SELECT c1.fpartner, ct1.ffuel, SUM(ct1.fmeas) AS c1meas, sum(ct1.fmeas*c1.fprice) AS c1sum "
                    "FROM cards c1 "
                    "INNER JOIN cards_types ct1 ON ct1.fid=c1.ftype "
                    "WHERE c1.frefunddate < %date1% "
                    "GROUP BY 1, 2) c1 ON c1.fpartner=p.fid and c1.ffuel=f.fid "

                    "LEFT JOIN (SELECT c2.fpartner, ct2.ffuel, SUM(ct2.fmeas) AS c2meas, sum(ct2.fmeas*c2.fprice) AS c2sum "
                    "FROM cards c2 "
                    "INNER JOIN cards_types ct2 ON ct2.fid=c2.ftype "
                    "WHERE c2.fpartnerdate BETWEEN %date1% AND %date2% "
                    "GROUP BY 1, 2) c2 ON c2.fpartner=p.fid  and c2.ffuel=f.fid "

                    "LEFT JOIN (SELECT c3.fpartner, ct3.ffuel, SUM(ct3.fmeas) AS c3meas, sum(ct3.fmeas*c3.fprice) AS c3sum "
                    "FROM cards c3 "
                    "INNER JOIN cards_types ct3 ON ct3.fid=c3.ftype "
                    "WHERE c3.frefunddate BETWEEN %date1% AND %date2% "
                    "GROUP BY 1, 2) c3 ON c3.fpartner=p.fid  and c3.ffuel=f.fid "

                    "where c0.c0meas>0 OR c1.c1meas>0 or c2.c2meas>0 OR c3.c3meas>0 ";
    QMap<QString, QString> m = fFilter->replaces();
    for (QMap<QString, QString>::const_iterator it = m.constBegin(); it != m.constEnd(); it++) {
        sql.replace(it.key(), it.value());
    }
    db.exec(sql);
    while (db.nextRow()) {
        int r = partnerFuel[db.getValue("fpartnerid").toInt()][db.getValue("ffuelid").toInt()][5];
        int c = 13;
        qDebug() << db.getValue("fpartnerid") << db.getValue("ffuelid").toInt() << db.getDouble("frealremainqty");
        fModel->setData(r, c, db.getDouble("frealremainqty"), Qt::EditRole);
    }

    // REMOVE EMPTY ROWS
    for (int i = fModel->rowCount() - 1; i > -1; i--) {
        bool empty = true;
        for (int j = 4; j < fModel->columnCount(); j++) {
            if (fModel->data(i, j, Qt::EditRole).toInt() != 0) {
                empty = false;
                break;
            }
        }
        if (empty) {
            fModel->removeRow(i);
        }
    }
    for (int i = fModel->rowCount() - 1; i > -1; i--) {
        bool empty = true;
        for (int j = 4; j < fModel->columnCount() - 2; j++) {
            if (fModel->data(i, j, Qt::EditRole).toInt() != 0) {
                empty = false;
                break;
            }
        }
        if (empty) {
            fModel->setData(i, 3, "");
        }
    }


    fTableView->resizeColumnsToContents();
    int rowStart = 0, partner = 0;
    fTableView->clearSpans();
    fModel->insertRow(0);
    fModel->insertRow(0);
    fTableView->setSpan(0, 4, 1, 4);
    fTableView->setSpan(0, 8, 1, 4);
    fTableView->setSpan(0, 0, 2, 2);
    fTableView->setSpan(0, 2, 2, 2);
    fTableView->setSpan(0, 12, 2, 1);
    fTableView->setSpan(0, 13, 2, 1);
    fTableView->setSpan(0, 14, 2, 1);
    fModel->setData(0, 0, tr("Partner"));
    fModel->setData(0, 2, tr("Fuel"));
    fModel->setData(0, 4, tr("Quantity, in"));
    fModel->setData(0, 8, tr("Quantity, out"));
    fModel->setData(0, 12, tr("Remain"));
    fModel->setData(0, 13, tr("Real remain"));
    fModel->setData(0, 14, tr("Balance"));
    fModel->setData(1, 4, tr("5"));
    fModel->setData(1, 5, tr("10"));
    fModel->setData(1, 6, tr("20"));
    fModel->setData(1, 7, tr("40"));
    fModel->setData(1, 8, tr("5"));
    fModel->setData(1, 9, tr("10"));
    fModel->setData(1, 10, tr("20"));
    fModel->setData(1, 11, tr("40"));
    for (int i = 0; i < fModel->rowCount(); i++) {
        if (partner != fModel->data(i, 0, Qt::EditRole).toInt()) {
            if (i > 0 && i - rowStart > 1) {
                fTableView->setSpan(rowStart, 14, i - rowStart, 1);
            }
            partner = fModel->data(i, 0, Qt::EditRole).toInt();
            rowStart = i;
        }
    }
    fTableView->setColumnHidden(0, true);
    fTableView->setColumnHidden(2, true);
    fTableTotal->setVisible(true);
    fTableTotal->setColumnCount(fModel->columnCount());
    for (int i = 4; i < fModel->columnCount(); i++) {
        double total = 0;
        for (int j = 0; j < fModel->rowCount(); j++) {
            total += fModel->data(j, i, Qt::EditRole).toDouble();
        }
        fTableTotal->setDouble(0, i, total);
    }
    fTableTotal->resizeColumnsToContents();
    for (int i = 4; i < fModel->columnCount(); i++) {
        fTableView->setColumnWidth(i, fTableTotal->columnWidth(i));
    }
    for (int i = 0; i < 4; i++) {
        fTableTotal->setColumnWidth(i, fTableView->columnWidth(i));
        fTableTotal->setData(0, i, "");
    }
    QStringList l;
    l.append(QString::number(fModel->rowCount()));
    fTableTotal->setVerticalHeaderLabels(l);
    lbFilter()->setText(fFilterWidget->conditionText());
}
