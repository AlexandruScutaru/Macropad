#pragma once

#include <QObject>


class AudioOutputSwitcher : public QObject {
    Q_OBJECT
public:
    explicit AudioOutputSwitcher(QObject* parent = nullptr);
    ~AudioOutputSwitcher();

    void switchOutput();

public slots:
    void onHotKeyTriggered();

};
