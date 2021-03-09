#-------------------------------------------------
#
# Project created by QtCreator 2020-03-24T10:51:47
#
#-------------------------------------------------

QT       += core gui sql network printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FlashCards
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

RC_FILE = res.rc
ICON = res/fuel.ico

SOURCES += \
        ../XLSX/src/xlsx.cpp \
        ../XLSX/src/xlsxcell.cpp \
        ../XLSX/src/xlsxcontenttype.cpp \
        ../XLSX/src/xlsxdocpropsapp.cpp \
        ../XLSX/src/xlsxdocpropscore.cpp \
        ../XLSX/src/xlsxdocument.cpp \
        ../XLSX/src/xlsxrels.cpp \
        ../XLSX/src/xlsxsharedstring.cpp \
        ../XLSX/src/xlsxsheet.cpp \
        ../XLSX/src/xlsxstyles.cpp \
        ../XLSX/src/xlsxtheme.cpp \
        ../XLSX/src/xlsxworkbook.cpp \
        ../XLSX/src/xlsxwriter.cpp \
        c5checkbox.cpp \
        c5combobox.cpp \
        c5connection.cpp \
        c5database.cpp \
        c5dateedit.cpp \
        c5filtervalues.cpp \
        c5filterwidget.cpp \
        c5graphicsview.cpp \
        c5grid.cpp \
        c5gridgilter.cpp \
        c5lineedit.cpp \
        c5message.cpp \
        c5printing.cpp \
        c5printpreview.cpp \
        c5selector.cpp \
        c5tablemodel.cpp \
        c5tableview.cpp \
        c5tablewidget.cpp \
        config.cpp \
        dbdialog.cpp \
        dialog.cpp \
        filteralltickets.cpp \
        filteralltickets2.cpp \
        filteramount1.cpp \
        filteramount2.cpp \
        filterpartner1.cpp \
        filterpartner2.cpp \
        filterpartner3.cpp \
        filterticketsbyone.cpp \
        fuelflashmove.cpp \
        homewindows.cpp \
        keyvaluelineedit.cpp \
        login.cpp \
        main.cpp \
        passticketsdialog.cpp \
        registerticketdialog.cpp \
        registeruseddialog.cpp \
        wdbfuel.cpp \
        wdbfuelflash.cpp \
        wdbpartner.cpp \
        wdbtickettype.cpp \
        wdbwidget.cpp \
        wdtalltickets.cpp \
        wdtalltickets2.cpp \
        wdtamount1.cpp \
        wdtamount2.cpp \
        wdtcardtypes.cpp \
        wdtcontainer.cpp \
        wdtfuel.cpp \
        wdtfuelflash.cpp \
        wdthome.cpp \
        wdticketsbyone.cpp \
        wdtpartners.cpp \
        wdtreppartners2.cpp \
        wdtreppartners1.cpp \
        wdtreppartners3.cpp \
        wdtwidget.cpp

HEADERS += \
    ../XLSX/src/crs32.h \
    ../XLSX/src/xlsx.h \
    ../XLSX/src/xlsxall.h \
    ../XLSX/src/xlsxcell.h \
    ../XLSX/src/xlsxcontenttype.h \
    ../XLSX/src/xlsxdocpropsapp.h \
    ../XLSX/src/xlsxdocpropscore.h \
    ../XLSX/src/xlsxdocument.h \
    ../XLSX/src/xlsxrels.h \
    ../XLSX/src/xlsxsharedstring.h \
    ../XLSX/src/xlsxsheet.h \
    ../XLSX/src/xlsxstyles.h \
    ../XLSX/src/xlsxtheme.h \
    ../XLSX/src/xlsxworkbook.h \
    ../XLSX/src/xlsxwriter.h \
    ../XLSX/src/zip.h \
    c5checkbox.h \
    c5combobox.h \
    c5connection.h \
    c5database.h \
    c5dateedit.h \
    c5filtervalues.h \
    c5filterwidget.h \
    c5graphicsview.h \
    c5grid.h \
    c5gridgilter.h \
    c5lineedit.h \
    c5message.h \
    c5printing.h \
    c5printpreview.h \
    c5selector.h \
    c5tablemodel.h \
    c5tableview.h \
    c5tablewidget.h \
    config.h \
    dbdialog.h \
    dialog.h \
    filteralltickets.h \
    filteralltickets2.h \
    filteramount1.h \
    filteramount2.h \
    filterpartner1.h \
    filterpartner2.h \
    filterpartner3.h \
    filterticketsbyone.h \
    fuelflashmove.h \
    homewindows.h \
    keyvaluelineedit.h \
    login.h \
    passticketsdialog.h \
    registerticketdialog.h \
    registeruseddialog.h \
    res.rc \
    wdbfuel.h \
    wdbfuelflash.h \
    wdbpartner.h \
    wdbtickettype.h \
    wdbwidget.h \
    wdtalltickets.h \
    wdtalltickets2.h \
    wdtamount1.h \
    wdtamount2.h \
    wdtcardtypes.h \
    wdtcontainer.h \
    wdtfuel.h \
    wdtfuelflash.h \
    wdthome.h \
    wdticketsbyone.h \
    wdtpartners.h \
    wdtreppartners2.h \
    wdtreppartners1.h \
    wdtreppartners3.h \
    wdtwidget.h

FORMS += \
    c5connection.ui \
    c5filtervalues.ui \
    c5grid.ui \
    c5gridgilter.ui \
    c5message.ui \
    c5printpreview.ui \
    c5selector.ui \
    dbdialog.ui \
    filteralltickets.ui \
    filteralltickets2.ui \
    filteramount1.ui \
    filteramount2.ui \
    filterpartner1.ui \
    filterpartner2.ui \
    filterpartner3.ui \
    filterticketsbyone.ui \
    fuelflashmove.ui \
    homewindows.ui \
    login.ui \
    passticketsdialog.ui \
    registerticketdialog.ui \
    registeruseddialog.ui \
    wdbfuel.ui \
    wdbfuelflash.ui \
    wdbpartner.ui \
    wdbtickettype.ui \
    wdthome.ui

DEFINES += _DBDRIVER_=\\\"QMYSQL\\\"
DEFINES += _ORGANIZATION_=\\\"Flash\\\"
DEFINES += _APPLICATION_=\\\"FlashCards\\\"
DEFINES += _MODULE_=\\\"FlashCards\\\"

RESOURCES += \
    res.qrc

INCLUDEPATH += c:/projects/xlsx/src
