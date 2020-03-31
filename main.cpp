#include "homewindows.h"
#include "config.h"
#include "c5connection.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator t;
    t.load(":/FlashCards.qm");
    a.installTranslator(&t);
    QFont font = a.font();
    font.setPointSize(12);
    a.setFont(font);
    QStringList params;
    C5Connection::readParams(params);
    if (params.count() > 3) {
        __dbhost = params.at(0);
        __dbschema = params.at(1);
        __dbusername = params.at(2);
        __dbpassword = params.at(3);
    }
    HomeWindows w;
    __parent = &w;
    w.showMaximized();
    a.processEvents();
    w.login();

    return a.exec();
}
