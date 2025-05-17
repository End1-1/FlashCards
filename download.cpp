#include "download.h"
#include "httprequest.h"
#include "c5database.h"
#include "config.h"
#include "moveconsts.h"
#include <QDir>
#include <QThread>

Download::Download(QObject *parent) : QObject(parent)
{
}

void Download::downloadData()
{
    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
    db.exec("select fid, fname, furl from download_servers");
    while (db.nextRow()) {
        emit newServer(db.getString("fname"));
        auto *hr = new HttpRequest(db.getString("furl"), SLOT(parseResponse(bool, QString)), this);
        hr->setProperty("fid", db.getInt("fid"));
        hr->setProperty("fname", db.getString("fname"));
        hr->setProperty("furl", db.getString("furl"));
        hr->getRequest();
    }
}

void Download::parseResponse(bool error, const QString &str)
{
    HttpRequest *ha = static_cast<HttpRequest *>(sender());
    C5Database::logEvent("Starting " + ha->property("furl").toString().toUtf8(), ha->property("fname").toString());
    if (error) {
        ha->deleteLater();
        return;
    }
    QRegularExpression r("\\d+-\\d+\\.xls");
    QStringList list;
    QRegularExpressionMatchIterator it = r.globalMatch(str);
    while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();
        list.append(match.captured(0));
    }
    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
    for (const QString &s : list) {
        int t1 = s.mid(0, 10).toInt();
        int t2 = s.mid(11, 10).toInt();
        QDateTime dt1 = QDateTime::fromSecsSinceEpoch(t1);
        QDateTime dt2 = QDateTime::fromSecsSinceEpoch(t2);
        db[":ffile"] = s;
        db[":fstation"] = ha->property("fid");
        db.exec("select fid from autoinput_flash where ffile=:ffile and fstation=:fstation");
        if (db.nextRow()) {
            C5Database::logEvent("Skipping: " + s + " " + ha->property("fid").toString() + "\r\n",
                                 ha->property("fname").toString());
            continue;
        } else {
            db[":fstation"] = ha->property("fid");
            db[":fdate"] = dt1.date();
            db[":ftime"] = dt1.time();
            db[":fcreated_date"] = QDate::currentDate();
            db[":fcreated_time"] = QTime::currentTime();
            db[":fstate"] = 1;
            db[":fflag"] = 0;
            db[":ffile"] = s;
            db.insert("autoinput_flash");
            C5Database::logEvent("Not skipping: " + s + "\r\n", ha->property("fname").toString());
        }
    }
    db[":fstation"] = ha->property("fid");
    db.exec("select * from autoinput_flash where fstation=:fstation and fflag=0 limit 1");
    if (db.nextRow()) {
        C5Database::logEvent("First: " + ha->property("furl").toString() + db.getString("ffile") + "\r\n",
                             ha->property("fname").toString());
        auto *hr = new HttpRequest(ha->property("furl").toString() + db.getString("ffile"), SLOT(parseFile(bool, QString)),
                                   this);
        hr->setProperty("fid", ha->property("fid"));
        hr->setProperty("fname", ha->property("fname"));
        hr->setProperty("ffile", db.getString("ffile"));
        hr->setProperty("fau", db.getInt("fid"));
        hr->setProperty("fdate", db.getDate("fdate"));
        hr->setProperty("furl", ha->property("furl"));
        hr->getRequest();
    } else {
        C5Database::logEvent("No records for: " + ha->property("fname").toString() + "\r\n", ha->property("fname").toString());
    }
    ha->deleteLater();
}

void Download::parseFile(bool error, const QString &str)
{
    HttpRequest *ha = static_cast<HttpRequest *>(sender());
    C5Database::logEvent("Parse file: " + ha->property("ffile").toString() + "\r\n", ha->property("fname").toString());
    C5Database::logEvent("Data: " + str + "\r\n", ha->property("fname").toString());
    C5Database db(__dbhost, __dbschema, __dbusername, __dbpassword);
    if (error) {
        C5Database::logEvent("Error: " + str + "\r\n", ha->property("fname").toString());
    } else {
        db.startTransaction();
        QMap<QString, int> fuel;
        db.exec("select * from fuel_flash");
        while (db.nextRow()) {
            fuel[db.getString("fname2")] = db.getInt("fid");
        }
        QStringList rows = str.split("\r\n", Qt::SkipEmptyParts);
        for (const QString &r : qAsConst(rows)) {
            QStringList cols = r.split(" ", Qt::SkipEmptyParts);
            if (cols.at(3).toDouble() > 0.001 || cols.at(2).toDouble() > 0.001) {
                db[":fau"] = ha->property("fau");
                db[":fstate"] = MOVE_INPUT_DRAFT;
                db[":ffuel"] = fuel[cols.at(1)];
                db[":fdate"] = ha->property("fdate");
                db[":fpartner"] = 0;
                db[":fpaymenttype"] = 1;
                db[":fsign"] = 1;
                db[":fqty"] = cols.at(3).toDouble();
                db[":fqty2"] = cols.at(2).toDouble();
                db[":fprice"] = 0;
                db[":ftotal"] = 0;
                db.insert("fuel_move");
            }
        }
        db[":fstation"] = ha->property("fid");
        db[":ffile"] = ha->property("ffile");
        db.exec("update autoinput_flash set fflag=1 where fstation=:fstation and ffile=:ffile");
        db.commit();
    }
    //Next
    db[":fstation"] = ha->property("fid");
    db.exec("select * from autoinput_flash where fstation=:fstation and fflag=0 limit 1");
    if (db.nextRow()) {
        C5Database::logEvent("Next record: " + ha->property("furl").toString() + db.getString("ffile") + "\r\n",
                             ha->property("fname").toString());
        auto *hr = new HttpRequest(ha->property("furl").toString() + db.getString("ffile"), SLOT(parseFile(bool, QString)),
                                   this);
        hr->setProperty("fid", ha->property("fid"));
        hr->setProperty("fname", ha->property("fname"));
        hr->setProperty("ffile", db.getString("ffile"));
        hr->setProperty("fau", db.getInt("fid"));
        hr->setProperty("fdate", db.getDate("fdate"));
        hr->setProperty("furl", ha->property("furl"));
        hr->getRequest();
    } else {
        C5Database::logEvent("End of records: " + ha->property("fname").toString() + "\r\n", ha->property("fname").toString());
    }
    ha->deleteLater();
}
