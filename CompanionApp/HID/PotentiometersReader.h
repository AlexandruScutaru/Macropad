#pragma once

#include <QObject>
#include <QPointer>

#include <vector>

extern "C" {
#include <hidapi.h>
}

class HidCommunicationThread;


class PotentiometersReader : public QObject {
    Q_OBJECT
public:
    explicit PotentiometersReader(QObject* parent = nullptr);
    ~PotentiometersReader();

    void startReading(hid_device* device);

signals:
    void potentiometersUpdated(const std::vector<int>& values);

private slots:
    void onMessageReceived(const std::vector<unsigned char>& data);
    void onReadError();

private:
    QPointer<HidCommunicationThread> mHidThread{ nullptr };
    std::vector<int> mPotValues;

};
