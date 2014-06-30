#include "iqsettingmanager.h"
#include <QDebug>

IQSettingManager::IQSettingManager()
{
}

void IQSettingManager::setSettingParam(const QString &key, const QVariant &value)
{
    QSettings settings;
    settings.setValue(key, value);
}

QVariant IQSettingManager::settingParam(const QString &key, const QVariant &defaultValue)
{
    QSettings settings;
    if (!settings.contains(key))
        settings.setValue(key, defaultValue);
    return settings.value(key);
}

quint32 IQSettingManager::lastReservationId()
{
    QDateTime mouthAway = QDateTime::currentDateTime();
    return settingParam("lastReservationId", 0).toUInt();
}

