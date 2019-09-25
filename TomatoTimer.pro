TEMPLATE = app

QT       += core gui widgets
CONFIG   += c++14

macx {
    QMAKE_MAC_SDK = MacOSX10.13.sdk
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.8
    QMAKE_MAC_SDK_PATH = "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.13.sdk"
}

TARGET = TomatoTimer

ICON = "resources/icon.icns"

SOURCES += main.cpp \
        mainwindow.cpp \
        pomodoro.cpp

HEADERS  += mainwindow.h \
    notifications/notifications.h \
    pomodoro.h \
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

linux {
    SOURCES += systemsounds/systemsounds_unix.cpp
    SOURCES += notifications/notifications_unix.cpp
}
