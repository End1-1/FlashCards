#-------------------------------------------------
#
# Project created by QtCreator 2020-03-24T10:51:47
#
#-------------------------------------------------

QT       += widgets core gui sql network printsupport

RESOURCES += \
    res.qrc

TARGET = APCards
TEMPLATE = app

CONFIG += c++20

RC_FILE = res.rc
ICON = res/fuel.ico

QMAKE_CXXFLAGS += -H

DEFINES += _DBDRIVER_=\\\"QMARIADB\\\"
DEFINES += _ORGANIZATION_=\\\"ArmPetrol\\\"
DEFINES += _APPLICATION_=\\\"ArmPetrolCards\\\"
DEFINES += _MODULE_=\\\"ArmPetrolCards\\\"

include(C:/projects/QXlsx/QXlsx/QXlsx.pri)

message(Include paths: $$INCLUDEPATH)

FORMS += \
    autoinputdoc.ui \
    c5connection.ui \
    c5filterautoinputflash.ui \
    c5filtervalues.ui \
    c5grid.ui \
    c5gridgilter.ui \
    c5message.ui \
    c5printpreview.ui \
    c5selector.ui \
    cardranges.ui \
    dbdialog.ui \
    dlgdebtpayment.ui \
    dlgfuelprices.ui \
    dlgpassword.ui \
    downloaddialog.ui \
    filteralltickets.ui \
    filteralltickets2.ui \
    filteramount1.ui \
    filteramount2.ui \
    filterdebts.ui \
    filterdebtshistory.ui \
    filterdocs.ui \
    filterflashfuelstore.ui \
    filterfuelflash.ui \
    filterfuelinout.ui \
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
    wdtamount3filter.ui \
    wdthome.ui \
    wdtrangemovementfilter.ui \
    wdttotalreportfilter.ui

DISTFILES += \
    FlashCards.qm

TRANSLATIONS += \
    FlashCards.ts

HEADERS += \
    C5Database.h \
    autoinputdoc.h \
    c5checkbox.h \
    c5combobox.h \
    c5connection.h \
    c5dateedit.h \
    c5filterautoinputflash.h \
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
    cardranges.h \
    config.h \
    dbdialog.h \
    dialog.h \
    dlgdebtpayment.h \
    dlgfuelprices.h \
    dlgpassword.h \
    doctype.h \
    download.h \
    downloaddialog.h \
    filteralltickets.h \
    filteralltickets2.h \
    filteramount1.h \
    filteramount2.h \
    filterdebts.h \
    filterdebtshistory.h \
    filterdocs.h \
    filterflashfuelstore.h \
    filterfuelflash.h \
    filterfuelinout.h \
    filterpartner1.h \
    filterpartner2.h \
    filterpartner3.h \
    filterticketsbyone.h \
    fuelflashmove.h \
    homewindows.h \
    httprequest.h \
    keyvaluelineedit.h \
    login.h \
    moveconsts.h \
    passticketsdialog.h \
    rc.h \
    registerticketdialog.h \
    registeruseddialog.h \
    wdbfuel.h \
    wdbfuelflash.h \
    wdbpartner.h \
    wdbtickettype.h \
    wdbwidget.h \
    wdtalltickets.h \
    wdtalltickets2.h \
    wdtamount1.h \
    wdtamount2.h \
    wdtamount3.h \
    wdtamount3filter.h \
    wdtcardtypes.h \
    wdtcontainer.h \
    wdtdebts.h \
    wdtdebtshistory.h \
    wdtflashautoinput.h \
    wdtflashfuelstore.h \
    wdtfuel.h \
    wdtfuelflash.h \
    wdtfuelflashmove.h \
    wdtfueloutin.h \
    wdtfuelprices.h \
    wdthome.h \
    wdticketsbyone.h \
    wdtpartners.h \
    wdtrangemovement.h \
    wdtrangemovementfilter.h \
    wdtreppartners1.h \
    wdtreppartners2.h \
    wdtreppartners3.h \
    wdtsaledocs.h \
    wdttotalreport.h \
    wdttotalreportfilter.h \
    wdtwidget.h

SOURCES += \
    autoinputdoc.cpp \
    c5checkbox.cpp \
    c5combobox.cpp \
    c5connection.cpp \
    c5database.cpp \
    c5dateedit.cpp \
    c5filterautoinputflash.cpp \
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
    cardranges.cpp \
    config.cpp \
    dbdialog.cpp \
    dialog.cpp \
    dlgdebtpayment.cpp \
    dlgfuelprices.cpp \
    dlgpassword.cpp \
    download.cpp \
    downloaddialog.cpp \
    filteralltickets.cpp \
    filteralltickets2.cpp \
    filteramount1.cpp \
    filteramount2.cpp \
    filterdebts.cpp \
    filterdebtshistory.cpp \
    filterdocs.cpp \
    filterflashfuelstore.cpp \
    filterfuelflash.cpp \
    filterfuelinout.cpp \
    filterpartner1.cpp \
    filterpartner2.cpp \
    filterpartner3.cpp \
    filterticketsbyone.cpp \
    fuelflashmove.cpp \
    homewindows.cpp \
    httprequest.cpp \
    keyvaluelineedit.cpp \
    login.cpp \
    main.cpp \
    moveconsts.cpp \
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
    wdtamount3.cpp \
    wdtamount3filter.cpp \
    wdtcardtypes.cpp \
    wdtcontainer.cpp \
    wdtdebts.cpp \
    wdtdebtshistory.cpp \
    wdtflashautoinput.cpp \
    wdtflashfuelstore.cpp \
    wdtfuel.cpp \
    wdtfuelflash.cpp \
    wdtfuelflashmove.cpp \
    wdtfueloutin.cpp \
    wdtfuelprices.cpp \
    wdthome.cpp \
    wdticketsbyone.cpp \
    wdtpartners.cpp \
    wdtrangemovement.cpp \
    wdtrangemovementfilter.cpp \
    wdtreppartners1.cpp \
    wdtreppartners2.cpp \
    wdtreppartners3.cpp \
    wdtsaledocs.cpp \
    wdttotalreport.cpp \
    wdttotalreportfilter.cpp \
    wdtwidget.cpp
