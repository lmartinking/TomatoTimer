#ifndef NOTIFICATIONS_H
#define NOTIFICATIONS_H

#include <QEvent>
#include <QObject>
#include <QStringList>
#include <QUuid>

const QEvent::Type NOTIFICATION_EVENT = static_cast<QEvent::Type>(QEvent::User + 1);

class NotificationEvent : public QEvent
{
public:
	enum ActionType {
		NONE,
		NOTIFICATION_DELIVERED,
		NOTIFICATION_CLICKED,
		ACTION_BUTTON_CLICKED,
	};
	Q_ENUM(ActionType)

public:
	NotificationEvent();
	NotificationEvent(QUuid uuid, ActionType typ);

	virtual ~NotificationEvent();

public:
	QUuid		id;
	ActionType	type;

	struct {
		QString title;
		QString subtitle;
		QString message;

		QString actionButton;
		QString otherButton;
	} context;
};

QUuid ShowNotification(const QString& title, const QString& subtitle, const QString& message,
					   const QString& actionButton = "", const QString& otherButton = "",
					   const QString& contentImagePath = "", const QString& indentityImagePath = "");

#endif // NOTIFICATIONS_H
