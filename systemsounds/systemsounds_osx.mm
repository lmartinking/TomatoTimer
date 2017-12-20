#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

#include <QDir>
#include <QFileInfo>

#include "systemsounds.h"


QStringList GetSystemSoundNames()
{
	QStringList names;
	QDir soundsDir("/System/Library/Sounds");
	for (auto& file : soundsDir.entryInfoList(QDir::Files))
	{
		QFileInfo info(file.absoluteFilePath());
		names << info.baseName();
	}
	return names;
}

void PlaySystemSound(const QString& name)
{
	[[NSSound soundNamed:name.toNSString()] play];
}
