#pragma once

#include <QObject>


class AudioOutputSwitcher : public QObject {
    Q_OBJECT
public:
    explicit AudioOutputSwitcher(QObject* parent = nullptr);
    ~AudioOutputSwitcher();

public slots:
    void onSwitchOutputRequested();

};
