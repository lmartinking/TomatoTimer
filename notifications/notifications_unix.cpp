#include <QUuid>
#include <QDebug>

#include "notifications.h"



QUuid ShowNotification(const QString& title, const QString& subtitle, const QString& message,
                       const QString& actionButton, const QString& otherButton,
                       const QString& contentImagePath, const QString& indentityImagePath)
{
    qDebug() << "ShowNotification" << title << " => " << subtitle << " " << message;

    return QUuid();
}
