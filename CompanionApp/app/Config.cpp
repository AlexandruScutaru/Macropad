#include "Config.h"

#include <QCoreApplication>
#include <QDebug>
#include <QSettings>

#define SETTINGS QSettings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName())


Config::Config(QObject* parent)
    : QObject(parent)
{
    qDebug() << "Config::Config";
}

Config::~Config() {
    qDebug() << "Config::~Config";
}


QVariant Config::getProperty(QAnyStringView key, const QVariant& defaultValue) const {
    auto settings = SETTINGS;

    if (!settings.contains(key)) {
        return defaultValue;
    }

    return settings.value(key);
}

void Config::setProperty(QAnyStringView key, const QVariant& value) {
    SETTINGS.setValue(key, value);
}
