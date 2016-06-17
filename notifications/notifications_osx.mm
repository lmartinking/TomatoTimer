#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

#include <QDebug>
#include <QFile>
#include <QByteArray>
#include <QtGlobal>
#include <QApplication>

#include "notifications.h"


NotificationEvent::~NotificationEvent()
{
}


@interface NotificationCenterDelegate : NSObject <NSUserNotificationCenterDelegate>
@end


QUuid ShowNotification(const char* title, const char* subtitle, const char* message,
					  const char* actionButton,
					  const char* otherButton,
					  const char* contentImagePath,
					  const char* indentityImagePath)
{
	static NotificationCenterDelegate* delegate = [[NotificationCenterDelegate alloc] init];
	static NSString *identifier = [NSString stringWithUTF8String:"com.irrationalidiom.TomatoTimer.Noficiation"];

	QUuid notification_id = QUuid::createUuid();

	NSUserNotification *notification = [[NSUserNotification alloc] init];
	notification.identifier = [NSString stringWithUTF8String:notification_id.toString().toUtf8().data()];

	// Content Image
	if (contentImagePath)
	{
		QFile img_file(contentImagePath);
		if (img_file.open(QFile::ReadOnly))
		{
			QByteArray img_file_data = img_file.readAll();
			qDebug() << "(Content) Image data size: " << img_file_data.size();
			NSData* image_data = [NSData dataWithBytes:(const void*)img_file_data.constData() length:img_file_data.size()];
			NSImage* image = [[NSImage alloc] initWithData:image_data];
			notification.contentImage = image;
		}
	}

	// NOTE: Private API
	if (indentityImagePath)
	{
		QFile img_file(indentityImagePath);
		if (img_file.open(QFile::ReadOnly))
		{
			QByteArray img_file_data = img_file.readAll();
			qDebug() << "(Identity) Image data size: " << img_file_data.size();
			NSData* image_data = [NSData dataWithBytes:(const void*)img_file_data.constData() length:img_file_data.size()];
			NSImage* image = [[NSImage alloc] initWithData:image_data];
			[notification setValue:image forKey:@"_identityImage"];
		}
	}

	if (title)
	{
		notification.title = [NSString stringWithUTF8String:title];
	}

	if (message)
	{
		notification.informativeText = [NSString stringWithUTF8String:message];
	}

	if (subtitle)
	{
		notification.subtitle = subtitle ? [NSString stringWithUTF8String:subtitle] : nil;
	}

	if (actionButton)
	{
		notification.hasActionButton = YES;
		notification.actionButtonTitle = [NSString stringWithUTF8String:actionButton];
	}

	if (otherButton)
	{
		notification.otherButtonTitle = [NSString stringWithUTF8String:otherButton];
	}

	notification.soundName = nil;

	// NOTE: Private API (Secret sauce!)
	[notification setValue:@YES forKey:@"_showsButtons"];

	NSUserNotificationCenter* unc = [NSUserNotificationCenter defaultUserNotificationCenter];
	unc.delegate = delegate;
	[unc deliverNotification:notification];

	qDebug() << "sent notification" << notification_id.toString();

	return notification_id;
}

/*
 * Notification Centre Delegate
 */

@implementation NotificationCenterDelegate

- (void)userNotificationCenter:(NSUserNotificationCenter *)center
		didDeliverNotification:(NSUserNotification *)notification
{
	qDebug() << "did deliver notification";
}

- (BOOL)userNotificationCenter:(NSUserNotificationCenter *)center
	 shouldPresentNotification:(NSUserNotification *)notification
{
	return YES;
}

- (void)userNotificationCenter:(NSUserNotificationCenter *)center didActivateNotification:(NSUserNotification *)notification
{
	QUuid uuid;
	if (notification.identifier)
	{
		uuid = QUuid(QString::fromNSString(notification.identifier));
	}

	qDebug() << "didActivateNotification " << notification.activationType << " " << uuid.toString();

	NotificationEvent::ActionType typ = NotificationEvent::NONE;

	if (notification.activationType == NSUserNotificationActivationTypeContentsClicked)
	{
		typ = NotificationEvent::NOTIFICATION_CLICKED;
	}
	else if (notification.activationType == NSUserNotificationActivationTypeActionButtonClicked)
	{
		typ = NotificationEvent::ACTION_BUTTON_CLICKED;
	}

	NotificationEvent* e = new NotificationEvent(uuid, typ);

	e->context.title = notification.title ? QString::fromNSString(notification.title) : "";
	e->context.subtitle = notification.subtitle ? QString::fromNSString(notification.subtitle) : "";
	e->context.message = notification.informativeText ? QString::fromNSString(notification.informativeText) : "";

	e->context.otherButton = notification.otherButtonTitle ? QString::fromNSString(notification.otherButtonTitle) : "";
	e->context.actionButton = notification.actionButtonTitle ? QString::fromNSString(notification.actionButtonTitle) : "";

	// Send it!
	QApplication* app = (QApplication*)QApplication::instance();
	app->postEvent(app, e);
}

@end
