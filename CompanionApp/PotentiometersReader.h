#pragma once

#include <QObject>
#include <QPointer>

#include <vector>

class HidHelper;
class HidCommunicationThread;


class PotentiometersReader : public QObject {
    Q_OBJECT
public:
    explicit PotentiometersReader(QObject* parent = nullptr);
    ~PotentiometersReader();

    void startReading();

signals:
    void potentiometersUpdated(const std::vector<int>& values);

private slots:
    void onMessageReceived(const std::vector<unsigned char>& data);
    void onReadError();

private:
    QPointer<HidCommunicationThread> mHidThread{ nullptr };
    HidHelper* mHidHelper{ nullptr };
    std::vector<int> mPotValues;

};
