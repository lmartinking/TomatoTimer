#include <QUuid>
#include <QDBusInterface>
#include <QDBusReply>
#include <QDebug>

#include "notifications.h"


QUuid ShowNotification(const QString& title, const QString& subtitle, const QString& message,
                       const QString& actionButton, const QString& otherButton,
                       const QString& contentImagePath, const QString& indentityImagePath)
{
    qDebug() << "ShowNotification" << title << " => " << subtitle << " " << message;
    qDebug() << "content img:" << contentImagePath;
    qDebug() << "identity img:" << indentityImagePath;

    static QDBusInterface notifications("org.freedesktop.Notifications", "/org/freedesktop/Notifications", "org.freedesktop.Notifications", QDBusConnection::sessionBus());
    if (notifications.isValid()) {

        QStringList actions;
        QMap<QString, QVariant> hints;

        QDBusReply<uint> reply = notifications.call("Notify",
            "TomatoTimer",  // App name
            (uint) 0,       // Replaces ID
            "",             // App Icon
            title,
            message,
            actions,
            hints,
            -1              // Timeout
            );

        if (! reply.isValid()) {
            qDebug() << "Call error:" << reply.error().message();
        }
    } else {
        qDebug() << "Err??" << QDBusConnection::sessionBus().lastError().message();
    }

    return QUuid();
}
