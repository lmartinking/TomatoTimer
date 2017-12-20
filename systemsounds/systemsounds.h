#ifndef SYSTEMSOUNDS_H
#define SYSTEMSOUNDS_H

#include <QStringList>

QStringList GetSystemSoundNames();

void PlaySystemSound(const QString& name);

#endif // SYSTEMSOUNDS_H
