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
		NOTIFICATION_CLICKED,
		ACTION_BUTTON_CLICKED,
	};

public:
	NotificationEvent() : QEvent(NOTIFICATION_EVENT), type(NONE) {};
	NotificationEvent(QUuid uuid, ActionType typ) : QEvent(NOTIFICATION_EVENT), id(uuid), type(typ) {};

	virtual ~NotificationEvent();

public:
	QUuid   id;
	ActionType	type;

	struct {
		QString title;
		QString subtitle;
		QString message;

		QString actionButton;
		QString otherButton;
	} context;
};

QUuid ShowNotification(const char* title, const char* subTitle, const char* message,
					  const char* actionButton = nullptr, const char* otherButton = nullptr,
					  const char* contentImagePath = nullptr,
					  const char* indentityImagePath = nullptr);

#endif // NOTIFICATIONS_H
