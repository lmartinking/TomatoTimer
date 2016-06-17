#-------------------------------------------------
#
# Project created by QtCreator 2016-06-15T13:04:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TomatoTimer
TEMPLATE = app

ICON = "resources/icon.icns"

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    notifications/notifications.h

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc

mac {
    LIBS += -framework Foundation -framework Cocoa
    QMAKE_INFO_PLIST = mac/Info.plist

    OBJECTIVE_SOURCES += \
        notifications/notifications_osx.mm
}

