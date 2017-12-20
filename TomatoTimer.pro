TEMPLATE = app

QT       += core gui
CONFIG   += c++14

macx {
    QMAKE_MAC_SDK = MacOSX10.13.sdk
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.13
}

QMAKE_MAC_SDK_PATH = "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.13.sdk"


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TomatoTimer

ICON = "resources/icon.icns"

SOURCES += main.cpp \
        mainwindow.cpp

HEADERS  += mainwindow.h \
    notifications/notifications.h
    systemsounds/systemsounds.h

FORMS    += mainwindow.ui

TRANSLATIONS = languages/en.ts

RESOURCES += \
    res.qrc

mac {
    LIBS += -framework Foundation -framework Cocoa
    QMAKE_INFO_PLIST = mac/Info.plist

    OBJECTIVE_SOURCES += \
        notifications/notifications_osx.mm \
        systemsounds/systemsounds_osx.mm
}

