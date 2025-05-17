#include "wdttotalreport.h"
#include "wdttotalreportfilter.h"
#include "c5tablemodel.h"

WdtTotalReport::WdtTotalReport(const QIcon &icon, const QString &label, QWidget *parent) :
    C5Grid(icon, label, parent)
{
    fFilterWidget = new WdtTotalReportFilter();
    fFilterWidget->restoreFilter(fFilterWidget);
    fFilter = static_cast<WdtTotalReportFilter*>(fFilterWidget);
    addAction(fReportActions, ":/res/filter.png", tr("Filter"), this, SLOT(setSearchParameters()));
    buildQuery();
}

void WdtTotalReport::buildQuery()
{
    fModel->clearModel();
    QStringList header;
    header.append("Օր");
    header.append("Գործընկեր");
    header.append("Վառելիքի տեսակ");
    header.append("Վաճառք/լիտր");
    header.append("Գին");
    header.append("Զեղչ");
    header.append("Գումար");
    header.append("Սպառում/լիտր");
    header.append("Մնացորդ/լիտր");
    header.append("Կանխիկ/փոխանցում");
    header.append("Պարտք");
    header.append("Ֆլեշ/Արմ պետրոլ");
    for (int i = 0; i < header.count(); i++) {
        fModel->insertColumn(i, header.at(i));
    }

    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
    QString tempTable = "t_" + db.uuid().replace("-", "");
    db.startTransaction();
    db.exec(QString("create temporary table %1 "
            "(fdate date, "
            "fpartnerid int, fpartner text, "
            "ffuelid int, ffuel text, "
            "fsale decimal(14,2), fprice decimal(14,2), fdiscount decimal(14,2),"
            "fexit decimal(14,2), fremain decimal(14,2), "
            "fpayment text, fdebt decimal(14,2), farmflash text)").arg(tempTable));
    db.exec("SELECT P.FID, p.fname , f.fid, f.fname "
            "FROM partners p, fuel f");

    QString mainTableSql;
    int counter = 0;
//    while (db.nextRow()) {
//        counter++;
//        if (counter > 500 && !mainTableSql.isEmpty()) {
//            db.exec(mainTableSql);
//            mainTableSql.clear();
//        }
//        for (QDate d = fFilter->d1(); d < fFilter->d2().addDays(1); d = d.addDays(1)) {
//            if (mainTableSql.isEmpty()) {
//                mainTableSql = "insert into " + tempTable + " (fdate, fpartnerid, fpartner, ffuelid, ffuel, fsale, "
//                            "fprice, fdiscount, fexit, fdebt, fremain) values ";
//            } else {
//                mainTableSql += ",";
//            }
//            mainTableSql += QString("('%1', %2, '%3', %4, '%5', 0, 0, 0, 0, 0, 0)")
//                    .arg(d.toString(FORMAT_DATE_TO_STR_MYSQL))
//                    .arg(db.getInt(0))
//                    .arg(db.getString(1))
//                    .arg(db.getInt(2))
//                    .arg(db.getString(3));
//        }
//    }
//    if (!mainTableSql.isEmpty()) {
//        db.exec(mainTableSql);
//    }

    //ARMPETROL sale
    db[":fdate1"] = fFilter->d1();
    db[":fdate2"] = fFilter->d2();
    db.exec("select d.fdatecreate, c.fpartner, ct.ffuel, sum(ct.fmeas) as fqty, c.fprice, c.fpricediscount, "
            "p.fname as fpaymentname, pn.fname as fpartnername, fn.fname as ffuelname "
            "from cards c "
            "left join cards_types ct on ct.fid=c.ftype "
            "left join docs d on d.ftransaction=c.fpartnertrans "
            "left join payment_types p on p.fid=d.fpaymenttype "
            "left join partners pn on pn.fid=c.fpartner "
            "left join fuel fn on fn.fid=ct.ffuel "
            "where d.fdatecreate between :fdate1 and :fdate2 "
            "group by 1,2,3,6");
    QStringList sqls;
    while (db.nextRow()) {
        QString sql = "insert into " + tempTable + " (fdate, fpartnerid, fpartner, ffuelid, ffuel, fsale, "
                        "fprice, fdiscount, fexit, fdebt, fremain, fpayment, farmflash) values ";
        sql += QString("('%1', %2, '%3', %4, '%5', %6, %7, %8, 0, 0, 0, '%9', '%10')")
                .arg(db.getDate("fdatecreate").toString(FORMAT_DATE_TO_STR_MYSQL))
                .arg(db.getInt("fpartner"))
                .arg(db.getString("fpartnername"))
                .arg(db.getInt("ffuel"))
                .arg(db.getString("ffuelname"))
                .arg(db.getDouble("fqty"))
                .arg(db.getDouble("fprice") + db.getDouble("fpricediscount"))
                .arg(db.getDouble("fpricediscount"))
                .arg(db.getString("fpaymentname"))
                .arg("ԱրմՊետրոլ");
        sqls += sql;

    }
    for (const QString &s: sqls) {
        db.exec(s);
    }

    //flash sale
    db[":fdate1"] = fFilter->d1();
    db[":fdate2"] = fFilter->d2();
    db.exec("select fm.fdate, fm.fpartner, fm.ffuel, sum(fm.fqty) as fqty, fm.fprice, fm.fdiscount, "
            "p.fname as fpaymentname, pn.fname as fpartnername, fn.fname as ffuelname "
            "from fuel_move fm "
            "left join payment_types p on p.fid=fm.fpaymenttype "
            "inner join partners pn on pn.fid=fm.fpartner "
            "left join fuel fn on fn.fid=fm.ffuel "
            "where fm.fdate between :fdate1 and :fdate2 "
            "group by 1,2,3");
    sqls.clear();
    while (db.nextRow()) {
        QString sql = "insert into " + tempTable + " (fdate, fpartnerid, fpartner, ffuelid, ffuel, fsale, "
                        "fprice, fdiscount, fexit, fdebt, fremain, fpayment, farmflash) values ";
        sql += QString("('%1', %2, '%3', %4, '%5', %6, %7, %8, 0, 0, 0, '%9', '%10')")
                .arg(db.getDate("fdatecreate").toString(FORMAT_DATE_TO_STR_MYSQL))
                .arg(db.getInt("fpartner"))
                .arg(db.getString("fpartnername"))
                .arg(db.getInt("ffuel"))
                .arg(db.getString("ffuelname"))
                .arg(db.getDouble("fqty"))
                .arg(db.getDouble("fprice"))
                .arg(db.getDouble("fdiscount"))
                .arg(db.getString("fpaymentname"))
                .arg("Ֆլեշ");
        sqls += sql;

    }
    for (const QString &s: sqls) {
        db.exec(s);
    }

    //ARMPETROL back
    db[":fdate1"] = fFilter->d1();
    db[":fdate2"] = fFilter->d2();
    db.exec("select d.fdatecreate, c.fpartner, ct.ffuel, sum(ct.fmeas) as fqty, c.fprice, c.fpricediscount, "
            "p.fname as fpaymentname, pn.fname as fpartnername, fn.fname as ffuelname "
            "from cards c "
            "left join cards_types ct on ct.fid=c.ftype "
            "left join docs d on d.ftransaction=c.fusedtrans "
            "left join payment_types p on p.fid=d.fpaymenttype "
            "left join partners pn on pn.fid=c.fpartner "
            "left join fuel fn on fn.fid=ct.ffuel "
            "where d.fdatecreate between :fdate1 and :fdate2 "
            "group by 1,2,3");
    sqls.clear();
    while (db.nextRow()) {
        QString sql = "insert into " + tempTable + " (fdate, fpartnerid, fpartner, ffuelid, ffuel, fsale, "
                        "fprice, fdiscount, fexit, fdebt, fremain, fpayment, farmflash) values ";
        sql += QString("('%1', %2, '%3', %4, '%5', %6, %7, %8, %11, 0, 0, '%9', '%10')")
                .arg(db.getDate("fdatecreate").toString(FORMAT_DATE_TO_STR_MYSQL))
                .arg(db.getInt("fpartner"))
                .arg(db.getString("fpartnername"))
                .arg(db.getInt("ffuel"))
                .arg(db.getString("ffuelname"))
                .arg(0)
                .arg(0)
                .arg(0)
                .arg(db.getString("fpaymentname"))
                .arg("ԱրմՊետրոլ")
                .arg(db.getDouble("fqty"));
        sqls += sql;

    }
    for (const QString &s: sqls) {
        //db.exec(s);
    }


//    db[":fdate1"] = fFilter->d1();
//    db[":fdate2"] = fFilter->d2();
//    db.exec("select d.fdatecreate, c.fpartner, ct.ffuel, sum(ct.fmeas) as fqty "
//            "from cards c "
//            "left join cards_types ct on ct.fid=c.ftype "
//            "left join docs d on d.ftransaction=c.fusedtrans "
//            "where d.fdatecreate between :fdate1 and :fdate2 "
//            "group by 1,2,3");
//    sqls.clear();
//    while (db.nextRow()) {
//        sqls.append(QString("update %1 set fexit=%2 "
//                            "where fdate='%3' and fpartnerid=%4 and ffuelid=%5")
//                    .arg(tempTable)
//                    .arg(db.getDouble("fqty"))
//                    .arg(db.getDate("fdatecreate").toString(FORMAT_DATE_TO_STR_MYSQL))
//                    .arg(db.getInt("fpartner"))
//                    .arg(db.getInt("ffuel")));
//    }
//    for (const QString &s: sqls) {
//        db.exec(s);
//    }


    //REMAIN
//    db[":fdate"] = fFilter->d2();
//    db.exec("SELECT c1.fpartner, ct1.ffuel, SUM(ct1.fmeas) AS fsale "
//            "FROM cards c1 "
//            "LEFT JOIN cards_types ct1 ON ct1.fid=c1.ftype "
//            "INNER JOIN docs d ON d.ftransaction=c1.fpartnertrans "
//            "WHERE d.fdatecreate<=:fdate "
//            "GROUP BY 1,2 "
//            "union "
//            "SELECT c1.fpartner, ct1.ffuel, SUM(ct1.fmeas)*-1 AS fsale "
//            "FROM cards c1 "
//            "LEFT JOIN cards_types ct1 ON ct1.fid=c1.ftype "
//            "INNER JOIN docs d ON d.ftransaction=c1.fusedtrans "
//            "WHERE d.fdatecreate<=:fdate "
//            "GROUP BY 1,2 ");
//    sqls.clear();
//    while (db.nextRow()) {
//        sqls.append(QString("update %1 set fremain=fremain+(%2) where fpartnerid=%3 and ffuelid=%4 and fdate='%5'")
//                    .arg(tempTable)
//                    .arg(db.getDouble("fsale"))
//                    .arg(db.getInt("fpartner"))
//                    .arg(db.getInt("ffuel"))
//                    .arg(fFilter->d2().toString(FORMAT_DATE_TO_STR_MYSQL)));
//    }
//    for (const QString &s: sqls) {
//        db.exec(s);
//    }

    //DEBTS
//    db[":fdate"] = fFilter->d2();
//    db.exec("select d.fpartner, sum(fdebet-fcredit) as fdebt "
//            "from debts d "
//            "where fdate<=:fdate "
//            "group by 1 ");
//    sqls.clear();
//    while (db.nextRow()) {
//        sqls.append(QString("update %1 set fdebt=%2 where fpartner=%3 and fdate='%4'")
//                    .arg(tempTable)
//                    .arg(db.getDouble("fdebt"))
//                    .arg(db.getInt("fpartner"))
//                    .arg(fFilter->d2().toString(FORMAT_DATE_TO_STR_MYSQL)));
//    }
//    for (const QString &s: sqls) {
//        db.exec(s);
//    }

//    db.exec(QString("delete from %1 where (fsale=0 and fexit=0 and fremain=0 and fdebt=0").arg(tempTable));
    fColumnsSum.append(QString("Վաճառք").toLower());
    fColumnsSum.append(QString("Ընդամենը").toLower());
    fModel->execQuery(db, "select fdate as `Ամսաթիվ`, fpartner as `Գործընկեր`, ffuel as `Վառելիք`, "
                        "sum(fsale) as `Վաճառք`, fprice as `Գին`, fdiscount as `Զեղչ`, "
                      "sum(fsale*(fprice-fdiscount)) as `Ընդամենը`,  fpayment as `Վճարում`, farmflash as `Առմ/Ֆլեշ` "
                      "from " + tempTable +
                      " group by 1, 2, 3, 6, 8, 9 "
                      "order by 1, 2, 3 ");
    fTableView->resizeColumnsToContents();
    sumColumnsData();
}
