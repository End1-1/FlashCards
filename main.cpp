#include "homewindows.h"
#include "config.h"
#include "c5connection.h"
#include <QFile>
#include <QApplication>
#include <QTranslator>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("fusion"));
    QFile styleSheet("./styles.qss");
    if (styleSheet.exists()) {
        styleSheet.open(QIODevice::ReadOnly);
        a.setStyleSheet(styleSheet.readAll());
    }
    QTranslator t;
    if ( t.load(":/FlashCards.qm")) {
        a.installTranslator( &t);
    }
    QStringList params;
    C5Connection::readParams(params);
    if (params.count() > 3) {
        __dbhost = params.at(0);
        __dbschema = params.at(1);
        __dbusername = params.at(2);
        __dbpassword = params.at(3);
        __appFontSize = params.at(4).toInt() > 0 ? params.at(4).toInt() : __appFontSize;
    }
    QFont font = a.font();
    font.setPointSize(__appFontSize);
    a.setFont(font);
    HomeWindows w;
    __parent = &w;
    w.showMaximized();
    a.processEvents();
    w.login();
    return a.exec();
}
