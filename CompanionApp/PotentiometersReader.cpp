#include "PotentiometersReader.h"
#include "HidCommunicationThread.h"

#include <QDebug>


PotentiometersReader::PotentiometersReader(QObject* parent)
    : QObject(parent)
{
    qDebug() << "PotentiometersReader::PotentiometersReader";
    mPotValues = { 0, 0, 0, 0 };
}

PotentiometersReader::~PotentiometersReader() {
    qDebug() << "PotentiometersReader::~PotentiometersReader";
    if (mHidThread && mHidThread->isRunning()) {
        mHidThread->requestInterruption();
        mHidThread->wait();
    }
}


void PotentiometersReader::startReading(hid_device* device) {
    mHidThread = new HidCommunicationThread(device, this);
    QObject::connect(mHidThread, &HidCommunicationThread::messageReceived, this, &PotentiometersReader::onMessageReceived);
    QObject::connect(mHidThread, &HidCommunicationThread::readError, this, &PotentiometersReader::onReadError);
    QObject::connect(mHidThread, &HidCommunicationThread::finished, mHidThread, &QObject::deleteLater);
    mHidThread->start();
}

void PotentiometersReader::onMessageReceived(const std::vector<unsigned char>& data) {
    auto pot = data[1];
    auto value = data[2] + (data[3] << 8);

    if (pot >= 0 && pot < mPotValues.size()) {
        mPotValues[pot] = (value / 255.0) * 100;
        emit potentiometersUpdated(mPotValues);
    }
}

void PotentiometersReader::onReadError() {
    qWarning() << "hid device read error, probably the device was disconnected";
    emit potentiometersUpdated({});
}
