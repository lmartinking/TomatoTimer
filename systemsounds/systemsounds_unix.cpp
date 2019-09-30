#include <QMap>
#include <QDirIterator>
#include <QProcess>
#include <QDebug>

#include "systemsounds.h"

#define SYSTEM_SOUNDS_PATH  "/usr/share/sounds"
#define PAPLAY_EXECUTABLE_PATH "/usr/bin/paplay"


typedef QMap<QString, QString> SystemSoundsMap;
const SystemSoundsMap& GetSystemSounds()
{
    static SystemSoundsMap sounds;
    if (sounds.empty()) {
        QStringList filters = { "*.wav", "*.oga", "*.ogg" };
        QDirIterator it(SYSTEM_SOUNDS_PATH, filters, QDir::NoFilter, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            it.next();
            auto f = it.fileInfo();
            sounds.insert(f.baseName(), f.absoluteFilePath());
        }
    }
    return sounds;
}


QStringList GetSystemSoundNames()
{
    qDebug() << "GetSystemSoundNames";
    return GetSystemSounds().keys();
}

void PlaySystemSound(const QString& name)
{
    auto path = GetSystemSounds().value(name, "");
    if (! path.isEmpty()) {
        qDebug() << "PlaySystemSound" << path;

        QStringList args = { "-n", "TomatoTimer", "--", path};
        QProcess::startDetached(PAPLAY_EXECUTABLE_PATH, args);
    }
}
