#include "../AudioOutputSwitcher.h"

#include <QDebug>


AudioOutputSwitcher::AudioOutputSwitcher(QObject* parent)
    : QObject(parent)
{
    qDebug() << "AudioOutputSwitcher::AudioOutputSwitcher";
}

AudioOutputSwitcher::~AudioOutputSwitcher() {
    qDebug() << "AudioOutputSwitcher::~AudioOutputSwitcher";
}


void AudioOutputSwitcher::onSwitchOutputRequested() {
    qDebug() << "AudioOutputSwitcher::onSwitchOutputRequested";
}
