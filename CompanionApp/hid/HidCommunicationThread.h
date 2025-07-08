#pragma once

#include <QThread>

extern "C" {
#include <hidapi.h>
}


class HidCommunicationThread : public QThread {
    Q_OBJECT
    void run() override;
public:
    explicit HidCommunicationThread(hid_device* device, QObject* parent = nullptr);
    ~HidCommunicationThread();

signals:
    void messageReceived(const std::vector<unsigned char>& data);
    void readError();

private:
    hid_device* mDevice{ nullptr };

    static const size_t BUFFER_LENGTH{ 32 };
    unsigned char mBuffer[BUFFER_LENGTH]{ 0 };

};
